#include "BongoSettings.hpp"
#include "BongoCat.hpp"
#include "EditCounterMenu.hpp"

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
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = ScrollLayer::create({ 300, 200 });

    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
        ->setGap(5)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(scrollLayer->getContentHeight())
    );

    // settings
    auto nodeSettings = createSettingNode(290, true);
    scrollLayer->m_contentLayer->addChild(nodeSettings);

    auto settingsMenu = createMenu(290);
    nodeSettings->addChildAtPosition(settingsMenu, Anchor::Top, ccp(0, -10));

    settingsMenu->addChildAtPosition(createLabel("Scale", false), Anchor::Top, ccp(-20, -10));

    auto scale = std::to_string(catNode->getScale()).substr(0, 5);
    auto scaleTextInput = createTextInput("scale", scale.c_str());
    settingsMenu->addChildAtPosition(scaleTextInput, Anchor::Top, ccp(35, -11));

    auto scaleVal = (catNode->getScale() - 0.25) / 4.75;
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onScaleChange), scaleVal), Anchor::Top, ccp(-20, -35));
    
    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetScale)), Anchor::Top, ccp(120, -35));

    // Pos X
    settingsMenu->addChildAtPosition(createLabel("Pos X", false), Anchor::Top, ccp(-20, -65));

    auto xPos = std::to_string(catNode->getPositionX()).substr(0, 5);
    auto posXTextInput = createTextInput("posx", xPos.c_str());
    settingsMenu->addChildAtPosition(posXTextInput, Anchor::Top, ccp(35, -66));

    auto posXVal = catNode->getPositionX() / winSize.width;
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onPosXChange), posXVal), Anchor::Top, ccp(-20, -90));

    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetPosX)), Anchor::Top, ccp(120, -90));

    // Pos Y
    settingsMenu->addChildAtPosition(createLabel("Pos Y", false), Anchor::Top, ccp(-20, -120));

    auto yPos = std::to_string(catNode->getPositionY()).substr(0, 5);
    auto posYTextInput = createTextInput("posy", yPos.c_str());
    settingsMenu->addChildAtPosition(posYTextInput, Anchor::Top, ccp(35, -121));

    auto posYVal = catNode->getPositionY() / winSize.height;
    settingsMenu->addChildAtPosition(createSlider(menu_selector(BongoCatSettings::onPosYChange), posYVal), Anchor::Top, ccp(-20, -145));

    settingsMenu->addChildAtPosition(createResetButton(menu_selector(BongoCatSettings::onResetPosY)), Anchor::Top, ccp(120, -145));

    // Flip X
    settingsMenu->addChildAtPosition(createLabel("Flip X", true), Anchor::Top, ccp(-105, -185));

    auto checkBoxSpriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    checkBoxSpriteOff->setScale(0.6f);
    auto checkBoxSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    checkBoxSpriteOn->setScale(0.6f);
    auto checkBoxButton = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onFlipX));

    settingsMenu->addChildAtPosition(checkBoxButton, Anchor::Top, ccp(-120, -185));
    if (Mod::get()->getSavedValue<int>("scaleX") < 0) checkBoxButton->toggle(true);

    // Hide in Level
    settingsMenu->addChildAtPosition(createLabel("Hide in level", true), Anchor::Top, ccp(30, -185));
    auto hideInLevelCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideInLevel));

    settingsMenu->addChildAtPosition(hideInLevelCheck, Anchor::Top, ccp(15, -185));
    if (Mod::get()->getSavedValue<bool>("hideLevel")) hideInLevelCheck->toggle(true);

    // Hide Counter
    settingsMenu->addChildAtPosition(createLabel("Hide Counter", true), Anchor::Top, ccp(-105, -220));
    auto hideCounterCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideCounter));

    settingsMenu->addChildAtPosition(hideCounterCheck, Anchor::Top, ccp(-120, -220));
    if (Mod::get()->getSavedValue<bool>("hideCounter")) hideCounterCheck->toggle(true);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.4);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(BongoCatSettings::onShowInfo));
    settingsMenu->addChildAtPosition(infoBtn, Anchor::Top, ccp(-135, -210));

    // Edit Counter
    settingsMenu->addChildAtPosition(createLabel("Edit Counter", true), Anchor::Top, ccp(30, -220));
    auto editCounterSpr = CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png");
    auto editCounterBtn = CCMenuItemSpriteExtra::create(editCounterSpr, this, menu_selector(BongoCatSettings::onEditCounter));

    settingsMenu->addChildAtPosition(editCounterBtn, Anchor::Top, ccp(15, -220));

    // Use left paw
    settingsMenu->addChildAtPosition(createLabel("Use Left Paw", true), Anchor::Top, ccp(-105, -255));
    auto leftPawCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onUseLeftPaw));

    settingsMenu->addChildAtPosition(leftPawCheck, Anchor::Top, ccp(-120, -255));
    if (Mod::get()->getSavedValue<int>("pawsMode") % 2 == 1) leftPawCheck->toggle(true);

    // Use right paw
    settingsMenu->addChildAtPosition(createLabel("Use Right Paw", true), Anchor::Top, ccp(30, -255));
    auto rightPawCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onUseRightPaw));

    settingsMenu->addChildAtPosition(rightPawCheck, Anchor::Top, ccp(15, -255));
    auto savedVal = Mod::get()->getSavedValue<int>("pawsMode");
    if (savedVal == 2 || savedVal == 3) rightPawCheck->toggle(true);

    // ----------

    scrollLayer->m_contentLayer->updateLayout();

    return scrollLayer;
}

