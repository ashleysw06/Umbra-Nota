#include <string>
#include <SFML/Graphics.hpp>
#include "container.h"

using namespace std;

#pragma once
class InputField {
public:
	string text; // What's displayed
	string value;
	string placeholder;

	// Style & Format
	float defaultPadding = 10;
	float textPadding = defaultPadding * 2;
	float textShadowOffset = 0.05;

	int textFontSize = 36;
	int fontSizePlaceholder = 24;
	int fontSizeMin = 18;

	unsigned int insertionLocation = 0; // Caret
	float blinkRate = 2; // Times per second

	InputField(string aPlaceholder);

	void HandleKey(char32_t unicode, bool pressed = true);
	void HandleInput(char32_t unicode);

	void Draw(sf::RenderWindow* window, Container* container);
};