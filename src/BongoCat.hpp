#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BongoCat : public CCLayer {
public:
	static BongoCat* create();
	static void addCat();
	static int m_catID;
	static int m_hatID;
	static int m_decoID;
	static bool m_hideInLevel;

	bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	void ccTouchEnded(CCTouch* touch, CCEvent* event);

private:
	bool m_lastPaw = false;

	bool init();
	void registerWithTouchDispatcher();

	void setFrame(int frame);
	void setToTop(float dt);

	void onSettings(CCObject* sender);
};
