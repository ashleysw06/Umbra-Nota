#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "wtypes.h"
#include "TextPrompt.h"
#include "Container.h" // UI.h or something
#include "ScrollView.h" // UI.h or something
#include "StandardCursor.hpp"

#include "NotePad.h"

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

	StandardCursor normalCursor(StandardCursor::NORMAL);
	StandardCursor handCursor(StandardCursor::HAND);

	// Colors
	Color backgroundColor(30, 30, 45);
	Color panelColor(40, 40, 60);
	Color noteColor(50, 50, 75);
	Color noteHoveredColor(70, 70, 105);
	Color textColor(200, 200, 200);

	// Style & Format
	float defaultPadding = 10;
	float notePadding = defaultPadding;
	float noteMargin = 5;
	float textPadding = defaultPadding;
	float bodyTextPadding = 5;

	int headerFontSize = 24;
	int bodyFontSize = 18;
	int metadataFontSize = 12;

	// Note Viewing
	ScrollView* focusedScrollView{};
	Note* hoveredNote{};
	Note* selectedNote{};

	// Containers
	Container body(windowWidth / 2, windowHeight / 2, windowWidth, windowHeight);

	UIRect toolBarRect(windowWidth / 2, 15 + defaultPadding, windowWidth - defaultPadding * 2, 30);
	Container toolBar(&toolBarRect);
	RectangleShape toolBarShape = toolBar.GetShape();
	toolBarShape.setFillColor(panelColor);

	UIRect noteContainerRect(
		windowWidth / 3 + defaultPadding / 3,
		windowHeight / 2 + (toolBarRect.size.y + defaultPadding * 2) / 2 - (defaultPadding / 2),
		windowWidth * 2 / 3 - defaultPadding * 3 / 2,
		windowHeight - (toolBarRect.size.y + defaultPadding * 2) - (defaultPadding)
	);
	ScrollView noteContainer(&noteContainerRect, ScrollView::VERTICAL);
	RectangleShape noteContainerShape = noteContainer.GetShape();
	noteContainerShape.setFillColor(panelColor);

	UIRect dataContainerRect(
		windowWidth * 5 / 6 + (defaultPadding / 6) - (defaultPadding / 2),
		windowHeight / 2 + (toolBarRect.size.y + defaultPadding * 2) / 2 - (defaultPadding / 2),
		windowWidth / 3 - (defaultPadding * 2 / 5) - (defaultPadding),
		windowHeight - (toolBarRect.size.y + defaultPadding * 2) - (defaultPadding)
	);
	Container dataContainer(&dataContainerRect);
	RectangleShape dataContainerShape = dataContainer.GetShape();
	dataContainerShape.setFillColor(panelColor);

	// Notes
	vector<Container*> cNotes;
	float noteWidth = noteContainer.rect->size.x - notePadding * 2;
	float noteHeight = fmin(100, fmax(50, noteContainer.rect->size.y)); // Max height fmin[0], Min Height fmax[0]
	cout << "Note Base Size: W - " << noteWidth << " H - " << noteHeight;
	for (size_t i = 0; i < notes.size(); i++) {
		UIRect cNoteRect(0, 0, noteWidth, noteHeight);
		Container cNote(&cNoteRect);
		cNotes.push_back(&cNote);
		noteContainer.AddChild(&cNote);
	}

	while (window.isOpen()) {
		noteContainer.ApplyDrag();
		if (noteContainer.rect->Contains(Mouse::getPosition(window))) focusedScrollView = &noteContainer;
		else focusedScrollView = nullptr;
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
				switch (mouseWheelScrolled->wheel) {
				case sf::Mouse::Wheel::Horizontal:
					if (focusedScrollView != NULL) focusedScrollView->AddScrollVelocityX(mouseWheelScrolled->delta);
					break;
				case sf::Mouse::Wheel::Vertical:
					if (focusedScrollView != NULL) focusedScrollView->AddScrollVelocityY(mouseWheelScrolled->delta);
					break;
				}
			}

			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
					if (hoveredNote != NULL) selectedNote = hoveredNote;
				}
				if (mouseButtonPressed->button == sf::Mouse::Button::Middle) {
					std::cout << "the middle button was pressed" << std::endl;
				}
				if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
					std::cout << "the right button was pressed" << std::endl;
				}
			}
		}

		// Scroll Dist Calculation
		noteContainer.ApplyVelocity();
		hoveredNote = nullptr;

		// Start Drawing
		normalCursor.set(window.getNativeHandle());
		window.clear(backgroundColor);

		window.draw(noteContainerShape);
		window.draw(dataContainerShape);
		window.draw(toolBarShape);

		for (size_t i = 0; i < notes.size(); i++) {
			Note* note = &notes[i];
			UIRect* pTextRect = cNotes[i]->rect;
			Container* pTextContainer = cNotes[i];

			pTextRect->position.x = noteContainer.rect->position.x + noteContainer.scrollDistance.x;
			pTextRect->position.y = (noteContainer.rect->GetTop() + noteHeight / 2 + i * (noteHeight + noteMargin) + notePadding) + noteContainer.scrollDistance.y;

			bool isHovered = pTextRect->Contains(Mouse::getPosition(window));
			if (isHovered) {
				hoveredNote = note;
				handCursor.set(window.getNativeHandle());
			}

			RectangleShape noteBody({ noteWidth, noteHeight });
			noteBody.setPosition({ pTextRect->GetLeft(), pTextRect->GetTop() });
			noteBody.setFillColor(isHovered ? noteHoveredColor : noteColor);

			Text headerText(font, note->header, headerFontSize);
			headerText.setFillColor(textColor);
			headerText.setStyle(sf::Text::Bold);
			headerText.setPosition({ pTextRect->GetLeft() + textPadding, pTextRect->GetTop() + textPadding });

			Text bodyText(font, note->body, bodyFontSize);
			bodyText.setFillColor(textColor);
			bodyText.setPosition({ pTextRect->GetLeft() + textPadding + bodyTextPadding, pTextRect->GetTop() + textPadding * 2 + headerFontSize });

			window.draw(noteBody);
			window.draw(bodyText);
			window.draw(headerText);
		}

		if (selectedNote != NULL) {

			// Header
			
			Text headerText(font, selectedNote->header, headerFontSize);
			headerText.setFillColor(textColor);
			headerText.setStyle(sf::Text::Bold);
			headerText.setPosition({ dataContainer.rect->GetLeft() + textPadding, dataContainer.rect->GetTop() + textPadding });

			window.draw(headerText);

			// Body
			// Text Wrap

			string delimiter = "\n";
			string s = selectedNote->body + delimiter;
			float textBounds = dataContainer.rect->size.x - (textPadding + bodyTextPadding);

			std::vector<std::string> lines;
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) { // Split by \n
				token = s.substr(0, pos);
				Text tmp(font, token, bodyFontSize);
				while (tmp.getLocalBounds().size.x > textBounds) {
					int availableSize = token.size();
					while (tmp.getLocalBounds().size.x > textBounds) { // Split overflow by blank space
						tmp.setString(token.substr(0, availableSize));
						int tempASize = token.substr(0, availableSize - 1).find_last_of(" ");
						if (tempASize != std::string::npos) {
							availableSize = tempASize;
						} else { // If no blank space: Split between last character within bounds
							availableSize--;
						}
					}
					lines.push_back(token.substr(0, availableSize));
					token.erase(0, availableSize + 1);
					tmp.setString(token);
				}
				lines.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			lines.push_back(s);

			// Draw lines

			for (size_t i = 0; i < lines.size(); i++) {
				Text bodyText(font, lines[i], bodyFontSize);
				bodyText.setFillColor(textColor);
				bodyText.setPosition({ dataContainer.rect->GetLeft() + textPadding + bodyTextPadding, dataContainer.rect->GetTop() + textPadding * 2 + headerFontSize + i * (bodyFontSize + 2 /* Line Spacing */) });

				window.draw(bodyText);
			}

			// Footer / Metadata

			string metadataText = "Created at: " + selectedNote->GetCreationDisplayTime() + 
				"Last Modified: " + selectedNote->GetModificationDisplayTime() +
				"Tags: ";
			for (size_t i = 0; i < selectedNote->tags.size(); i++) {
				string tag = selectedNote->tags[i];
				metadataText += "#" + tag + " ";
			}
			Text metadata(font, metadataText, bodyFontSize);
			metadata.setFillColor(textColor);
			metadata.setPosition({ dataContainer.rect->GetLeft() + textPadding + bodyTextPadding, dataContainer.rect->GetBottom() - metadata.getLocalBounds().size.y - textPadding });

			window.draw(metadata);
		}

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
