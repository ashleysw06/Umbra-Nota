#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "wtypes.h"
#include "TextPrompt.h"
#include "NotePad.h"
#include "Container.h"

// TODO:
// - NotePad Styles
NotePad::NotePad() {
}

void NotePad::Open() {
    Font font;
    if (!font.openFromFile("Resources\\Font\\arial.ttf")) {
        return;
    }

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);

    RenderWindow window;

    unsigned int windowWidth = 1280;
    unsigned int windowHeight = 720;

    window.setPosition({ desktop.right / 2, desktop.bottom / 2 });
    window.create(VideoMode({ windowWidth, windowHeight }), "Umbra Nota", Style::None);

    // Colors
    Color backgroundColor(30, 30, 45);
    Color panelColor(40, 40, 60);
    Color noteColor(50, 50, 75);
    Color textColor(200, 200, 200);

    // Style & Format
    float defaultPadding = 10;
    float notePadding = defaultPadding;
    float noteMargin = 5;
    float textPadding = defaultPadding;
    float bodyTextPadding = 5;

    int headerFontSize = 24;
    int bodyFontSize = 18;

    // Scrolling
    Vector2f scrollDistance = { 0, 0 };
    Vector2f scrollVelocity = { 0, 0 };
    float scrollVelocityLimit = 50;
    float scrollVelocityDrag = 0.9;

    float scrollSpeed = 7;
    bool limitScrollToContainerBounds = true;
    float scrollElasticity = 0.9;

    // Containers and (Mostly) Static UI
    Container body(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight);

    UIRect toolBarRect(windowWidth / 2, 15 + defaultPadding, windowWidth - defaultPadding * 2, 30);
    Container toolBar(&toolBarRect);
    RectangleShape toolBarShape = toolBar.GetShape();
    /*RectangleShape toolUnderlayMaskShape = toolBar.GetShape();
    toolUnderlayMaskShape.setPosition({ 0.0f, 0.0f });
    toolUnderlayMaskShape.setSize({ 0.0f + windowWidth, 30 + defaultPadding * 2 });
    toolUnderlayMaskShape.setFillColor(backgroundColor);*/
    toolBarShape.setFillColor(panelColor);

    UIRect noteContainerRect(windowWidth / 3 + defaultPadding, windowHeight / 2 + defaultPadding + 30, windowWidth * 2 / 3, windowHeight - 30 + defaultPadding * 4);
    Container noteContainer(&noteContainerRect);
    RectangleShape noteContainerShape = noteContainer.GetShape();
    noteContainerShape.setFillColor(panelColor);

    UIRect metadataContainerRect(windowWidth * 0.33, windowHeight + 30, defaultPadding, 0);
    Container metadataContainer(&metadataContainerRect);
    RectangleShape metadataContainerShape = metadataContainer.GetShape();
    metadataContainerShape.setFillColor(panelColor);

    // Notes
    float noteWidth = noteContainer.rect->width - notePadding * 2;
    float noteHeight = fmin(100, fmax(50, noteContainer.rect->height)); // Max height fmin[0], Min Height fmax[0]

    Vector2f groupedNoteSize = { noteWidth, noteHeight / 2 + notes.size() * (noteHeight + noteMargin) + notePadding};
    Vector2f overflowSize = { groupedNoteSize.x - noteContainer.rect->width, groupedNoteSize.y - noteContainer.rect->height };

    while (window.isOpen()) {
        scrollVelocity *= scrollVelocityDrag;
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->scancode == Keyboard::Scancode::Escape) {
                    window.close();
                }

                if (keyPressed->scancode == Keyboard::Scancode::Enter) { }
            }

            if (const auto* textEntered = event->getIf<Event::TextEntered>()) {
                if (textEntered->unicode < 128) {}
            }

            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                float lScrollVelocity = 0;
                switch (mouseWheelScrolled->wheel) {
                case sf::Mouse::Wheel::Vertical:
                    lScrollVelocity = scrollVelocity.y + scrollSpeed * mouseWheelScrolled->delta;
                    scrollVelocity.y = lScrollVelocity;
                    if (abs(lScrollVelocity) > scrollVelocityLimit) {
                        scrollVelocity.y = (signbit(lScrollVelocity) ? -1 : 1) * scrollVelocityLimit;
                    }
                    break;
                case sf::Mouse::Wheel::Horizontal:
                    lScrollVelocity = scrollVelocity.x + scrollSpeed * mouseWheelScrolled->delta;
                    scrollVelocity.x = lScrollVelocity;
                    if (abs(lScrollVelocity) > scrollVelocityLimit) {
                        scrollVelocity.x = (signbit(lScrollVelocity) ? -1 : 1) * scrollVelocityLimit;
                    }
                    break;
                }
                /*std::cout << "mouse x: " << mouseWheelScrolled->position.x << std::endl;
                std::cout << "mouse y: " << mouseWheelScrolled->position.y << std::endl;*/
            }

            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << mouseButtonPressed->position.x << std::endl;
                    std::cout << "mouse y: " << mouseButtonPressed->position.y << std::endl;
                }
            }
        }


        // Scroll Dist Calculation
        scrollDistance += scrollVelocity;
        Vector2f scrollOverflow(-scrollDistance.x - overflowSize.x, -scrollDistance.y - overflowSize.y);

        /*system("cls");
        std::cout << "Horizontal Axis: X, Width"
            << "    Scroll Distance: " << scrollDistance.x << endl
            << "    Base Note Width: " << noteWidth << endl
            << "    Grouped Note Width: " << groupedNoteSize.x << endl
            << "    Container Width: " << noteContainer.rect->width << endl
            << "    Note Overflow: " << overflowSize.x << endl
            << "    Scroll Overflow: " << scrollOverflow.x << endl << endl
            << "Vertical Axis: Y, Height" << endl
            << "    Scroll Distance: " << scrollDistance.y << endl
            << "    Base Note Height: " << noteHeight << endl
            << "    Grouped Note Height: " << groupedNoteSize.y << endl
            << "    Container Height: " << noteContainer.rect->height << endl
            << "    Note Overflow: " << overflowSize.y << endl
            << "    Scroll Overflow: " << scrollOverflow.y << endl;*/
           
        if (limitScrollToContainerBounds)  {
            if (scrollDistance.x > 0) scrollDistance.x *= scrollElasticity;
            if (scrollOverflow.x > 0 && overflowSize.x > 0) {
                scrollDistance.x = -overflowSize.x - scrollOverflow.x * scrollElasticity;
            } else if (overflowSize.x < 0) scrollDistance.x *= scrollElasticity;

            if (scrollDistance.y > 0) scrollDistance.y *= scrollElasticity;
            if (scrollOverflow.y > 0 && overflowSize.y > 0) {
                scrollDistance.y = -overflowSize.y - scrollOverflow.y * scrollElasticity;
            } else if (overflowSize.y < 0) scrollDistance.y *= scrollElasticity;
        }


        // Start Drawing
        window.clear(backgroundColor);

        window.draw(noteContainerShape);
        window.draw(metadataContainerShape);

        for (size_t i = 0; i < notes.size(); i++) {
            Note* note = &notes[i];

            RectangleShape noteBody({ noteWidth, noteHeight });
            UIRect noteRect((noteContainer.rect->x) + scrollDistance.x, (noteContainer.rect->GetTop() + noteHeight / 2 + i * (noteHeight + noteMargin) + notePadding) + scrollDistance.y, noteWidth, noteHeight);
            Container noteDataContainer(&noteRect);
            Vector2f bodyPosition = { noteDataContainer.rect->GetLeft(), noteDataContainer.rect->GetTop() };

            noteBody.setPosition(bodyPosition);
            noteBody.setFillColor(noteColor);

            Text headerText(font, (*note).header, headerFontSize);
            headerText.setFillColor(textColor);
            headerText.setStyle(sf::Text::Bold);
            headerText.setPosition({ noteDataContainer.rect->GetLeft() + textPadding, noteDataContainer.rect->GetTop() + textPadding });

            Text bodyText(font, (*note).body, bodyFontSize);
            bodyText.setFillColor(textColor);
            bodyText.setPosition({ noteDataContainer.rect->GetLeft() + textPadding + bodyTextPadding, noteDataContainer.rect->GetTop() + textPadding * 2 + headerFontSize });

            window.draw(noteBody);
            window.draw(bodyText);
            window.draw(headerText);
        }

        //window.draw(toolUnderlayMaskShape);
        window.draw(toolBarShape);

        window.display();
    }
}

void NotePad::Close() {
}

void NotePad::AddNote(Note note) {
	notes.push_back(note);
}

void NotePad::AddNote(string text) {
    Note lNote(text);
    notes.push_back(lNote);
}

void NotePad::AddNote(string header, string text) {
    Note note(header, text);
    notes.push_back(note);
}
