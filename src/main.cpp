#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/VideoOptionsLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "BongoCat.hpp"
#include "BongoSettings.hpp"
#include "SecretUnlocks.hpp"

bool added = false;

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		if(!added) {
			auto runningScene = CCDirector::sharedDirector()->getRunningScene();
			auto catNode = runningScene->getChildByType<BongoCat>(0);
			if (catNode) {
				catNode->removeFromParentAndCleanup(false);
				SceneManager::get()->forget(catNode);
			}

			BongoCat::addCat();
			added = true;
		}

		return true;
	}
};

class $modify(GJBaseGameLayer) {
	void handleButton(bool down, int button, bool p2) {
		GJBaseGameLayer::handleButton(down, button, p2);

		if (down && button == 1) {
			auto runningScene = CCDirector::sharedDirector()->getRunningScene();
			auto catNode = runningScene->getChildByType<BongoCat>(0);

			auto label = catNode->getChildByType<CCLabelBMFont>(0);
			auto count = std::stoi(label->getString());
			label->setString(std::to_string(count + 1).c_str());

			Mod::get()->setSavedValue<int>("count", count + 1);

			SecretUnlocks::m_clicksThisSession++;
			SecretUnlocks::checkSessionUnlocks();

			if (catNode->m_pawsToUse == 3) {
				catNode->setFrame(catNode->m_lastPaw ? 2 : 3);
				catNode->m_lastPaw = !catNode->m_lastPaw;
			}

			if (catNode->m_pawsToUse == 2) {
				catNode->setFrame(2);
			}

			if (catNode->m_pawsToUse == 1) {
				catNode->setFrame(3);
			}
		}

		if (!down) {
			auto runningScene = CCDirector::sharedDirector()->getRunningScene();
			auto catNode = runningScene->getChildByType<BongoCat>(0);
			catNode->setFrame(1);
		}
	}
};

// Fix graphics getting messed up on fullscreen switch or res change
#ifdef GEODE_IS_DESKTOP
class $modify(VideoOptionsLayer) {
	void onApply(CCObject * sender) {
		VideoOptionsLayer::onApply(sender);
		added = false;
	}
};
#endif

// Add a shortcut to the bongo cat settings menu into the mod settings menu
// since you can move the cat off-screen now and lose access to the menu
class BongoSettings : public SettingBaseValue<int> {
public:
	static Result<std::shared_ptr<SettingV3>> parse(std::string const&, std::string const&, matjson::Value const&) {
		return Ok(std::make_shared<BongoSettings>());
	};
	SettingNode* createNode(float width) override;
};

class ExtraBongoSettingNode : public SettingValueNode<BongoSettings> {
protected:
	bool init(std::shared_ptr<BongoSettings>& setting, float width) {
		if (!SettingValueNodeV3::init(setting, width))
			return false;

		this->setContentSize({ width, 40.f });

		auto* sprite = ButtonSprite::create("Open Settings", 0, false, "bigFont.fnt", "GJ_button_04.png", 24.5f, 0.4f);
		auto* btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(ExtraBongoSettingNode::onOpen));
		auto* menu = CCMenu::create();
		menu->setPosition({ width / 2, 20.f });
		menu->addChild(btn);
		this->addChild(menu);

		return true;
	}
	void onOpen(CCObject*) {
		BongoCatSettings::create()->show();
	}
public:
	static ExtraBongoSettingNode* create(std::shared_ptr<BongoSettings> value, float width) {
		auto ret = new ExtraBongoSettingNode();
		if (ret && ret->init(value, width)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

SettingNode* BongoSettings::createNode(float width) {
	return ExtraBongoSettingNode::create(std::static_pointer_cast<BongoSettings>(shared_from_this()), width);
}

$execute{
	(void)Mod::get()->registerCustomSettingType("open-menu", &BongoSettings::parse);
}