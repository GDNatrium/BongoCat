#include "SecretUnlocks.hpp"
#include "CustomizeMenu.hpp"

int SecretUnlocks::m_secretOneTimesClicked = 0;
int SecretUnlocks::m_clicksThisSession = 0;

bool SecretUnlocks::m_secret1 = false;
bool SecretUnlocks::m_secret2 = false;
bool SecretUnlocks::m_secret3 = false;

void SecretUnlocks::setSecretUnlocks() {
	if (Mod::get()->getSavedValue<bool>("secret1")) m_secret1 = true;
	if (Mod::get()->getSavedValue<bool>("secret2")) m_secret2 = true;
	if (Mod::get()->getSavedValue<bool>("secret3")) m_secret3 = true;
}

void SecretUnlocks::secretUnlock(int id) {
	switch (id) {
	case 1:
		secretOne();
		break;
	case 2:
		secretTwo();
		break;
	case 3:
		secretThree();
		break;
	}

}

bool SecretUnlocks::isUnlocked(int id) {
	switch (id) {
	case 0:
		return true;
	case 1:
		return m_secret1;
	case 2:
		return m_secret2;
	case 3:
		return m_secret3;
	default:
		return false;
	}
}

void SecretUnlocks::checkSessionUnlocks() {
	if (m_clicksThisSession >= 1000) {
		Mod::get()->setSavedValue<bool>("secret2", true);
		m_secret2 = true;
	}

	if (m_clicksThisSession >= 2500) {
		Mod::get()->setSavedValue<bool>("secret3", true);
		m_secret3 = true;
	}
}

void SecretUnlocks::secretOne() {
	SecretUnlocks::m_secretOneTimesClicked++;

	switch (m_secretOneTimesClicked) {
	case 1:
		FLAlertLayer::create("STOP!", "<cr>Hey, stop doing this!!!</c>", "OK?")->show();
		break;

	case 2:
		FLAlertLayer::create("Please", "Pretty please, this is annoying...", "OK")->show();
		break;

	case 3:
		FLAlertLayer::create("FINE", "<cr>You won't stop, huh?</c>\nI just won't respond anymore...", "OK")->show();
		break;

	case 5:
		FLAlertLayer::create("...", "Nope, nothing.", "OK")->show();
		break;

	case 10:
		FLAlertLayer::create("...", "...", "OK")->show();
		break;

	case 15:
		FLAlertLayer::create("...", "......", "OK")->show();
		break;

	case 20:
		FLAlertLayer::create("GAH", "You can't help it, right?", "OK")->show();
		break;

	case 30:
		FLAlertLayer::create("...", "Fine. <cy>You win.</c>", "OK")->show();

		m_secret1 = true;
		Mod::get()->setSavedValue<bool>("secret1", true);

		auto mainLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeMenu>(0)->m_mainLayer;
		mainLayer->getChildByID("ScrollLayer")->removeFromParentAndCleanup(true);

		auto list = CustomizeMenu::createScrollLayer(CustomizeMenu::hats, 200, "Hats", true);
		mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2 + 10, -list->getContentHeight() / 2 - 13 });

		break;
	}
}

void SecretUnlocks::secretTwo() {
	FLAlertLayer::create("Touch Grass", 
		"<cy>Get 1000 clicks in one session</c>\nto unlock this <cg>Hat</c>!\n\n"
		"<cj>" + std::to_string(m_clicksThisSession) + " clicks in this session.</c>",
		"OK")->show();

	if (m_clicksThisSession >= 1000) {
		Mod::get()->setSavedValue<bool>("secret2", true);
		m_secret2 = true;

		auto mainLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeMenu>(0)->m_mainLayer;
		mainLayer->getChildByID("ScrollLayer")->removeFromParentAndCleanup(true);

		auto list = CustomizeMenu::createScrollLayer(CustomizeMenu::hats, 200, "Hats", true);
		mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2 + 10, -list->getContentHeight() / 2 - 13 });
	}
}

void SecretUnlocks::secretThree() {
	FLAlertLayer::create("Cant Stop",
		"<cy>Get 2500 clicks in one session</c>\nto unlock this <cg>Cat</c>!\n\n"
		"<cj>" + std::to_string(m_clicksThisSession) + " clicks in this session.</c>",
		"OK")->show();

	if (m_clicksThisSession >= 2500) {
		Mod::get()->setSavedValue<bool>("secret3", true);
		m_secret2 = true;

		auto mainLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeMenu>(0)->m_mainLayer;
		mainLayer->getChildByID("ScrollLayer")->removeFromParentAndCleanup(true);

		auto list = CustomizeMenu::createScrollLayer(CustomizeMenu::cats, 100, "Cats", false);
		mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2 + 10, -list->getContentHeight() / 2 - 13 });
	}
}