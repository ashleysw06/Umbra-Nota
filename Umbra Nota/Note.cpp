#include <iostream>
#include <ctime>

#include "Note.h"

using namespace std;

Note::Note(string text) : Note(text, text) { }

Note::Note(string header, string text) {
    body = text;
    SetHeader(header);

    dateCreated = time(NULL);
    lastModified = time(NULL);
}

void Note::SetHeader() {
    header = body.substr(0, fmin(20, body.size()));
}

void Note::SetHeader(string text) {
    header = text.substr(0, fmin(20, text.size()));
}

void Note::Modifiy(string text) {
    body = text;
    SetHeader();
}

string Note::GetCreationDisplayTime() {
    struct tm datetime = *localtime(&lastModified);

    return asctime(&datetime);
}

string Note::GetModificationDisplayTime() {
    struct tm datetime = *localtime(&lastModified);

    return asctime(&datetime);
}
