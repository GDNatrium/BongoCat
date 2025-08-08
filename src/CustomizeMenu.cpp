#include "CustomizeMenu.hpp"
#include "BongoCat.hpp"
#include "BongoSettings.hpp"
#include "SecretUnlocks.hpp"

bool CustomizeMenu::added = false;
std::vector<CustomizeMenu::Items> CustomizeMenu::cats;
std::vector<CustomizeMenu::Items> CustomizeMenu::hats;
std::vector<CustomizeMenu::Items> CustomizeMenu::deco;

void CustomizeMenu::addCats() {
    cats.push_back({ "Bongo", 0, "cat1_2.png"_spr, 1, "locked.png"_spr, "Default" });
    cats.push_back({ "Lucky", 2500, "cat2_2.png"_spr, 2, "locked.png"_spr, "Click 2500 times" });
    cats.push_back({ "Orange", 10000, "cat3_2.png"_spr, 3, "locked.png"_spr, "Click 10000 times" });
    cats.push_back({ "Gray", 25000, "cat4_2.png"_spr, 4, "locked.png"_spr, "Click 25000 times" });
    cats.push_back({ "Luna", 35000, "cat5_2.png"_spr, 5, "locked.png"_spr, "Click 35000 times" });
    cats.push_back({ "Robot", 40000, "cat6_2.png"_spr, 6, "locked.png"_spr, "Click 40000 times" });
    cats.push_back({ "Fox", 60000, "cat7_2.png"_spr, 7, "locked.png"_spr, "Click 60000 times" });
    cats.push_back({ "Frog", 90000, "cat8_2.png"_spr, 8, "locked.png"_spr, "Click 90000 times" });
    cats.push_back({ "Axolotl", 100000, "cat9_2.png"_spr, 9, "locked.png"_spr, "Click 100000 times" });
    cats.push_back({ "Panda", -1, "cat10_2.png"_spr, 10, "locked.png"_spr, "Default", 3 });
}

void CustomizeMenu::addHats() {
    hats.push_back({ "None", 0, "extra1.png"_spr, 1, "extraLocked.png"_spr, "Default" });
    hats.push_back({ "Party Hat", 500, "extra2.png"_spr, 2, "extraLocked.png"_spr, "Click 500 times" });
    hats.push_back({ "Beanie", 5000, "extra3.png"_spr, 3, "extraLocked.png"_spr, "Click 5000 times" });
    hats.push_back({ "Chef", 20000, "extra4.png"_spr, 4, "extraLocked.png"_spr, "Click 20000 times" });
    hats.push_back({ "Sombrero", 30000, "extra5.png"_spr, 5, "extraLocked.png"_spr, "Click 30000 times" });
    hats.push_back({ "Dasher", 50000, "extra6.png"_spr, 6, "extraLocked.png"_spr, "Click 50000 times" });
    hats.push_back({ "Bunny", 70000, "extra7.png"_spr, 7, "extraLocked.png"_spr, "Click 70000 times" });
    hats.push_back({ "Top Hat", 80000, "extra8.png"_spr, 8, "extraLocked.png"_spr, "Click 80000 times" });
    hats.push_back({ "Witch", 90000, "extra9.png"_spr, 9, "extraLocked.png"_spr, "Click 90000 times" });
    hats.push_back({ "Crown", 100000, "extra10.png"_spr, 10, "extraLocked.png"_spr, "Click 100000 times" });
    hats.push_back({ "Buddy", -1, "extra12.png"_spr, 12, "extraLocked.png"_spr, "Default", 1 });
    hats.push_back({ "Leaf", -1, "extra11.png"_spr, 11, "extraLocked.png"_spr, "Default", 2 });
}

void CustomizeMenu::addDeco() {
    deco.push_back({ "None", 0, "extra1.png"_spr, 1, "extraLocked.png"_spr, "Default" });
    deco.push_back({ "Monocle", 1000, "deco2.png"_spr, 2, "extraLocked.png"_spr, "Click 1000 times" });
    deco.push_back({ "Moustache", 1500, "deco3.png"_spr, 3, "extraLocked.png"_spr, "Click 1500 times" });
    deco.push_back({ "Juice", 3000, "deco4.png"_spr, 4, "extraLocked.png"_spr, "Click 3000 times" });
    deco.push_back({ "Sunglasses", 4000, "deco5.png"_spr, 5, "extraLocked.png"_spr, "Click 4000 times" });
}

bool CustomizeMenu::setup() {
    this->setID("BongoCatSettings");
    this->setTitle("Bongo Cat Customization");

    if (!added) {
        addCats();
        addHats();
        addDeco();
        added = true;
    }

    SecretUnlocks::setSecretUnlocks();

    CustomizeMenu::m_page = 1;

    auto list = CustomizeMenu::createScrollLayer(cats, 100, "Cats", false);
    list->moveToTop();
    m_mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2 + 10, -list->getContentHeight() / 2 - 13 });

    auto borders = ListBorders::create();
    borders->setContentSize(list->getContentSize());
    borders->setZOrder(1);
    m_mainLayer->addChildAtPosition(borders, Anchor::Center, ccp(0, -13));

    auto prevPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto prevPageBtn = CCMenuItemSpriteExtra::create(prevPageSpr, this, menu_selector(CustomizeMenu::onChangePage));
    prevPageBtn->setTag(-1);
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(prevPageBtn, Anchor::Left, ccp(-30, 0));

    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(nextPageSpr, this, menu_selector(CustomizeMenu::onChangePage));
    nextPageBtn->setTag(1);
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(nextPageBtn, Anchor::Right, ccp(30, 0));

    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
    auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(CustomizeMenu::onSettings));
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(settingsBtn, Anchor::TopRight, ccp(-5, -5));

    return true;
}

