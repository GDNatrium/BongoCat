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

    RowLayout* layout = RowLayout::create();
    layout->setGap(3);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    layout->setCrossAxisOverflow(true);

    // cat designs
    auto node = createSettingNode(80, false);
    scrollLayer->m_contentLayer->addChild(node);

    auto catMenu = createMenu(80);
    catMenu->setLayout(layout);
    node->addChildAtPosition(catMenu, Anchor::Top, ccp(0, -10));

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

    // hats
    auto nodeHats = createSettingNode(80, true);
    scrollLayer->m_contentLayer->addChild(nodeHats);

    auto hatsMenu = createMenu(80);
    nodeHats->addChildAtPosition(hatsMenu, Anchor::Top, ccp(0, -10));

    hatsMenu->setLayout(layout);

    std::array<int, 10> unlockValuesHats = { 0, 500, 5000, 20000, 30000, 50000, 70000, 80000, 90000, 100000 };

    for (int i = 1; i <= 10; ++i) {
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

    // deco
    auto nodeDeco = createSettingNode(50, false);
    scrollLayer->m_contentLayer->addChild(nodeDeco);

    auto decoMenu = createMenu(50);
    nodeDeco->addChildAtPosition(decoMenu, Anchor::Top, ccp(0, -10));
    decoMenu->setLayout(layout);

    std::array<int, 5> unlockValuesDeco = { 0, 1000, 1500, 3000, 4000 };

    for (int i = 1; i <= 5; ++i) {
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

    // settings
    auto nodeSettings = createSettingNode(260, true);
    scrollLayer->m_contentLayer->addChild(nodeSettings);

    auto settingsMenu = createMenu(260);
    nodeSettings->addChildAtPosition(settingsMenu, Anchor::Top, ccp(0, -10));

    settingsMenu->addChildAtPosition(createLabel("Scale"), Anchor::Top, ccp(-20, -10));

    settingsMenu->addChildAtPosition(createTextInputBG(), Anchor::Top, ccp(35, -11));
    auto scale = std::to_string(catNode->getScale()).substr(0, 4);
    auto scaleTextInput = createTextInput("scale", scale.c_str());
    settingsMenu->addChildAtPosition(scaleTextInput, Anchor::Top, ccp(35, -11));

    auto scaleVal = (catNode->getScale() - 0.25) / 4.75;
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onScaleChange), scaleVal), Anchor::Center, ccp(-20, 100));
    
    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetScale)), Anchor::Center, ccp(120, 100));

    // Pos X
    settingsMenu->addChildAtPosition(createLabel("Pos X"), Anchor::Top, ccp(-20, -65));

    settingsMenu->addChildAtPosition(createTextInputBG(), Anchor::Top, ccp(35, -66));
    auto xPos = std::to_string(catNode->getPositionX()).substr(0, 4);
    auto posXTextInput = createTextInput("posx", xPos.c_str());
    settingsMenu->addChildAtPosition(posXTextInput, Anchor::Top, ccp(35, -66));

    auto posXVal = (catNode->getPositionX() - 45) / (winSize.width - 90);
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onPosXChange), posXVal), Anchor::Center, ccp(-20, 45));

    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetPosX)), Anchor::Center, ccp(120, 45));

    // Pos Y
    settingsMenu->addChildAtPosition(createLabel("Pos Y"), Anchor::Top, ccp(-20, -120));

    settingsMenu->addChildAtPosition(createTextInputBG(), Anchor::Top, ccp(35, -121));
    auto yPos = std::to_string(catNode->getPositionY()).substr(0, 4);
    auto posYTextInput = createTextInput("posy", yPos.c_str());
    settingsMenu->addChildAtPosition(posYTextInput, Anchor::Top, ccp(35, -121));

    auto posYVal = (catNode->getPositionY() - 25) / (winSize.height - 50);
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onPosYChange), posYVal), Anchor::Center, ccp(-20, -10));

    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetPosY)), Anchor::Center, ccp(120, -10));

    // Flip X
    settingsMenu->addChildAtPosition(createLabel("Flip X"), Anchor::Top, ccp(-70, -185));

    auto checkBoxSpriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    checkBoxSpriteOff->setScale(0.6f);
    auto checkBoxSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    checkBoxSpriteOn->setScale(0.6f);
    auto checkBoxButton = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onFlipX));

    settingsMenu->addChildAtPosition(checkBoxButton, Anchor::Center, ccp(-120, -55));
    if (Mod::get()->getSavedValue<int>("scaleX") < 0) checkBoxButton->toggle(true);

    // Hide in Level
    settingsMenu->addChildAtPosition(createLabel("Hide in level"), Anchor::Top, ccp(75, -185));
    auto hideInLevelCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideInLevel));

    settingsMenu->addChildAtPosition(hideInLevelCheck, Anchor::Center, ccp(0, -55));
    if (Mod::get()->getSavedValue<bool>("hideLevel")) hideInLevelCheck->toggle(true);

    // Hide Counter
    settingsMenu->addChildAtPosition(createLabel("Hide Counter"), Anchor::Top, ccp(-40, -220));
    auto hideCounterCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideCounter));

    settingsMenu->addChildAtPosition(hideCounterCheck, Anchor::Center, ccp(-120, -90));
    if (Mod::get()->getSavedValue<bool>("hideCounter")) hideCounterCheck->toggle(true);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.5);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(BongoCatSettings::onShowInfo));
    settingsMenu->addChildAtPosition(infoBtn, Anchor::Center, ccp(25, -85));

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

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<CCTextInputNode>(0);
    textBox->setString(std::to_string(0.25 + value * 4.75).substr(0, 4));

    Mod::get()->setSavedValue<float>("scale", 0.25 + value * 4.75);
}

