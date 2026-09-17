#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <chrono>
#include <Windows.h>
#include <time.h>
#include <fstream>

#include<json/json.h>

#include "WinKeyCodes.h"
#include "Keybind.h"
#include "TextPrompt.h"
#include "NotePad.h"

#include <spdlog/spdlog.h>

using namespace sf;
using namespace std;

int main() {
	cout << "Umbra Nota v a0.0.2" << endl;
	string motd = "MOTD";
	
	// Setup Keybinds

	char memoChars[] = { VK_CONTROL, VK_RETURN, NULL, NULL, NULL };
	Keybind memoBind = Keybind(memoChars);

	char viewChars[] = { VK_CONTROL, VK_SHIFT, KEY_U, NULL, NULL };
	Keybind viewBind = Keybind(viewChars);

	char quitChars[] = { VK_SHIFT, VK_MENU, KEY_Q, NULL, NULL };
	Keybind quitBind = Keybind(quitChars);

	// Get Random Quote or Message of the Day

	string url = "https://randomapi.dev/";

	Http::Request request;
	request.setMethod(sf::Http::Request::Method::Get);
	request.setUri("/api/quotes?maxLength=120&count=1");
	request.setHttpVersion(1, 1); // HTTP 1.1
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setBody("");

	Http http(url);
	Http::Response response = http.sendRequest(request);
	bool responseStatus = response.getStatus() == Http::Response::Status::Ok;

	// Debug Info
	cout << "Server Request:" << endl
	 << "   Status: " << responseStatus << endl
	 << "   HTTP version: " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << endl
	 << "   Content-Type header:" << response.getField("Content-Type") << endl;

	if (responseStatus) { // GOD FORGIVE ME FOR THE HORRORS I'M COMMITING
		// TODO: Create Custom JSON parsing tool
		const string rawJson = response.getBody();

		int textStartPos = 18;
		string textSplice = rawJson.substr(textStartPos);
		int textLength = textSplice.find("\"");

		int authorStartPos = 12;
		string authorSplice = textSplice.substr(textLength + authorStartPos);
		int authorLength = authorSplice.find("\"");

		motd = "\"" + textSplice.substr(0, textLength) + "\" - " + authorSplice.substr(0, authorLength);

	} else { std::cout << "request failed" << std::endl; }

	// Setup notepad
	NotePad notePad = NotePad();
	notePad.AddNote("Welcome to Umbra Nota!", "Version: Umbra Nota v0.0.2a"
		"\n# Getting Started!"
		"\nOpen the Notepad to view your notes with `CTRL` + `SHFT` + `U`"
		"\nOpen a new note with `CTRL` + `ENTER`"
		"\nEnd the program at anytime with `ALT` + `SHFT` + `Q`");

	// Load string from file
	string sJson;
	ifstream iNotesFile;
	iNotesFile.open("notes.json");

	string line;
	if (iNotesFile.is_open()) {
		while (getline(iNotesFile, line)) {
			sJson += line;
		}
		iNotesFile.close();
	}

	iNotesFile.close();

	// Load JSON from string
	Json::FastWriter fastWriter;
	Json::Value root;
	Json::Reader reader;
	reader.parse(sJson, root);


	// Load Notes from JSON
	if (root["notes"].type() == Json::nullValue) root["notes"] = Json::Value(Json::arrayValue);
	Json::Value jNotes = root["notes"];
	for (size_t i = 0; i < jNotes.size(); i++) {
		Json::Value jNote(jNotes.get(i, NULL));
		Note note(jNote["head"].asString(), jNote["body"].asString());

		Json::Value jMeta(jNote.get("metadata", NULL));
		if (jMeta != NULL) {
			note.dateCreated = jMeta.get("createdAt", 0).asInt();
			note.dateCreated = jMeta.get("lastModified", 0).asInt();

			Json::Value jTags = jMeta["tags"];
			for (size_t j = 0; j < jTags.size(); j++) {
				note.tags.push_back(jTags.get(j, "NULL").asString());
			}
		}
		
		notePad.AddNote(note);
	}

	
	notePad.Open();

	while (true) {
		memoBind.Update();
		viewBind.Update();
		quitBind.Update();

		if (memoBind.GetChange(0)) {
			cout << "New Text Prompt" << endl;
			string res = TextPrompt().CreatePrompt(motd);

			cout << "Response: " << res << endl;
			Note note(res);

			if (res.size() > 0) notePad.AddNote(note);
			Json::Value jNote;

			jNote["head"] = note.header;
			jNote["body"] = note.body;
			jNote["metadata"]["createdAt"] = note.dateCreated;
			jNote["metadata"]["lastModified"] = note.lastModified;
			jNote["metadata"]["tags"] = Json::Value(Json::arrayValue);

			Json::Value jTags = jNote["metadata"]["tags"];
			for (size_t j = 0; j < jTags.size(); j++) {
				jTags.append(note.tags.at(j));
			}

			cout << fastWriter.write(jNote) << endl;
			root["notes"].append(jNote);


			std::string output = fastWriter.write(root);

			ofstream notesFileOutput;
			notesFileOutput.open("notes.json");
			notesFileOutput << output;
			notesFileOutput.close();
		}

		if (viewBind.GetChange(0)) {
			cout << "Opening NotePad" << endl;
			notePad.Open();
		}

		if (quitBind.GetChange(1)) {
			break;
		}
	}

	cout << "Exiting Program..." << endl;
	return 0;
} 
