#pragma once
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Note {
public:
	time_t dateCreated;
	time_t lastModified;
	string header;
	string body;
	vector<string> tags;

	Note(string text);
	Note(string text, string header);
	void SetHeader();
	void SetHeader(string text);
	void Modifiy(string text);

	string GetCreationDisplayTime();
	string GetModificationDisplayTime();
};

