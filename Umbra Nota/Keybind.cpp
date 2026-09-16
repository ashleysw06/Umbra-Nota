#include <iostream>
#include "Keybind.h"

Keybind::Keybind(char aKeys[]) {
	for (size_t i = 0; i < sizeof(keys); i++) {
		keys[i] = aKeys[i];
	}
}

void Keybind::Update() {
	std::cout << keys[0];
	lastState = isActive;
	isActive = GetActive();
}

bool Keybind::GetActive() {
    for (int i = 0; i < sizeof(keys); ++i) {
		if (keys[i] == NULL) continue;

		//	This may seem random but it needs to be here for some reason?
		// 
		//	Fixes bug:
		//	Even after releasing key pressing a previous key will return true
		GetAsyncKeyState(keys[i]); 

		if (GetAsyncKeyState(keys[i]) == false) return false;
    }

	return true;
}

bool Keybind::GetChange() {
	return (lastState != isActive);
}

bool Keybind::GetChange(int dir) {
	return (lastState != isActive && lastState == dir);
}
