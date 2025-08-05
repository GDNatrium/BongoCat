#include "BongoSettings.hpp"
#include "BongoCat.hpp"

bool BongoCatSettings::setup() {
    this->setID("BongoCatSettings");
    this->setTitle("Bongo Cat Settings");

    auto list = BongoCatSettings::createScrollLayer();
    list->moveToTop();
    m_mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2, -list->getContentHeight() / 2 - 13});

    auto borders = ListBorders::create();
    borders->setContentSize(list->getContentSize());
    m_mainLayer->addChildAtPosition(borders, Anchor::Center, ccp(0, -13));

    return true;
}

ScrollLayer* BongoCatSettings::createScrollLayer() {
    auto runningScene = CCDirector::sharedDirector()->getRunningScene();
    auto catNode = runningScene->getChildByID("natrium.bongo_cat/BongoCat");
    int count = std::stoi(catNode->getChildByType<CCLabelBMFont>(0)->getString());
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = ScrollLayer::create({ 300, 200 });

    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
        ->setGap(-1)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(scrollLayer->getContentHeight())
    );

    // cat designs
    auto node = CCNode::create();
    node->setContentSize({ scrollLayer->getContentWidth(), 80 });
    auto bg = CCLayerColor::create(ccc4(0, 0, 0, 50), node->getContentWidth(), node->getContentHeight());
    bg->ignoreAnchorPointForPosition(false);
    node->addChildAtPosition(bg, Anchor::Center);

    auto catMenu = CCMenu::create();
    catMenu->setContentSize(node->getContentSize());
    catMenu->setAnchorPoint(ccp(0.5, 1));
    node->addChildAtPosition(catMenu, Anchor::Top, ccp(0, -10));

    RowLayout* layout = RowLayout::create();
    layout->setGap(3);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    layout->setCrossAxisOverflow(true);
    catMenu->setLayout(layout);

    std::array<int, 9> unlockValuesCats = { 0, 2500, 10000, 25000, 35000, 40000, 60000, 90000, 100000};

    for (int i = 1; i <= 9; ++i) {
        CCSprite* catSpr = nullptr;
        bool isUnlocked = count >= unlockValuesCats[i - 1];

        if (isUnlocked) {
            auto spriteName = fmt::format("cat{}_2.png"_spr, i);
            catSpr = CCSprite::createWithSpriteFrameName(spriteName.c_str());
        }
        else {
            catSpr = CCSprite::createWithSpriteFrameName("locked.png"_spr);
            auto lockLabel = CCLabelBMFont::create(std::to_string(unlockValuesCats[i - 1]).c_str(), "goldFont.fnt");
            lockLabel->setScale(0.75);
            lockLabel->limitLabelWidth(45, 0.75, 0.3);
            catSpr->addChildAtPosition(lockLabel, Anchor::Center, ccp(0, -10));
        }

        catSpr->setScale(0.8f);

        auto catBtn = CCMenuItemSpriteExtra::create(catSpr, catMenu, menu_selector(BongoCatSettings::onUpdateCat));
        catBtn->setTag(i);

        if (!isUnlocked)
            catBtn->setEnabled(false);

        catMenu->addChild(catBtn);
    }

    catMenu->updateLayout();

    scrollLayer->m_contentLayer->addChild(node);

    // hats
    auto nodeHats = CCNode::create();
    nodeHats->setContentSize({ scrollLayer->getContentWidth(), 80 });
    auto bgHats = CCLayerColor::create(ccc4(0, 0, 0, 90), nodeHats->getContentWidth(), nodeHats->getContentHeight());
    bgHats->ignoreAnchorPointForPosition(false);
    nodeHats->addChildAtPosition(bgHats, Anchor::Center);

    auto hatsMenu = CCMenu::create();
    hatsMenu->setContentSize(nodeHats->getContentSize());
    hatsMenu->setAnchorPoint(ccp(0.5, 1));
    nodeHats->addChildAtPosition(hatsMenu, Anchor::Top, ccp(0, -10));

    hatsMenu->setLayout(layout);

    std::array<int, 8> unlockValuesHats = { 0, 500, 5000, 20000, 30000, 50000, 70000, 80000 };

    for (int i = 1; i <= 8; ++i) {
        auto catSpr = CCSprite::createWithSpriteFrameName("locked.png"_spr);
        CCSprite* hatsSpr = nullptr;
        bool isUnlocked = count >= unlockValuesHats[i - 1];

        if (isUnlocked) {
            if (i != 6) {
                auto spriteName = fmt::format("extra{}.png"_spr, i);
                hatsSpr = CCSprite::createWithSpriteFrameName(spriteName.c_str());
            }
            else {
                auto gm = GameManager::get();
                auto playerHat = SimplePlayer::create(gm->getPlayerFrame());
                playerHat->setColors(gm->colorForIdx(gm->getPlayerColor()), gm->colorForIdx(gm->getPlayerColor2()));
                if (gm->m_playerGlow) playerHat->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
                playerHat->setPosition(ccp(8, 28));
                playerHat->setScale(0.5);
                playerHat->setRotation(8);
                hatsSpr = playerHat;
            }
        }
        else {
            hatsSpr = CCSprite::createWithSpriteFrameName("extraLocked.png"_spr);
            auto lockLabel = CCLabelBMFont::create(std::to_string(unlockValuesHats[i - 1]).c_str(), "goldFont.fnt");
            lockLabel->setScale(0.75);
            lockLabel->limitLabelWidth(45, 0.75, 0.3);
            hatsSpr->addChildAtPosition(lockLabel, Anchor::Center, ccp(0, -10));
        }

        if (i == 1) hatsSpr->setVisible(false);
        hatsSpr->setAnchorPoint(ccp(0.5, 0.18));

        catSpr->setScale(0.8f);
        
        if (i == 6 && isUnlocked) {
            catSpr->addChildAtPosition(hatsSpr, Anchor::Center, ccp(8, 19));
        }
        else {
            catSpr->addChildAtPosition(hatsSpr, Anchor::Center);
        }

        auto catBtn = CCMenuItemSpriteExtra::create(catSpr, hatsMenu, menu_selector(BongoCatSettings::onUpdateExtra));
        catBtn->setTag(i);

        if (!isUnlocked)
            catBtn->setEnabled(false);

        hatsMenu->addChild(catBtn);
    }

    hatsMenu->updateLayout();

    scrollLayer->m_contentLayer->addChild(nodeHats);

    // deco
    auto nodeDeco = CCNode::create();
    nodeDeco->setContentSize({ scrollLayer->getContentWidth(), 50 });
    auto bgDeco = CCLayerColor::create(ccc4(0, 0, 0, 50), nodeDeco->getContentWidth(), nodeDeco->getContentHeight());
    bgDeco->ignoreAnchorPointForPosition(false);
    nodeDeco->addChildAtPosition(bgDeco, Anchor::Center);

    auto decoMenu = CCMenu::create();
    decoMenu->setContentSize(nodeDeco->getContentSize());
    decoMenu->setAnchorPoint(ccp(0.5, 1));
    nodeDeco->addChildAtPosition(decoMenu, Anchor::Top, ccp(0, -10));

    decoMenu->setLayout(layout);

    std::array<int, 3> unlockValuesDeco = { 0, 1000, 1500 };

    for (int i = 1; i <= 3; ++i) {
        auto catSpr = CCSprite::createWithSpriteFrameName("locked.png"_spr);
        CCSprite* decoSpr = nullptr;
        bool isUnlocked = count >= unlockValuesDeco[i - 1];

        if (isUnlocked) {
            auto spriteName = fmt::format("deco{}.png"_spr, i);
            decoSpr = CCSprite::createWithSpriteFrameName(spriteName.c_str());
        }
        else {
            decoSpr = CCSprite::createWithSpriteFrameName("extraLocked.png"_spr);
            auto lockLabel = CCLabelBMFont::create(std::to_string(unlockValuesDeco[i - 1]).c_str(), "goldFont.fnt");
            lockLabel->setScale(0.75);
            lockLabel->limitLabelWidth(45, 0.75, 0.3);
            decoSpr->addChildAtPosition(lockLabel, Anchor::Center, ccp(0, -10));
        }

        if (i == 1) decoSpr->setVisible(false);
        decoSpr->setAnchorPoint(ccp(0.5, 0.18));

        catSpr->setScale(0.8f);
        catSpr->addChildAtPosition(decoSpr, Anchor::Center);

        auto catBtn = CCMenuItemSpriteExtra::create(catSpr, decoMenu, menu_selector(BongoCatSettings::onUpdateDeco));
        catBtn->setTag(i);

        if (!isUnlocked)
            catBtn->setEnabled(false);

        decoMenu->addChild(catBtn);
    }

    decoMenu->updateLayout();

    scrollLayer->m_contentLayer->addChild(nodeDeco);

    // settings
    auto nodeSettings = CCNode::create();
    nodeSettings->setContentSize({ scrollLayer->getContentWidth(), 230 });
    auto bgSettings = CCLayerColor::create(ccc4(0, 0, 0, 90), nodeSettings->getContentWidth(), nodeSettings->getContentHeight());
    bgSettings->ignoreAnchorPointForPosition(false);
    nodeSettings->addChildAtPosition(bgSettings, Anchor::Center);

    auto settingsMenu = CCMenu::create();
    settingsMenu->setContentSize(nodeSettings->getContentSize());
    settingsMenu->setAnchorPoint(ccp(0.5, 1));

    scrollLayer->m_contentLayer->addChild(nodeSettings);

    auto scaleLabel = CCLabelBMFont::create("Scale", "bigFont.fnt");
    scaleLabel->setScale(0.5);
    settingsMenu->addChildAtPosition(scaleLabel, Anchor::Top, ccp(0, -10));

    auto scaleSlider = Slider::create(this, menu_selector(BongoCatSettings::onScaleChange));
    scaleSlider->setValue((catNode->getScale() - 0.25) / 4.75);
    scaleSlider->updateBar();
    settingsMenu->addChildAtPosition(scaleSlider, Anchor::Center, ccp(-20, 85));
    
    auto resetScaleSpr = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
    resetScaleSpr->setScale(0.75);
    auto resetScaleBtn = CCMenuItemSpriteExtra::create(resetScaleSpr, scaleSlider, menu_selector(BongoCatSettings::onResetScale));
    settingsMenu->addChildAtPosition(resetScaleBtn, Anchor::Center, ccp(120, 85));

    auto posXLabel = CCLabelBMFont::create("Pos X", "bigFont.fnt");
    posXLabel->setScale(0.5);
    settingsMenu->addChildAtPosition(posXLabel, Anchor::Top, ccp(0, -65));

    auto posXSlider = Slider::create(this, menu_selector(BongoCatSettings::onPosXChange));
    posXSlider->setValue((catNode->getPositionX() - 45) / (winSize.width - 90));
    posXSlider->updateBar();
    settingsMenu->addChildAtPosition(posXSlider, Anchor::Center, ccp(-20, 30));

    auto resetPosXSpr = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
    resetPosXSpr->setScale(0.75);
    auto resetPosXBtn = CCMenuItemSpriteExtra::create(resetPosXSpr, scaleSlider, menu_selector(BongoCatSettings::onResetPosX));
    settingsMenu->addChildAtPosition(resetPosXBtn, Anchor::Center, ccp(120, 30));

    auto posYLabel = CCLabelBMFont::create("Pos Y", "bigFont.fnt");
    posYLabel->setScale(0.5);
    settingsMenu->addChildAtPosition(posYLabel, Anchor::Top, ccp(0, -120));

    auto posYSlider = Slider::create(this, menu_selector(BongoCatSettings::onPosYChange));
    posYSlider->setValue((catNode->getPositionY() - 25) / (winSize.height - 50));
    posYSlider->updateBar();
    settingsMenu->addChildAtPosition(posYSlider, Anchor::Center, ccp(-20, -25));

    auto resetPosYSpr = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
    resetPosYSpr->setScale(0.75);
    auto resetPosYBtn = CCMenuItemSpriteExtra::create(resetPosYSpr, scaleSlider, menu_selector(BongoCatSettings::onResetPosY));
    settingsMenu->addChildAtPosition(resetPosYBtn, Anchor::Center, ccp(120, -25));

    auto checkBoxSpriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    checkBoxSpriteOff->setScale(0.6f);
    auto checkBoxSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    checkBoxSpriteOn->setScale(0.6f);
    auto checkBoxButton = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onFlipX));

    settingsMenu->addChildAtPosition(checkBoxButton, Anchor::Center, ccp(-120, -70));
    if (Mod::get()->getSavedValue<int>("scaleX") < 0) checkBoxButton->toggle(true);

    auto flipXLabel = CCLabelBMFont::create("Flip X", "bigFont.fnt");
    flipXLabel->setScale(0.5);
    settingsMenu->addChildAtPosition(flipXLabel, Anchor::Top, ccp(-70, -185));

    auto hideInLevelCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideInLevel));

    settingsMenu->addChildAtPosition(hideInLevelCheck, Anchor::Center, ccp(0, -70));
    if (Mod::get()->getSavedValue<bool>("hideLevel")) hideInLevelCheck->toggle(true);

    auto hideInLevelLabel = CCLabelBMFont::create("Hide in level", "bigFont.fnt");
    hideInLevelLabel->setScale(0.5);
    settingsMenu->addChildAtPosition(hideInLevelLabel, Anchor::Top, ccp(75, -185));

    nodeSettings->addChildAtPosition(settingsMenu, Anchor::Top, ccp(0, -10));

    // ----------

    scrollLayer->m_contentLayer->updateLayout();

    return scrollLayer;
}

