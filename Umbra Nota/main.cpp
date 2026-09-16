#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <chrono>
#include <Windows.h>
#include <time.h>

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
    notePad.AddNote("Umbra Nota v0.0.2a", "Welcome to Umbra Nota. Here are a few notes to get you started!");
    notePad.AddNote("Open Text Prompt", "CTRL + Enter");
    notePad.AddNote("Toggle Notepad View", "CTRL + SHFT + U");
    notePad.AddNote("Quit", "ALT + SHFT + Q");
    notePad.AddNote("Sample Note", "Sample Text");
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
        }

        if (viewBind.GetChange(0)) {
            cout << "Opening NotePad" << endl;
            notePad.Open();
        }

        if (quitBind.GetChange(1)) {
            cout << "Exiting Program..." << endl;
            return 0;
        }
    }
} 
