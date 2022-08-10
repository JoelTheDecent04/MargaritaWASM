#include "Multiplayer.h"

#ifdef MULTIPLAYER

#include <emscripten/websocket.h>
#include <SDL.h>
#include "json.hpp"
#include "Space.h"
#include "Player.h"

static bool connected = false;

EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData) {
    puts("onopen");

    connected = true;

    return EM_TRUE;
}
EM_BOOL onerror(int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData) {
    puts("onerror");
    connected = false;

    return EM_TRUE;
}
EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData) {
    puts("onclose");
    connected = false;

    return EM_TRUE;
}
EM_BOOL onmessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) {
    puts("onmessage");
    if (websocketEvent->isText) {
        // For only ascii chars.
        printf("message: %s\n", websocketEvent->data);
    }

    EMSCRIPTEN_RESULT result;
    result = emscripten_websocket_close(websocketEvent->socket, 1000, "no reason");
    if (result) {
        printf("Failed to emscripten_websocket_close(): %d\n", result);
    }
    return EM_TRUE;
}

int sock = -1;

void AttemptConnection()
{
    if (!emscripten_websocket_is_supported())
        SDL_Log("Websockets not supported\n");

    EmscriptenWebSocketCreateAttributes attributes = {
        "ws://localhost:5252/", //URL 
        nullptr, //Protocols
        EM_TRUE //Create on main thread
    };

    sock = emscripten_websocket_new(&attributes);

    emscripten_websocket_set_onopen_callback(sock, NULL, onopen);
    emscripten_websocket_set_onerror_callback(sock, NULL, onerror);
    emscripten_websocket_set_onclose_callback(sock, NULL, onclose);
    emscripten_websocket_set_onmessage_callback(sock, NULL, onmessage);

}

LoginScreen::LoginScreen()
: gui(1280, 720)
{
    lPrevLevel = Game::lCurrentLevel;

    gui.Add(std::make_shared<WidgetLabel>("Username:", Alignment::LeftCentre, 36), 490, 100);
    gui.Add(textbox = std::make_shared<WidgetTextbox>(300, Alignment::TopLeft, true, 36), 490, 150);
    gui.Add(std::make_shared<WidgetButton>(100, 40, [](void* p) {((LoginScreen*)p)->Login();}, "Login"), 490, 200);
}
void LoginScreen::Render()
{
    gui.Draw(0,0);
}
void LoginScreen::Update(float deltatime)
{
    int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.Update(deltatime, nCursorX, nCursorY);  

    if (connecting && connected) {
        SDL_Log("Sending: %s\n", login_request.c_str());
        emscripten_websocket_send_binary(sock, (void*)login_request.c_str(), login_request.length());
        SpaceGame::Instance().SetConnectionWebsocket(sock);
        //emscripten_websocket_send_utf8_text(sock, login_request.c_str());
        Game::LoadLevel(lPrevLevel);
    }
}
void LoginScreen::LeftClick()
{
    int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}
void LoginScreen::KeyDown(int key)
{
    if (key == SDL_SCANCODE_ESCAPE)
		Game::LoadLevel(lPrevLevel);
    else
        gui.KeyDown(key);
}
void LoginScreen::KeyPress(const char* text)
{
    if (first_input) { //TODO fix
        first_input = false;
        return;
    }
    gui.TextInput(text);
}
void LoginScreen::Login()
{
    if (!connected)
        AttemptConnection();
    
    nlohmann::json login_json;
    login_json["username"] = textbox->Text().c_str();
    login_request = login_json.dump();

    connecting = true;
}

WidgetEnemyInformation::WidgetEnemyInformation()
: WidgetGroup(100, 120)
{
    Add(std::make_shared<WidgetLabel>("Player2", Alignment::TopLeft, 18), 0, 0);
    Add(std::make_shared<WidgetLabel>("HP", Alignment::TopLeft, 14), 0, 22);
    Add(std::make_shared<WidgetBar>(100, 20, clrRed, &current_hp, &max_hp), 0, 40);
    Add(std::make_shared<WidgetLabel>("Energy", Alignment::TopLeft, 14), 0, 60);
    Add(std::make_shared<WidgetBar>(100, 20, clrRed, &current_energy, &max_energy), 0, 80);
    Add(std::make_shared<WidgetLabel>("Wave N", Alignment::TopLeft, 18), 0, 100);
}

void SendGameStatus()
{
    if (connected)
    {
        nlohmann::json status_json;
        auto& player = *SpaceGame::Instance().Player();

        status_json["hp"] = player.fHealth;
        status_json["max_hp"] = player.fMaxHealth;
        status_json["energy"] = player.nEnergy;
        status_json["max_energy"] = player.nMaxEnergy;

        status_json["wave"] = SpaceGame::Instance().Wave();

        std::string status = status_json.dump();

        emscripten_websocket_send_binary(sock, (void*)status.c_str(), status.length());
    }
}

#endif