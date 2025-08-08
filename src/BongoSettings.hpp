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

    TextInput* createTextInput(char const* id, char const* placeholder);

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

    CCLabelBMFont* createLabel(char const* txt, bool leftAlign) {
        auto label = CCLabelBMFont::create(txt, "bigFont.fnt");
        label->limitLabelWidth(100, 0.5, 0.1);
        if(leftAlign) label->setAnchorPoint(ccp(0, 0.5));
        return label;
    }

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