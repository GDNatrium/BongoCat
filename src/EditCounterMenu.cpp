#include "EditCounterMenu.hpp"

bool EditCounterMenu::setup() {
    this->setID("BongoCatSettings");
    this->setTitle("Edit Counter");

    auto bongoCat = CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat");

    auto bgLabel = CCLabelBMFont::create("Background", "bigFont.fnt");
    bgLabel->setScale(0.5);

    m_mainLayer->addChildAtPosition(bgLabel, Anchor::Top, ccp(0, -45));

    RowLayout* layout = RowLayout::create();
    layout->setGap(8);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);

    auto bgMenu = CCMenu::create();
    bgMenu->setContentSize(m_mainLayer->getContentSize());
    bgMenu->setLayout(layout);
    m_mainLayer->addChildAtPosition(bgMenu, Anchor::Top, ccp(0, -75));

    for (int i = 1; i <= 5; ++i) {
        std::string filename = "GJ_square0" + std::to_string(i) + ".png";
        auto sprite = CCScale9Sprite::create(filename.c_str());
        sprite->setScale(0.4);
        auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(EditCounterMenu::onChangeBackground));
        button->setTag(i);
        bgMenu->addChild(button);
    }

    auto sprite = CCScale9Sprite::create("square02_001.png");
    sprite->setScale(0.4);
    sprite->setOpacity(100);
    auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(EditCounterMenu::onChangeBackground));
    button->setTag(6);
    bgMenu->addChild(button);

    bgMenu->updateLayout();

    auto fontLabel = CCLabelBMFont::create("Font", "bigFont.fnt");
    fontLabel->setScale(0.5);

    m_mainLayer->addChildAtPosition(fontLabel, Anchor::Top, ccp(0, -105));

    auto fontMenu = CCMenu::create();
    fontMenu->setContentSize(m_mainLayer->getContentSize());
    m_mainLayer->addChildAtPosition(fontMenu, Anchor::Top, ccp(0, -130));

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    backSpr->setScale(0.6);
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(EditCounterMenu::onChangeFont));
    backBtn->setTag(-1);
    fontMenu->addChildAtPosition(backBtn, Anchor::Left, ccp(50, -2));

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);
    nextSpr->setScale(0.6);
    auto nextBtn = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(EditCounterMenu::onChangeFont));
    nextBtn->setTag(1);
    fontMenu->addChildAtPosition(nextBtn, Anchor::Right, ccp(-50, -2));

    auto fntFile = bongoCat->getChildByType<CCLabelBMFont>(0)->getFntFile();

    if(m_page == -1) m_page = Mod::get()->getSavedValue<int>("font", 0);

    auto fontPreview = CCLabelBMFont::create(getFontText(m_page).c_str(), fntFile);
    fontPreview->setScale(m_page == 0 ? 0.8 : 0.65);
    fontMenu->addChildAtPosition(fontPreview, Anchor::Center);

    return true;
}

void EditCounterMenu::onChangeBackground(CCObject* sender) {
    bool isTag6 = sender->getTag() == 6;
    auto bongoCat = CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat");
    auto bg = bongoCat->getChildByType<CCScale9Sprite>(0);

    auto contentSize = bg->getContentSize();

    std::string texture;
    if (!isTag6) texture = "GJ_square0" + std::to_string(sender->getTag()) + ".png";
    else texture = "square02_small.png";

    auto size = !isTag6 ? 80.f : 40.f;
    auto spriteFrame = CCSpriteFrame::create(texture.c_str(), {0, 0, size, size});
    bg->setSpriteFrame(spriteFrame);
    bg->setContentSize(contentSize);
    bg->setOpacity(!isTag6 ? 255 : 100);

    Mod::get()->setSavedValue<int>("bg", sender->getTag());
}

void EditCounterMenu::onChangeFont(CCObject* sender) {
    auto bongoCat = CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat");
    auto counterLabel = bongoCat->getChildByType<CCLabelBMFont>(0);
    auto previewLabel = m_mainLayer->getChildByType<CCMenu>(1)->getChildByType<CCLabelBMFont>(0);

    m_page += sender->getTag();

    if (m_page > 61) m_page = 0;
    if (m_page < 0) m_page = 61;

    auto newFntFile = getFontByID(m_page);
    auto newFntText = getFontText(m_page);

    previewLabel->setScale(m_page == 0 ? 0.8 : 0.65);
    previewLabel->setFntFile(newFntFile.c_str());
    previewLabel->setString(newFntText.c_str());

    auto value = counterLabel->getString();

    auto count = CCLabelBMFont::create(value, newFntFile.c_str());
    count->setAnchorPoint(m_page <= 2 ? ccp(0, 0.5) : ccp(0, 0.6));
    count->setPosition(ccp(-36, -9));
    auto scaleFactor = m_page == 0 ? 0.6 : 0.45;
    count->limitLabelWidth(52.f, scaleFactor, 0.2f);
    bongoCat->addChild(count);

    counterLabel->removeFromParent();

    Mod::get()->setSavedValue<int>("font", m_page);
}