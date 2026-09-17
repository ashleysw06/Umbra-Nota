
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include "wtypes.h"
#include "TextPrompt.h"
#include "Container.h"

using namespace sf;
using namespace std;

TextPrompt::TextPrompt() {}

string TextPrompt::CreatePrompt() {
    return CreatePrompt("Type Note Here...");
}

string TextPrompt::CreatePrompt(string message) {
    Font font;
    if (!font.openFromFile("Resources\\Font\\arial.ttf")) {
        return "ERROR: FONT NOT FOUND"; // Temporary error handling
        return "";
    }

    string placeHolderText = message;

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);

    unsigned int windowWidth = 800;
    unsigned int windowHeight = 120;

    RenderWindow window;
    window.setPosition({ desktop.right / 2, desktop.bottom / 2 });
    window.create(VideoMode({ windowWidth, windowHeight }), "Umbra Nota", Style::None);

    // Colors
    Color backgroundColor(30, 30, 45);
    Color panelColor(40, 40, 60);
    Color noteColor(50, 50, 75);
    Color textColor(200, 200, 200);
    Color placeholderTextColor(100, 100, 150, 128);

    // Style & Format
    float defaultPadding = 10;
    float textPadding = defaultPadding * 2;
    float textShadowOffset = 0.05;

    int fontSize = 38;
    int fontSizeMin = fontSize / 2;

    UIRect promptRect(windowWidth / 2, windowHeight / 2, windowWidth - defaultPadding * 2, windowHeight - defaultPadding * 2);
    Container promptContainer(&promptRect);
    RectangleShape promptContainerShape = promptContainer.GetShape();
    promptContainerShape.setFillColor(noteColor);

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->scancode == Keyboard::Scancode::Escape) {
                    window.close();
                }

                if (keyPressed->scancode == Keyboard::Scancode::Enter) {
                    return content;
                    window.close();
                }
            }

            if (const auto* textEntered = event->getIf<Event::TextEntered>()) {
                cout << "Key Pressed: " << textEntered->unicode << " ( " << static_cast<char>(textEntered->unicode) << ") " <<endl;
                switch (textEntered->unicode) {
                case 8: // Backspace
                    if (content.size() > 0) content.erase(content.size() - 1, 1);
                    break;
                case 127: // CTRL + Backspace
                    content.erase(content.find_last_of(" ") + 1, content.size());
                    break;
                default:
                    content += static_cast<char>(textEntered->unicode);
                }
            }
        }

        window.clear(backgroundColor);

        bool isContentSatisfied = (content != "");
        string textContent = isContentSatisfied ? content : placeHolderText;
        int promptFontSize = isContentSatisfied ? fontSize : fontSize * 2 / 3;
        Color promptTextColor = isContentSatisfied ? textColor : placeholderTextColor;
        Color shladowTextColor(panelColor.r / 2, panelColor.g / 2, panelColor.b / 2, panelColor.a / 2);

        Text shadow(font, textContent, promptFontSize);
        Text text(font, textContent, promptFontSize);

        // Handle text overflow

        float textLimitWidth = promptContainer.rect->size.x - (promptContainer.rect->GetLeft() + textPadding) * 2;
        float textContentWidth = text.getGlobalBounds().size.x + textPadding * 2;

        if (textContentWidth > textLimitWidth) {
            promptFontSize *= textLimitWidth / textContentWidth;
            promptFontSize = fmax(fontSizeMin, promptFontSize);
            text.setCharacterSize(promptFontSize);
            shadow.setCharacterSize(promptFontSize);
        }

        Vector2f textPosition({ promptContainer.rect->GetLeft() + textPadding, static_cast<float>(windowHeight) / 2 - promptFontSize * 3 / 4 });

        shadow.setPosition({ textPosition.x + textShadowOffset * promptFontSize, textPosition.y + textShadowOffset * promptFontSize });
        shadow.setFillColor(shladowTextColor);
        shadow.setStyle(sf::Text::Bold);

        text.setPosition({ textPosition.x, textPosition.y });
        text.setFillColor(promptTextColor);
        text.setStyle(sf::Text::Bold);

        window.draw(promptContainerShape);
        window.draw(shadow);
        window.draw(text);

        window.display();
    }
    return "";
}
