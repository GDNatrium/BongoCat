#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

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
