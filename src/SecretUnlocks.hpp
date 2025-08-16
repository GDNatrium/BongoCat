#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SecretUnlocks {
public:
	static void setSecretUnlocks();

	static void secretUnlock(int id);
	static bool isUnlocked(int id);
	static void setUnlockSecret(int id, bool unlock);

	static int m_clicksThisSession;
	static void checkSessionUnlocks();

	static bool m_hideAndSeekActive;

private:
	static void secretOne();
	static int m_secretOneTimesClicked;
	static bool m_secret1; // annoyed the lock

	static void secretTwo();
	static bool m_secret2; // 1000 clicks in 1 session

	static void secretThree();
	static bool m_secret3; // 2500 clicks in 1 session

	static void secretFour();
	static bool m_secret4; // 30 mins in 1 session

	static void secretFive();
	static bool m_secret5; // 1h in 1 session

	static void secretSix();
	static bool m_secret6; // 500 clicks in 1 session

	static void secretSeven();
	static int m_secretSevenTimesClicked;
	static bool m_secret7; // Hide and seek

	static std::string formattedSessionTime();
};