void BongoCatSettings::onScaleChange(CCObject* sender) {
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    catSpr->setScale(0.25 + value * 4.75);

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<TextInput>(0);
    textBox->setString(std::to_string(0.25 + value * 4.75).substr(0, 5));

    Mod::get()->setSavedValue<float>("scale", 0.25 + value * 4.75);
}

void BongoCatSettings::onResetScale(CCObject* sender) {
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(0)->setValue(0.158);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setScale(1);

    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<TextInput>(0)->setString("1.000");

    Mod::get()->setSavedValue<float>("scale", 1);
}

void BongoCatSettings::onPosXChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    auto posX = value * winSize.width;

    catSpr->setPositionX(posX);

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<TextInput>(1);
    textBox->setString(std::to_string(posX).substr(0, 5));

    Mod::get()->setSavedValue<float>("posX", posX);
}

void BongoCatSettings::onResetPosX(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(1)->setValue((winSize.width - 45) / winSize.width);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionX(winSize.width - 45);

    auto xPos = std::to_string(winSize.width - 45).substr(0, 5);
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<TextInput>(1)->setString(xPos);

    Mod::get()->setSavedValue<float>("posX", winSize.width - 45);
}

void BongoCatSettings::onPosYChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));

    auto yPos = value * winSize.height;

    auto textBox = static_cast<SliderThumb*>(sender)->getParent()->getParent()->getParent()->getChildByType<TextInput>(2);
    textBox->setString(std::to_string(yPos).substr(0, 5));

    catSpr->setPositionY(yPos);
    Mod::get()->setSavedValue<float>("posY", yPos);
}

void BongoCatSettings::onResetPosY(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<Slider>(2)->setValue(25 / winSize.height);
    auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat"));
    catSpr->setPositionY(25);

    static_cast<CCMenuItemSpriteExtra*>(sender)->getParent()->getChildByType<TextInput>(2)->setString("25.00");

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

TextInput* BongoCatSettings::createTextInput(char const* id, char const* placeholder) {
    auto textInput = TextInput::create(150, placeholder);
    textInput->getInputNode()->addTextArea(TextArea::create("", "bigFont.fnt", 0.5f, 80, { 0.5, 0.5 }, 10.0f, true));
    textInput->setID(id);
    textInput->setMaxCharCount(5);
    textInput->setFilter("0123456789.-");
    textInput->getBGSprite()->setContentSize({ 200, 60 });
    textInput->setScale(0.5);
    textInput->getInputNode()->setScale(1.5);
    textInput->getInputNode()->setAnchorPoint(ccp(0, -0.1));

    textInput->setCallback([=](const std::string& input) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto catNode = CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat");

        auto result = numFromString<float>(input);

        if (id == std::string("scale")) {
            auto value = (result.isOk() && result.unwrap() >= 0.01f) ? result.unwrap() : 0.01f;

            float val = (value - 0.25f) / 4.75f;
            auto slider = textInput->getParent()->getChildByType<Slider>(0);

            slider->setValue(val);
            slider->updateBar();

            catNode->setScale(value);
        }

        if (id == std::string("posx")) {
            auto value = result.isOk() ? result.unwrap() : 0.f;
            float val = value / winSize.width;

            auto slider = textInput->getParent()->getChildByType<Slider>(1);
            slider->setValue(val);
            slider->updateBar();

            catNode->setPositionX(value);
        }

        if (id == std::string("posy")) {
            auto value = result.isOk() ? result.unwrap() : 0.f;
            float val = value / winSize.height;

            auto slider = textInput->getParent()->getChildByType<Slider>(2);
            slider->setValue(val);
            slider->updateBar();

            catNode->setPositionY(value);
        }

        });
     
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

void BongoCatSettings::onEditCounter(CCObject* sender) {
    EditCounterMenu::create()->show();
}

void BongoCatSettings::onUseLeftPaw(CCObject* sender) {
    auto currentPaws = BongoCat::m_pawsToUse;
    if (currentPaws % 2 == 0) BongoCat::m_pawsToUse++;
    else BongoCat::m_pawsToUse--;

    Mod::get()->setSavedValue<int>("pawsMode", BongoCat::m_pawsToUse);
}

void BongoCatSettings::onUseRightPaw(CCObject* sender) {
    auto currentPaws = BongoCat::m_pawsToUse;
    if (currentPaws == 0 || currentPaws == 1) BongoCat::m_pawsToUse += 2;
    else BongoCat::m_pawsToUse -= 2;

    Mod::get()->setSavedValue<int>("pawsMode", BongoCat::m_pawsToUse);
}