void BongoCatSettings::onResetScale(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(0)->setValue(0.158);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setScale(1);

    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<CCTextInputNode>(0)->setString("1.00");

    Mod::get()->setSavedValue<float>("scale", 1);
}

void BongoCatSettings::onPosXChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    catSpr->setPositionX(45 + value * (winSize.width - 90));

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<CCTextInputNode>(1);
    textBox->setString(std::to_string(45 + value * (winSize.width - 90)).substr(0, 4));

    Mod::get()->setSavedValue<float>("posX", 45 + value * (winSize.width - 90));
}

void BongoCatSettings::onResetPosX(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(1)->setValue(1);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionX(winSize.width - 45);

    auto xPos = std::to_string(winSize.width - 45).substr(0, 4);
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<CCTextInputNode>(1)->setString(xPos);

    Mod::get()->setSavedValue<float>("posX", winSize.width - 45);
}

void BongoCatSettings::onPosYChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<CCTextInputNode>(2);
    textBox->setString(std::to_string(25 + value * (winSize.height - 50)).substr(0, 4));

    catSpr->setPositionY(25 + value * (winSize.height - 50));
    Mod::get()->setSavedValue<float>("posY", 25 + value * (winSize.height - 50));
}

void BongoCatSettings::onResetPosY(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(2)->setValue(0);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionY(25);

    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<CCTextInputNode>(2)->setString("25.0");

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

CCTextInputNode* BongoCatSettings::createTextInput(char const* id, char const* placeholder) {
    auto textInput = CCTextInputNode::create(150, 20, placeholder, 0, "bigFont.fnt");
    textInput->setID(id);
    textInput->setDelegate(ScaleDelegate::get());
    textInput->setMaxLabelLength(4);
    textInput->setLabelPlaceholderColor({ 120, 140, 200 });
    textInput->addTextArea(TextArea::create("", "bigFont.fnt", 0.5f, 80, { 0.5, 0.5 }, 10.0f, true));
    textInput->setUserObject("fix-text-input", CCBool::create(true));
    textInput->setAllowedChars("0123456789.-");
    textInput->setAnchorPoint(CCPoint(0, 0.1));
    textInput->setScale(0.4f);

    return textInput;
}

void BongoCatSettings::onHideCounter(CCObject* sender) {
    BongoCat::m_hideCounter = !BongoCat::m_hideCounter;
    Mod::get()->setSavedValue<bool>("hideCounter", BongoCat::m_hideCounter);
}

void BongoCatSettings::onShowInfo(CCObject* sender) {
    FLAlertLayer::create("Settings Info", 
        "You can also access this menu through the <cy>Mod Settings Menu</c> "
        "if you can't open it through the normal button anymore.", 
        "OK")->show();
}