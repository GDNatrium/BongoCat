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
        return std::string("gjFont") + (id - 1 < 10 ? "0" : "") + std::to_string(id - 1) + ".fnt";
    }

private:
    void onChangeBackground(CCObject* sender);
    void onChangeFont(CCObject* sender);

    int m_page = 0;

    void setPageId(std::string fnt) {
        if (fnt == "goldFont.fnt") m_page = 0;
        else if (fnt == "bigFont.fnt") m_page = 1;
        else if (fnt == "chatFont.fnt") m_page = 61;
        else {
            int num = std::stoi(fnt.substr(6, 2));
            m_page = num + 1;
        }
    }

    std::string getFontText(std::string fnt) {
        if (fnt == "goldFont.fnt") return "Gold";
        if (fnt == "bigFont.fnt") return "Font 1";
        if (fnt == "chatFont.fnt") return "Chat";

        int num = std::stoi(std::string(fnt).substr(6, 2));
        return "Font " + std::to_string(num + 1);
    }

};