void BongoCatSettings::onUpdateCat(CCObject* sender) {
    BongoCat::m_catID = sender->getTag();

    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat")->getChildByID("main"));
    if (!catSpr) return;

    std::string name = fmt::format("cat{}_{}.png"_spr, BongoCat::m_catID, 1);
    auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
    if (frameSpr) catSpr->setDisplayFrame(frameSpr);

    Mod::get()->setSavedValue<int>("cat", BongoCat::m_catID);
}

void BongoCatSettings::onUpdateExtra(CCObject* sender) {
    BongoCat::m_hatID = sender->getTag();

    auto extraSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat")->getChildByID("hat"));
    if (!extraSpr) return;

    if (BongoCat::m_hatID == 1) extraSpr->setVisible(false);
    else if (BongoCat::m_hatID == 6) {
        extraSpr->setVisible(false);
        static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat")->getChildByID("hat2"))->setVisible(true);
    }
    else {
        extraSpr->setVisible(true);
        static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat")->getChildByID("hat2"))->setVisible(false);
    }

    std::string name = fmt::format("extra{}.png"_spr, BongoCat::m_hatID);
    auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
    if (frameSpr) extraSpr->setDisplayFrame(frameSpr);

    Mod::get()->setSavedValue<int>("extra", BongoCat::m_hatID);
}

