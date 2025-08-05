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
};
