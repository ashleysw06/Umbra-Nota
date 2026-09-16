#include <Windows.h>

#pragma once
class Keybind {
public:
	Keybind( char aKeys[] );
	char keys[5] = { NULL, NULL, NULL, NULL, NULL };
	void Update();

	bool GetActive();

	bool GetChange();
	bool GetChange(int dir);

	bool isActive = false;
	bool lastState = false;
};

