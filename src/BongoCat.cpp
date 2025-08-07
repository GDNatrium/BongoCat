#include "BongoCat.hpp"
#include "BongoSettings.hpp"

int BongoCat::m_catID = 1;
int BongoCat::m_hatID = 1;
int BongoCat::m_decoID = 1;
bool BongoCat::m_hideInLevel = false;
bool BongoCat::m_hideCounter = false;

BongoCat* BongoCat::create() {
	auto ret = new BongoCat();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void BongoCat::addCat() {
	auto sharedDir = CCDirector::sharedDirector();
	auto winSize = sharedDir->getWinSize();

	auto cat = BongoCat::create();
	cat->setID("BongoCat"_spr);
	sharedDir->getRunningScene()->addChild(cat);
	SceneManager::get()->keepAcrossScenes(cat);

	m_catID = Mod::get()->getSavedValue<int>("cat", 1);
	m_hatID = Mod::get()->getSavedValue<int>("extra", 1);
	m_decoID = Mod::get()->getSavedValue<int>("deco", 1);
	m_hideInLevel = Mod::get()->getSavedValue<bool>("hideLevel", false);
	m_hideCounter = Mod::get()->getSavedValue<bool>("hideCounter", false);
	auto scaleX = Mod::get()->getSavedValue<float>("scaleX", 1);
	auto newScale = Mod::get()->getSavedValue<float>("scale", 1);
	auto posX = Mod::get()->getSavedValue<float>("posX", winSize.width - 45);
	auto posY = Mod::get()->getSavedValue<float>("posY", 25);

	if (posX > winSize.width) posX = winSize.width - 45;
	if (posY > winSize.height) posY = 25;

	cat->setScale(newScale);
	cat->setPositionX(posX);
	cat->setPositionY(posY);
	
	auto sprName = fmt::format("cat{}_1.png"_spr, cat->m_catID);
	auto catSpr = CCSprite::createWithSpriteFrameName(sprName.c_str());
	catSpr->setAnchorPoint({ 0.5, 0.275 });
	catSpr->setID("main");
	catSpr->setZOrder(50);
	catSpr->setScaleX(scaleX);
	cat->addChild(catSpr);

	auto sprHatName = fmt::format("extra{}.png"_spr, cat->m_hatID);
	auto sprHat = CCSprite::createWithSpriteFrameName(sprHatName.c_str());
	sprHat->setZOrder(51);
	sprHat->setID("hat");
	sprHat->setAnchorPoint(ccp(0.5, 0.01));
	if (m_hatID == 1 || m_hatID == 6) sprHat->setVisible(false);
	sprHat->setScaleX(scaleX);
	cat->addChild(sprHat);

	auto gm = GameManager::get();
	auto playerHat = SimplePlayer::create(gm->getPlayerFrame());
	playerHat->setColors(gm->colorForIdx(gm->getPlayerColor()), gm->colorForIdx(gm->getPlayerColor2()));
	if (gm->m_playerGlow) playerHat->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
	playerHat->setID("hat2");
	playerHat->setPosition(scaleX > 0 ? ccp(8, 28) : ccp(-8, 28));
	playerHat->setScale(0.5);
	playerHat->setRotation(scaleX > 0 ? 8 : -8);
	sprHat->setZOrder(52);
	cat->addChild(playerHat);
	if (m_hatID != 6) playerHat->setVisible(false);

	auto sprDecoName = fmt::format("deco{}.png"_spr, cat->m_decoID);
	auto sprDeco = CCSprite::createWithSpriteFrameName(sprDecoName.c_str());
	sprDeco->setZOrder(51);
	sprDeco->setID("deco");
	sprDeco->setAnchorPoint(ccp(0.5, 0.01));
	if (m_hatID == 1) sprDeco->setVisible(false);
	sprDeco->setScaleX(scaleX);
	cat->addChild(sprDeco);


	auto box = CCScale9Sprite::create("GJ_square01.png");
	box->setContentSize({ 160, 44 });
	box->setScale(0.5);
	box->setPositionY(-10);
	cat->addChild(box);
	if (m_hideCounter) box->setVisible(false);

	auto shadow = CCScale9Sprite::create("square02_small.png");
	shadow->setContentSize({ 115, 30 });
	shadow->setOpacity(100);
	shadow->setScale(0.5);
	shadow->setPosition({ -8.5f, -10 });
	cat->addChild(shadow);
	if (m_hideCounter) shadow->setVisible(false);

	auto settingsSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
	settingsSpr->setScale(0.375);
	auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, cat, menu_selector(BongoCat::onSettings));
	settingsBtn->setPosition(ccp(29, -10));
	auto menu = CCMenu::create();
	menu->setPosition(ccp(0, 0));
	menu->setTouchPriority(-999);
	menu->addChild(settingsBtn);
	cat->addChild(menu);
	if (m_hideCounter) menu->setVisible(false);

	auto amount = Mod::get()->getSavedValue<int>("count", 0);
	// auto amount = 99980;
	auto count = CCLabelBMFont::create(std::to_string(amount).c_str(), "goldFont.fnt");
	count->setAnchorPoint(ccp(0, 0.5));
	count->setPosition(ccp(-36, -9));
	count->limitLabelWidth(52.f, 0.6f, 0.2f);
	cat->addChild(count);
	if (m_hideCounter) count->setVisible(false);
}

