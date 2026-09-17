
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include "wtypes.h"
#include "TextPrompt.h"
#include "Container.h"
#include "InputField.h"

using namespace sf;
using namespace std;

TextPrompt::TextPrompt() {}

string TextPrompt::CreatePrompt() {
    return CreatePrompt("Type Note Here...");
}

string TextPrompt::CreatePrompt(string message) {
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
    Color noteColor(50, 50, 75);

    float defaultPadding = 10;

    UIRect promptRect(windowWidth / 2, windowHeight / 2, windowWidth - defaultPadding * 2, windowHeight - defaultPadding * 2);
    Container promptContainer(&promptRect);
    RectangleShape promptContainerShape = promptContainer.GetShape();
    promptContainerShape.setFillColor(noteColor);

    InputField inputField(message);

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            bool shifting = false;

            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::LShift)) {
                shifting = true;
            }

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                inputField.HandleKey(static_cast<unsigned int>(keyPressed->scancode));
                if (keyPressed->scancode == Keyboard::Scancode::Escape) {
                    window.close();
                }

                if (keyPressed->scancode == Keyboard::Scancode::Enter && !shifting) {
                    return inputField.value;
                    window.close();
                }
            }

            if (const auto* keyReleased = event->getIf<Event::KeyReleased>()) {
                inputField.HandleKey(static_cast<unsigned int>(keyReleased->scancode), false);
            }

            if (const auto* textEntered = event->getIf<Event::TextEntered>()) {
                inputField.HandleInput(textEntered->unicode);
            }
        }

        window.clear(backgroundColor);

        window.draw(promptContainerShape);
        inputField.Draw(&window, &promptContainer);

        window.display();
    }
    return "";
}
