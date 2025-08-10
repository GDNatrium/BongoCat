#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class EditCounterMenu : public geode::Popup<> {
protected:
    bool setup() override;

public:
    static EditCounterMenu* create() {
        auto ret = new EditCounterMenu();
        if (ret->initAnchored(260.f, 160.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    static std::string getFontByID(int id) {
        if (id == 0) return "goldFont.fnt";
        if (id == 1) return "bigFont.fnt";
        if (id == 61) return "chatFont.fnt";
        return fmt::format("gjFont{:02d}.fnt", id - 1);
    }

private:
    void onChangeBackground(CCObject* sender);
    void onChangeFont(CCObject* sender);

    int m_page = -1;

    std::string getFontText(int id) {
        if (id == 0) return "Gold";
        if (id == 1) return "Font 1";
        if (id == 61) return "Chat";
        return fmt::format("Font {}", id);
    }

};