bool BongoCat::init() {
	this->schedule(schedule_selector(BongoCat::setToTop));
	this->setTouchEnabled(true);
	return true;
}

void BongoCat::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -999, false);
}

bool BongoCat::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	auto label = this->getChildByType<CCLabelBMFont>(0);
	auto count = std::stoi(label->getString());
	label->setString(std::to_string(count + 1).c_str());

	Mod::get()->setSavedValue<int>("count", count + 1);

	setFrame(m_lastPaw ? 2 : 3);
	m_lastPaw = !m_lastPaw;
	return true;
}

void BongoCat::ccTouchEnded(CCTouch* touch, CCEvent* event) {
	setFrame(1);
}

void BongoCat::setFrame(int frame) {
	if (!this) return;

	auto catSpr = static_cast<CCSprite*>(this->getChildByID("main"));
	if (!catSpr) return;

	std::string name = fmt::format("cat{}_{}.png"_spr, m_catID, frame);
	auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
	if (frameSpr) catSpr->setDisplayFrame(frameSpr);
}

void BongoCat::setToTop(float dt) {
	auto runningScene = CCDirector::sharedDirector()->getRunningScene();

	if (m_hideCounter) {
		auto bongoCat = runningScene->getChildByType<BongoCat*>(0);
		if (!bongoCat) return;

		bongoCat->getChildByType<CCScale9Sprite>(0)->setVisible(false);
		bongoCat->getChildByType<CCScale9Sprite>(1)->setVisible(false);
		bongoCat->getChildByType<CCMenu>(0)->setVisible(false);
		bongoCat->getChildByType<CCLabelBMFont>(0)->setVisible(false);
	}
	else {
		auto bongoCat = runningScene->getChildByType<BongoCat*>(0);
		if (!bongoCat) return;

		bongoCat->getChildByType<CCScale9Sprite>(0)->setVisible(true);
		bongoCat->getChildByType<CCScale9Sprite>(1)->setVisible(true);
		bongoCat->getChildByType<CCMenu>(0)->setVisible(true);
		bongoCat->getChildByType<CCLabelBMFont>(0)->setVisible(true);
	}

	if (runningScene->getChildByType<PlayLayer>(0) && m_hideInLevel) this->setVisible(false);
	else this->setVisible(true);

	if (BongoCat::m_hatID == 6) {
		auto gm = GameManager::get();
		auto playerHat = this->getChildByType<SimplePlayer>(0);
		playerHat->setColors(gm->colorForIdx(gm->getPlayerColor()), gm->colorForIdx(gm->getPlayerColor2()));
		if (gm->m_playerGlow) playerHat->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
		else playerHat->disableGlowOutline();

		if (playerHat->m_iconRequestID != gm->getPlayerFrame()) {
			auto scaleX = Mod::get()->getSavedValue<float>("scaleX", 1);

			playerHat->removeFromParentAndCleanup(true);

			auto playerHatNew = SimplePlayer::create(gm->getPlayerFrame());
			playerHatNew->setColors(gm->colorForIdx(gm->getPlayerColor()), gm->colorForIdx(gm->getPlayerColor2()));
			if (gm->m_playerGlow) playerHatNew->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
			playerHatNew->setID("hat2");
			playerHatNew->setPosition(scaleX > 0 ? ccp(8, 28) : ccp(-8, 28));
			playerHatNew->setScale(0.5);
			playerHatNew->setRotation(scaleX > 0 ? 8 : -8);

			this->addChild(playerHatNew);
		}
	}

	if (runningScene->getHighestChildZ() == this->getZOrder()) return;

	this->setZOrder(runningScene->getHighestChildZ() + 1);
}

void BongoCat::onSettings(CCObject* sender) {
	if (!CCDirector::sharedDirector()->getRunningScene()->getChildByID("BongoCatSettings")) {
		BongoCatSettings::create()->show();
	}
}