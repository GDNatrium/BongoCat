#include "BongoCat.hpp"
#include "CustomizeMenu.hpp"
#include "SecretUnlocks.hpp"
#include "EditCounterMenu.hpp"

int BongoCat::m_catID = 1;
int BongoCat::m_hatID = 1;
int BongoCat::m_decoID = 1;
bool BongoCat::m_hideInLevel = false;
bool BongoCat::m_hideCounter = false;
int BongoCat::m_pawsToUse = 3;
Timer<> BongoCat::sessionTimer;

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
	m_pawsToUse = Mod::get()->getSavedValue<int>("pawsMode", 3);

	auto scaleX = Mod::get()->getSavedValue<float>("scaleX", 1);
	auto newScale = Mod::get()->getSavedValue<float>("scale", 1);
	auto posX = Mod::get()->getSavedValue<float>("posX", winSize.width - 45);
	auto posY = Mod::get()->getSavedValue<float>("posY", 25);
	auto bgID = Mod::get()->getSavedValue<int>("bg", 1);
	auto fontID = Mod::get()->getSavedValue<int>("font", 0);

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
	if (m_decoID == 1) sprDeco->setVisible(false);
	sprDeco->setScaleX(scaleX);
	cat->addChild(sprDeco);

	std::string bgFile;
	if (bgID != 6) bgFile = "GJ_square0" + std::to_string(bgID) + ".png";
	else bgFile = "square02_small.png";

	auto box = CCScale9Sprite::create(bgFile.c_str());
	if (bgID == 6) box->setOpacity(100);
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

	auto fontFile = EditCounterMenu::getFontByID(fontID);
	auto scaleFactor = fontID == 0 ? 0.6 : 0.45;

	auto amount = Mod::get()->getSavedValue<int>("count", 0);
	auto count = CCLabelBMFont::create(std::to_string(amount).c_str(), fontFile.c_str());
	count->setAnchorPoint(fontID <= 2 ? ccp(0, 0.5) : ccp(0, 0.6));
	count->setPosition(ccp(-36, -9));
	count->limitLabelWidth(52.f, scaleFactor, 0.2f);
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
	auto pauseLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<PauseLayer>(0);
	if (PlayLayer::get() && !pauseLayer) return true;

	if (GJBaseGameLayer::get() && GJBaseGameLayer::get()->m_playbackMode == PlaybackMode::Playing) return true;

	auto label = this->getChildByType<CCLabelBMFont>(0);
	auto count = std::stoi(label->getString());
	label->setString(std::to_string(count + 1).c_str());

	Mod::get()->setSavedValue<int>("count", count + 1);

	SecretUnlocks::m_clicksThisSession++;
	SecretUnlocks::checkSessionUnlocks();

	if (m_pawsToUse == 3) {
		setFrame(m_lastPaw ? 2 : 3);
		m_lastPaw = !m_lastPaw;
	}

	if (m_pawsToUse == 2) {
		setFrame(2);
	}

	if (m_pawsToUse == 1) {
		setFrame(3);
	}

	return true;
}

void BongoCat::ccTouchEnded(CCTouch* touch, CCEvent* event) {
	setFrame(1);
}

void BongoCat::setFrame(int frame) {
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

	// Changing Bongo Cat z-order down again to only be one above the current highest node at all times.
	// Some menus with set z-orders would appear behind their popup if Bongo Cat previously set it's ID to be above them. 
	// With popups appearing at the highest z-order, they then ended up above their submenus.
	// Also feel like this is just better, otherwise any popup would boost the z order more and more lol
	auto children = runningScene->getChildren();
	auto highest = static_cast<CCNode*>(children->objectAtIndex(children->count() - 1));

	if (highest->getID() == "natrium.bongo_cat/BongoCat") {
		auto secondHighest = static_cast<CCNode*>(children->objectAtIndex(children->count() - 2));
		this->setZOrder(secondHighest->getZOrder() + 1);
	}
	else {
		this->setZOrder(highest->getZOrder() + 1);
	}
}

void BongoCat::onSettings(CCObject* sender) {
	if (!CCDirector::sharedDirector()->getRunningScene()->getChildByID("BongoCatSettings")) {
		CustomizeMenu::create()->show();
	}
}