ScrollLayer* CustomizeMenu::createScrollLayer(std::vector<Items> items, int type, char const* titleTxt, bool defaultCat) {
    auto runningScene = CCDirector::sharedDirector()->getRunningScene();
    auto catNode = runningScene->getChildByID("natrium.bongo_cat/BongoCat");
    int count = std::stoi(catNode->getChildByType<CCLabelBMFont>(0)->getString());
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = ScrollLayer::create({ 300, 200 });

    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
        ->setGap(5)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(scrollLayer->getContentHeight())
    );

    RowLayout* layout = RowLayout::create();
    layout->setGap(3);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    layout->setCrossAxisOverflow(true);

    auto offSetNodeStart = CCScale9Sprite::create("square02_small.png");
    offSetNodeStart->setOpacity(0);
    offSetNodeStart->setContentSize({ 280, 5 });
    scrollLayer->m_contentLayer->addChildAtPosition(offSetNodeStart, Anchor::Center);

    // -----

    int rowAmount = (items.size() + 3 - 1) / 3;
    float nodeHeight = rowAmount * 90 + 25;

    auto catSettingsNode = CCScale9Sprite::create("square02_small.png");
    catSettingsNode->setOpacity(50);
    catSettingsNode->setContentSize({280, nodeHeight});
    scrollLayer->m_contentLayer->addChildAtPosition(catSettingsNode, Anchor::Center);

    auto title = CCLabelBMFont::create(titleTxt, "bigFont.fnt");
    title->setScale(0.6);
    catSettingsNode->addChildAtPosition(title, Anchor::Top, ccp(0, -15));

    RowLayout* mainLayout = RowLayout::create();
    mainLayout->setGap(15);
    mainLayout->setAutoScale(false);
    mainLayout->setGrowCrossAxis(true);
    mainLayout->setCrossAxisOverflow(true);

    auto catMenu = CCMenu::create();
    catMenu->setContentSize({ 280, catSettingsNode->getContentHeight() - 30 });
    catMenu->setLayout(mainLayout);
    catMenu->setAnchorPoint(ccp(0.5, 1));
    catSettingsNode->addChildAtPosition(catMenu, Anchor::Top, ccp(0, -30));

    for (int i = 0; i < items.size(); ++i) {
        auto bg = CCScale9Sprite::create("square02_small.png");
        bg->setOpacity(50);
        bg->setContentSize({ 75, 75 });
        catMenu->addChild(bg);

        bool isUnlocked = count >= items[i].price && SecretUnlocks::isUnlocked(items[i].specialUnlock);

        auto name = CCLabelBMFont::create("???", "goldFont.fnt");
        name->limitLabelWidth(22, 0.5, 0.1);
        if (isUnlocked) name->setString(items[i].name);
        bg->addChildAtPosition(name, Anchor::Bottom, ccp(0, 15));

        auto itemMenu = CCMenu::create();
        itemMenu->setContentSize(bg->getContentSize());
        bg->addChildAtPosition(itemMenu, Anchor::Center);

        auto item = isUnlocked ? CCSprite::createWithSpriteFrameName(items[i].file) : CCSprite::createWithSpriteFrameName(items[i].lockedFile);

        auto itemBtn = CCMenuItemSpriteExtra::create(item, item, menu_selector(CustomizeMenu::onChangeItem));
        itemBtn->setTag(type + i);
        if (!isUnlocked) itemBtn->setEnabled(false);
        itemMenu->addChildAtPosition(itemBtn, Anchor::Center);

        // Cube
        if (items[i].name == "Dasher" && isUnlocked) {
            auto gm = GameManager::get();

            auto playerHat = SimplePlayer::create(gm->getPlayerFrame());
            playerHat->setColors(gm->colorForIdx(gm->getPlayerColor()), gm->colorForIdx(gm->getPlayerColor2()));
            if (gm->m_playerGlow) playerHat->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
            playerHat->setScale(0.5);
            playerHat->setRotation(8);
            itemBtn->addChildAtPosition(playerHat, Anchor::Center, ccp(8, 4));

            item->setVisible(false);
        }

        // Hats and extras
        if (defaultCat) {
            auto defaultSpr = CCSprite::createWithSpriteFrameName("locked.png"_spr);
            defaultSpr->setZOrder(-1);
            itemBtn->addChildAtPosition(defaultSpr, Anchor::Center, ccp(0, -16));
            itemBtn->setPositionY(itemBtn->getPositionY() + 16);
        }

        if (!isUnlocked) {
            auto lockSpr = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
            lockSpr->setScale(0.8);

            auto lockBtn = CCMenuItemSpriteExtra::create(lockSpr, lockSpr, menu_selector(CustomizeMenu::onLockedItem));
            lockBtn->setTag(type + i);

            itemMenu->addChildAtPosition(lockBtn, Anchor::Center);
        }
    }

    catMenu->updateLayout();

    // -----

    auto offSetNodeEnd = CCScale9Sprite::create("square02_small.png");
    offSetNodeEnd->setOpacity(0);
    offSetNodeEnd->setContentSize({ 280, 5 });
    scrollLayer->m_contentLayer->addChildAtPosition(offSetNodeEnd, Anchor::Center);

    scrollLayer->m_contentLayer->updateLayout();

    return scrollLayer;
}

