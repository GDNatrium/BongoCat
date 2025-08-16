#include "SecretUnlocks.hpp"
#include "CustomizeMenu.hpp"
#include "BongoCat.hpp"

int SecretUnlocks::m_secretOneTimesClicked = 0;
int SecretUnlocks::m_secretSevenTimesClicked = 0;
int SecretUnlocks::m_clicksThisSession = 0;
bool SecretUnlocks::m_hideAndSeekActive = false;

bool SecretUnlocks::m_secret1 = false;
bool SecretUnlocks::m_secret2 = false;
bool SecretUnlocks::m_secret3 = false;
bool SecretUnlocks::m_secret4 = false;
bool SecretUnlocks::m_secret5 = false;
bool SecretUnlocks::m_secret6 = false;
bool SecretUnlocks::m_secret7 = false;

void SecretUnlocks::setSecretUnlocks() {
	if (Mod::get()->getSavedValue<bool>("secret1")) m_secret1 = true;
	if (Mod::get()->getSavedValue<bool>("secret2")) m_secret2 = true;
	if (Mod::get()->getSavedValue<bool>("secret3")) m_secret3 = true;
	if (Mod::get()->getSavedValue<bool>("secret4")) m_secret4 = true;
	if (Mod::get()->getSavedValue<bool>("secret5")) m_secret5 = true;
	if (Mod::get()->getSavedValue<bool>("secret6")) m_secret6 = true;
	if (Mod::get()->getSavedValue<bool>("secret7")) m_secret7 = true;
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
	case 4:
		secretFour();
		break;
	case 5:
		secretFive();
		break;
	case 6:
		secretSix();
		break;
	case 7:
		secretSeven();
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
	case 4:
		return m_secret4;
	case 5:
		return m_secret5;
	case 6:
		return m_secret6;
	case 7:
		return m_secret7;
	default:
		return false;
	}
}

void SecretUnlocks::setUnlockSecret(int id, bool unlock) {
	switch (id) {
	case 1:
		m_secret1 = unlock;
		Mod::get()->setSavedValue<bool>("secret1", unlock);
		break;
	case 2:
		m_secret2 = unlock;
		Mod::get()->setSavedValue<bool>("secret2", unlock);
		break;
	case 3:
		m_secret3 = unlock;
		Mod::get()->setSavedValue<bool>("secret3", unlock);
		break;
	case 4:
		m_secret4 = unlock;
		Mod::get()->setSavedValue<bool>("secret4", unlock);
		break;
	case 5:
		m_secret5 = unlock;
		Mod::get()->setSavedValue<bool>("secret5", unlock);
		break;
	case 6:
		m_secret6= unlock;
		Mod::get()->setSavedValue<bool>("secret6", unlock);
		break;
	case 7:
		m_secret7 = unlock;
		Mod::get()->setSavedValue<bool>("secret7", unlock);
		break;
	}
}

void SecretUnlocks::checkSessionUnlocks() {
	if (m_clicksThisSession >= 500) setUnlockSecret(6, true);

	if (m_clicksThisSession >= 1000) setUnlockSecret(2, true);

	if (m_clicksThisSession >= 2500) setUnlockSecret(3, true);

	if (BongoCat::sessionTimer.elapsed() >= 1000 * 60 * 30) setUnlockSecret(4, true);

	if (BongoCat::sessionTimer.elapsed() >= 1000 * 60 * 60) setUnlockSecret(5, true);
}

std::string SecretUnlocks::formattedSessionTime() {
	int totalSeconds = static_cast<int>(std::round(BongoCat::sessionTimer.elapsed() / 1000.0f));
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	std::string result;
	if (hours > 0) {
		result += std::to_string(hours) + " hour" + (hours > 1 ? "s" : "");
		if (minutes > 0) result += ", " + std::to_string(minutes) + " minute" + (minutes > 1 ? "s" : "");
		if (seconds > 0) result += ", " + std::to_string(seconds) + " second" + (seconds > 1 ? "s" : "");
	}
	else if (minutes > 0) {
		result += std::to_string(minutes) + " minute" + (minutes > 1 ? "s" : "");
		if (seconds > 0) result += ", " + std::to_string(seconds) + " second" + (seconds > 1 ? "s" : "");
	}
	else {
		result += std::to_string(seconds) + " second" + (seconds != 1 ? "s" : "");
	}

	return result;
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

		setUnlockSecret(1, true);

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
}

void SecretUnlocks::secretThree() {
	FLAlertLayer::create("Cant Stop",
		"<cy>Get 2500 clicks in one session</c>\nto unlock this <cg>Cat</c>!\n\n"
		"<cj>" + std::to_string(m_clicksThisSession) + " clicks in this session.</c>",
		"OK")->show();
}

void SecretUnlocks::secretFour() {
	auto result = formattedSessionTime();

	FLAlertLayer::create("Tea Break",
		"<cy>Bongo for 30 minutes</c>\nto unlock this <cg>Deco</c>!\n\n"
		"<cj>" + result + " in this session.</c>",
		"OK")->show();
}

void SecretUnlocks::secretFive() {
	auto result = formattedSessionTime();

	FLAlertLayer::create("Spin Around",
		"<cy>Bongo for 1 hour</c>\nto unlock this <cg>Cat</c>!\n\n"
		"<cj>" + result + " in this session.</c>",
		"OK")->show();
}

void SecretUnlocks::secretSix() {
	FLAlertLayer::create("Patience",
		"<cy>Get 500 clicks in one session</c>\nto unlock this <cg>Hat</c>!\n\n"
		"<cj>" + std::to_string(m_clicksThisSession) + " clicks in this session.</c>",
		"OK")->show();
}

void SecretUnlocks::secretSeven() {
	SecretUnlocks::m_secretSevenTimesClicked++;

	SecretUnlocks::m_hideAndSeekActive = true;

	switch (m_secretSevenTimesClicked) {
	case 1:
		FLAlertLayer::create("Hey!",
			"Let's play a round of hide and seek!\n"
			"<cy>I go first, don't cheat!</c>",
			"OK")->show();
		break;

	case 2:
		FLAlertLayer::create("Hide and Seek",
			"I am already hidden! Come find me!",
			"OK")->show();
		break;

	case 3:
		FLAlertLayer::create("Hide and Seek",
			"No hints...",
			"OK")->show();
		break;

	case 4:
		FLAlertLayer::create("Hide and Seek",
			"Still hiding! You will never find me.",
			"OK")->show();
		break;

	case 5:
		FLAlertLayer::create("Hide and Seek",
			"If you don't find me in this session, you will have to ask me to play again next time!",
			"OK")->show();
		break;

	case 6:
		FLAlertLayer::create("Hide and Seek",
			"Okay, I have to stop talking now. Best of luck!",
			"OK")->show();
		break;

	default:
		FLAlertLayer::create("Hide and Seek",
			"<cg>I am currently hiding, come find me!</c>",
			"OK")->show();
		break;
	}
}