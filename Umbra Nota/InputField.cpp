#include <iostream>
#include <SFML/Graphics.hpp>

#include "windows.h"
#include "sysinfoapi.h"
#include "InputField.h"

using namespace std;

InputField::InputField(string aPlaceholder) {
    placeholder = aPlaceholder;
}

void InputField::HandleKey(char32_t unicode, bool pressed /* true*/) {
    if (!pressed) return;
    cout << "Key Unicode: " << unicode << endl;
    switch (unicode)  {
    case 83: // Del
        if (insertionLocation < value.size() && value.size() > 0) {
            value.erase(insertionLocation, 1);
        }
        break;
    case 86: // Right
        if (insertionLocation < value.size()) insertionLocation += 1;
        break;
    case 87: // Left
        if (insertionLocation > 0) insertionLocation -= 1;
        break;
    case 88: // Down
        insertionLocation = value.size(); // TODO: Same position in following line
        break;
    case 89: // Up
        insertionLocation = 0; // TODO: Same position in previous line
        break;
    default:
        break;
    }
    //cout << preIValue << " | " << postIValue << endl
    //    << preWord << "-" << postWord << endl;
}

void InputField::HandleInput(char32_t unicode) {
	cout << "Input Unicode: " << unicode << " ( " << static_cast<char>(unicode) << " ) " << endl;
    string preIValue = value.substr(0, insertionLocation);
    string postIValue = value.substr(insertionLocation, value.size() - insertionLocation);
    string preWord = value.substr(preIValue.find_last_of(" ") + 1, insertionLocation - preIValue.find_last_of(" ") - 1);
    string postWord = value.substr(insertionLocation, postIValue.find_first_of(" "));
    switch (unicode) {
    case 0: // Del
        break;
    case 8: // Backspace
        if (insertionLocation > 0) {
            value.erase(insertionLocation - 1, 1);
            insertionLocation -= 1;
        }
        break;
    case 13: // Shift Enter
        value += "\n";
        insertionLocation += 1;
        break;
    case 127: // CTRL + Backspace
        if (insertionLocation > 0) {
            value.erase(insertionLocation - fmax(1, preWord.size()), fmax(1, preWord.size()));
            insertionLocation -= fmax(1, preWord.size());
        }
        break;
    default:
        char c = unicode;
        bool isValid = c != NULL;
        if (isValid) {
            value = value.substr(0, insertionLocation) + c + value.substr(insertionLocation, value.size());
            insertionLocation += 1;
        }
    }
}

void InputField::Draw(sf::RenderWindow* window, Container* container) {
    Font font;
    if (!font.openFromFile("Resources\\Font\\arial.ttf")) return;

    Color panelColor(40, 40, 60);
    Color textColor(200, 200, 200);
    Color placeholderTextColor(100, 100, 150, 128);

    bool valueSatisfied = (value != "");
    text = valueSatisfied ? value : placeholder;
    int fontSize = valueSatisfied ? textFontSize : fontSizePlaceholder;

    Color activeColor = valueSatisfied ? textColor : placeholderTextColor;
    Color shadowTextColor(panelColor.r / 2, panelColor.g / 2, panelColor.b / 2, panelColor.a / 2);

    Text shadow(font, InputField::text, fontSize);
    shadow.setStyle(sf::Text::Bold);

    Text text(font, InputField::text, fontSize);
    text.setStyle(sf::Text::Bold);

    // Handle text overflow

    float textLimitWidth = container->rect->size.x - (container->rect->GetLeft() + textPadding) * 2;
    float textContentWidth = text.getLocalBounds().size.x + textPadding * 2;

    if (textContentWidth > textLimitWidth) {
        fontSize *= textLimitWidth / textContentWidth;
        fontSize = fmax(fontSizeMin, fontSize);
        text.setCharacterSize(fontSize);
        shadow.setCharacterSize(fontSize);
    }

    Vector2f textPosition({ container->rect->GetLeft() + textPadding, container->rect->position.y - fontSize * 3 / 4 });

    shadow.setPosition({ textPosition.x + textShadowOffset * fontSize, textPosition.y + textShadowOffset * fontSize });
    shadow.setFillColor(shadowTextColor);

    text.setPosition({ textPosition.x, textPosition.y });
    text.setFillColor(activeColor);

    // Setup Up Caret
    Text tmp(font, value.substr(0, insertionLocation), fontSize);
    tmp.setStyle(sf::Text::Bold);

    RectangleShape caret;

    caret.setSize({ 2, 1.0f + fontSize });
    caret.setPosition({ container->rect->GetLeft() + textPadding + tmp.getLocalBounds().size.x + 1, textPosition.y });

    SYSTEMTIME st;
    GetSystemTime(&st);
    float s = st.wMilliseconds / 1000.0f;
    if (s * blinkRate / 2 - floor(s * blinkRate / 2) >= 0.5) {
        caret.setSize({ 0, 0 });
    }

    window->draw(shadow);
    window->draw(text);
    window->draw(caret);

}