void BongoCatSettings::onUpdateDeco(CCObject* sender) {
    BongoCat::m_decoID = sender->getTag();

    auto extraSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat")->getChildByID("deco"));
    if (!extraSpr) return;

    if (BongoCat::m_decoID == 1) extraSpr->setVisible(false);
    else extraSpr->setVisible(true);

    std::string name = fmt::format("deco{}.png"_spr, BongoCat::m_decoID);
    auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
    if (frameSpr) extraSpr->setDisplayFrame(frameSpr);

    Mod::get()->setSavedValue<int>("deco", BongoCat::m_decoID);
}

void BongoCatSettings::onScaleChange(CCObject* sender) {
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    catSpr->setScale(0.25 + value * 4.75);

    Mod::get()->setSavedValue<float>("scale", 0.25 + value * 4.75);
}

void BongoCatSettings::onResetScale(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(0)->setValue(0.158);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setScale(1);

    Mod::get()->setSavedValue<float>("scale", 1);
}

void BongoCatSettings::onPosXChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    catSpr->setPositionX(45 + value * (winSize.width - 90));

    Mod::get()->setSavedValue<float>("posX", 45 + value * (winSize.width - 90));
}

void BongoCatSettings::onResetPosX(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(1)->setValue(1);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionX(winSize.width - 45);

    Mod::get()->setSavedValue<float>("posX", winSize.width - 45);
}

void BongoCatSettings::onPosYChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    catSpr->setPositionY(25 + value * (winSize.height - 50));
    Mod::get()->setSavedValue<float>("posY", 25 + value * (winSize.height - 50));
}

void BongoCatSettings::onResetPosY(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(2)->setValue(0);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionY(25);

    Mod::get()->setSavedValue<float>("posY", 25);
}

void BongoCatSettings::onFlipX(CCObject* sender) {
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    auto curScale = catSpr->getChildByID("main")->getScale();
    catSpr->getChildByID("main")->setScaleX(-curScale);
    catSpr->getChildByID("hat")->setScaleX(-curScale);
    catSpr->getChildByID("hat2")->setRotation(-catSpr->getChildByID("hat2")->getRotation());
    catSpr->getChildByID("hat2")->setPositionX(-catSpr->getChildByID("hat2")->getPositionX());
    catSpr->getChildByID("deco")->setScaleX(-curScale);

    Mod::get()->setSavedValue<int>("scaleX", -curScale);
}

void BongoCatSettings::onHideInLevel(CCObject* sender) {
    BongoCat::m_hideInLevel = !BongoCat::m_hideInLevel;
    Mod::get()->setSavedValue<bool>("hideLevel", BongoCat::m_hideInLevel);
}