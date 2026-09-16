#include <vector>
#include "Note.h"
#include <SFML/Graphics.hpp>

#pragma once
class NotePad {
public:
	vector<Note> notes;

	NotePad();

	void Open();
	void Close();

	void AddNote(Note note);
	void AddNote(string note);
	void AddNote(string header, string text);
};