void CustomizeMenu::onChangeItem(CCObject* sender) {
    auto tag = sender->getTag();

    if (tag >= 100 && tag < 200) {
        BongoCat::m_catID = cats[tag - 100].fileID;

        auto catSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByType<BongoCat>(0)->getChildByID("main"));
        if (!catSpr) return;

        std::string name = fmt::format("cat{}_{}.png"_spr, BongoCat::m_catID, 1);
        auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
        if (frameSpr) catSpr->setDisplayFrame(frameSpr);

        Mod::get()->setSavedValue<int>("cat", BongoCat::m_catID);
    }

    if (tag >= 200 && tag < 300) {
        BongoCat::m_hatID = hats[tag - 200].fileID;

        auto bongoCat = CCDirector::sharedDirector()->getRunningScene()->getChildByType<BongoCat>(0);

        auto extraSpr = static_cast<CCSprite*>(bongoCat->getChildByID("hat"));
        if (!extraSpr) return;

        auto playerSpr = static_cast<CCSprite*>(bongoCat->getChildByID("hat2"));
        if (!playerSpr) return;

        if (BongoCat::m_hatID == 1) extraSpr->setVisible(false);
        else if (BongoCat::m_hatID == 6) {
            extraSpr->setVisible(false);
            playerSpr->setVisible(true);
        }
        else {
            extraSpr->setVisible(true);
            playerSpr->setVisible(false);
        }

        std::string name = fmt::format("extra{}.png"_spr, BongoCat::m_hatID);
        auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
        if (frameSpr) extraSpr->setDisplayFrame(frameSpr);

        Mod::get()->setSavedValue<int>("extra", BongoCat::m_hatID);
    }

    if (tag >= 300 && tag < 400) {
        BongoCat::m_decoID = deco[tag - 300].fileID;

        auto extraSpr = static_cast<CCSprite*>(CCDirector::sharedDirector()->getRunningScene()->getChildByType<BongoCat>(0)->getChildByID("deco"));
        if (!extraSpr) return;

        if (BongoCat::m_decoID == 1) extraSpr->setVisible(false);
        else extraSpr->setVisible(true);

        std::string name = fmt::format("deco{}.png"_spr, BongoCat::m_decoID);
        auto frameSpr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
        if (frameSpr) extraSpr->setDisplayFrame(frameSpr);

        Mod::get()->setSavedValue<int>("deco", BongoCat::m_decoID);
    }
}

void CustomizeMenu::onLockedItem(CCObject* sender) {
    auto tag = sender->getTag();

    gd::string desc;
    char const* extraText;

    if (tag >= 100 && tag < 200) {
        if (cats[tag - 100].specialUnlock != 0) {
            SecretUnlocks::secretUnlock(cats[tag - 100].specialUnlock);
            return;
        }
        desc = cats[tag - 100].unlockReq;
        extraText = "Cat";
    }

    if (tag >= 200 && tag < 300) {
        if (hats[tag - 200].specialUnlock != 0) {
            SecretUnlocks::secretUnlock(hats[tag - 200].specialUnlock);
            return;
        }
        desc = hats[tag - 200].unlockReq;
        extraText = "Hat";
    }

    if (tag >= 300 && tag < 400) {
        desc = deco[tag - 300].unlockReq;
        extraText = "Deco";
    }

    FLAlertLayer::create("Unlock", "<cy>" + desc + "</c> to unlock this <cg>" + extraText + "</c>!", "OK")->show();
}

void CustomizeMenu::onChangePage(CCObject* sender) {
    m_page = m_page + sender->getTag();

    if (m_page < 1) m_page = 3;
    if (m_page > 3) m_page = 1;

    m_mainLayer->getChildByID("ScrollLayer")->removeFromParentAndCleanup(true);

    ScrollLayer* list;

    if(m_page == 1) list = CustomizeMenu::createScrollLayer(cats, 100, "Cats", false);
    if (m_page == 2) list = CustomizeMenu::createScrollLayer(hats, 200, "Hats", true);
    if (m_page == 3) list = CustomizeMenu::createScrollLayer(deco, 300, "Extras", true);

    list->moveToTop();
    m_mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2 + 10, -list->getContentHeight() / 2 - 13 });
}

void CustomizeMenu::onSettings(CCObject* sender) {
    BongoCatSettings::create()->show();
}