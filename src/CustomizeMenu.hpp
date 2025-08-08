#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomizeMenu : public geode::Popup<> {
protected:
    bool setup() override;

public:
    static CustomizeMenu* create() {
        auto ret = new CustomizeMenu();
        if (ret->initAnchored(320.f, 250.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    struct Items {
        char const* name;
        int price;
        char const* file;
        int fileID;
        char const* lockedFile;
        char const* unlockReq;
        int specialUnlock = 0;
    };

    int m_page;
    static ScrollLayer* createScrollLayer(std::vector<Items>, int type, char const* title, bool defaultCat);

    static std::vector<Items> cats;
    static std::vector<Items> hats;
    static std::vector<Items> deco;

private:
    static bool added;

    void addCats();
    void addHats();
    void addDeco();

    void onChangeItem(CCObject* sender);
    void onLockedItem(CCObject* sender);

    void onChangePage(CCObject* sender);
    void onSettings(CCObject* sender);
};
