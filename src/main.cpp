#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

#include "BongoCat.hpp"

bool added = false;

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		// $on_mod(Loaded) crashes
		if(!added) {
			BongoCat::addCat();
			added = true;
		}

		return true;
	}
};

class $modify(CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
		if(!CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat)) return false;

		if (!PlayLayer::get() && !LevelEditorLayer::get()) return true;

		bool isJumpKey = key == enumKeyCodes::KEY_Up || key == enumKeyCodes::KEY_W || key == enumKeyCodes::KEY_Space;

		if (isKeyDown && !isKeyRepeat && isJumpKey) {
			auto runningScene = CCDirector::sharedDirector()->getRunningScene();
			auto catNode = runningScene->getChildByType<BongoCat>(0);
			catNode->ccTouchBegan(nullptr, nullptr);
		}

		if (!isKeyDown) {
			auto runningScene = CCDirector::sharedDirector()->getRunningScene();
			auto catNode = runningScene->getChildByType<BongoCat>(0);
			catNode->ccTouchEnded(nullptr, nullptr);
		}

		return true;
	}
};