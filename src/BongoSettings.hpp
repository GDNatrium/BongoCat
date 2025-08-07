#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BongoCatSettings : public geode::Popup<> {
protected:
    bool setup() override;

public:
    static BongoCatSettings* create() {
        auto ret = new BongoCatSettings();
        if (ret->initAnchored(320.f, 250.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

private:
    ScrollLayer* createScrollLayer();

    void onUpdateCat(CCObject* sender);
    void onUpdateExtra(CCObject* sender);
    void onUpdateDeco(CCObject* sender);

    void onScaleChange(CCObject* sender);
    void onResetScale(CCObject* sender);

    void onPosXChange(CCObject* sender);
    void onResetPosX(CCObject* sender);

    void onPosYChange(CCObject* sender);
    void onResetPosY(CCObject* sender);

    void onFlipX(CCObject* sender);
    void onHideInLevel(CCObject* sender);
    void onHideCounter(CCObject* sender);
    void onShowInfo(CCObject* sender);

    CCNode* createSettingNode(float height, bool darker) {
        auto node = CCNode::create();
        node->setContentSize({ 300, height });

        auto bg = CCLayerColor::create(darker ? ccc4(0, 0, 0, 90) : ccc4(0, 0, 0, 50), node->getContentWidth(), node->getContentHeight());
        bg->ignoreAnchorPointForPosition(false);
        node->addChildAtPosition(bg, Anchor::Center);

        return node;
    }

    CCMenu* createMenu(float height) {
        auto menu = CCMenu::create();
        menu->setContentSize({ 300, height });
        menu->setAnchorPoint(ccp(0.5, 1));
        return menu;
    }

    CCLabelBMFont* createLabel(char const* txt) {
        auto label = CCLabelBMFont::create(txt, "bigFont.fnt");
        label->setScale(0.5);
        return label;
    }

    CCScale9Sprite* createTextInputBG() {
        auto bg = CCScale9Sprite::create("square02b_001.png");
        bg->setColor({ 0,0,0 });
        bg->setOpacity(100);
        bg->setContentSize({ 200, 60 });
        bg->setScale(0.25f);
        return bg;
    }

    CCTextInputNode* createTextInput(char const* id, char const* placeholder);

    Slider* createSlider(SEL_MenuHandler menu, float val) {
        auto slider = Slider::create(this, menu);
        slider->setValue(val);
        slider->updateBar();
        return slider;
    }

    CCMenuItemSpriteExtra* createResetButton(SEL_MenuHandler menu) {
        auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
        resetSpr->setScale(0.75);
        auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu);
        return resetBtn;
    }
};

class ScaleDelegate : public TextInputDelegate {
public:
    static ScaleDelegate* get() {
        static ScaleDelegate instance;
        return &instance;
    }

    virtual void textChanged(CCTextInputNode* p0) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto catNode = CCDirector::sharedDirector()->getRunningScene()->getChildByID("natrium.bongo_cat/BongoCat");
        auto id = p0->getID();

        if (id == "scale") {
            auto value = 0.0f;
            if (p0->getString() != "") value = std::stof(p0->getString());
            if (value < 0.01) value = 0.01;

            auto val = (value - 0.25) / 4.75;

            auto slider = p0->getParent()->getChildByType<Slider>(0);
            slider->setValue(val);
            slider->updateBar();

            catNode->setScale(value);
        }

        if (id == "posx") {
            auto value = 0.0f;
            if (p0->getString() != "") value = std::stof(p0->getString());

            auto val = (value - 45) / (winSize.width - 90);

            auto slider = p0->getParent()->getChildByType<Slider>(1);
            slider->setValue(val);
            slider->updateBar();

            catNode->setPositionX(value);
        }

        if (id == "posy") {
            auto value = 0.0f;
            if (p0->getString() != "") value = std::stof(p0->getString());

            auto val = (value - 25) / (winSize.height - 50);

            auto slider = p0->getParent()->getChildByType<Slider>(2);
            slider->setValue(val);
            slider->updateBar();

            catNode->setPositionY(value);
        }
    }
};