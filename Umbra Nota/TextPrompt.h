#include <string>

using namespace std;

#pragma once

class TextPrompt {
public:
	TextPrompt();
	string CreatePrompt();
	string CreatePrompt(string message);
};

