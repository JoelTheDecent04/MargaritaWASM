#include "Multiplayer.h"

#ifdef MULTIPLAYER

#include <emscripten/websocket.h>
#include <SDL.h>
#include "json.hpp"
#include "Space.h"
#include "Player.h"
#include "Snail.h"

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
        //printf("message: %s\n", websocketEvent->data);
        SpaceGame::Instance().GameClient().DataReceive((const char*)websocketEvent->data);
    }

    return EM_TRUE;
}

int sock = -1;

void AttemptConnection()
{
    if (!emscripten_websocket_is_supported())
        SDL_Log("Websockets not supported\n");

    EmscriptenWebSocketCreateAttributes attributes = {
        "ws://119.42.52.199:5252/", //URL 
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
{
    lPrevLevel = Game::lCurrentLevel;

    gui->Add(std::make_shared<WidgetLabel>("Username:", Alignment::LeftCentre, 36), 490, 100);
    gui->Add(textbox = std::make_shared<WidgetTextbox>(300, Alignment::TopLeft, true, 36), 490, 150);
    gui->Add(std::make_shared<WidgetButton>(100, 40, [&] { this->Login();}, "Login"), 490, 200);
}
void LoginScreen::update(float deltatime)
{
    if (connecting && connected) {
        SpaceGame::Instance().GameClient().SetSocket(sock);
        Game::LoadLevel(lPrevLevel);
    }
}
void LoginScreen::LeftClick()
{
	gui->ClickOn(Mouse::X, Mouse::Y);
}
void LoginScreen::KeyDown(int key)
{
    if (key == SDL_SCANCODE_ESCAPE)
		Game::LoadLevel(lPrevLevel);
    else
        gui->KeyDown(key);
}
void LoginScreen::KeyPress(const char* text)
{
    if (first_input) { //TODO fix
        first_input = false;
        return;
    }
    gui->TextInput(text);
}
void LoginScreen::Login()
{
    if (!connected)
        AttemptConnection();
    
    SpaceGame::Instance().GetPlayer().Name = textbox->Text();

    connecting = true;
}

WidgetEnemyInformation::WidgetEnemyInformation()
: WidgetGroup(100, 120)
{
    Add(std::make_shared<WidgetLabel>(name.c_str(), Alignment::TopLeft, 18), 0, 0);
    Add(std::make_shared<WidgetLabel>("HP", Alignment::TopLeft, 14), 0, 22);
    Add(std::make_shared<WidgetBar>(100, 20, clrGreen, &current_hp, &max_hp), 0, 40);
    Add(std::make_shared<WidgetLabel>("Energy", Alignment::TopLeft, 14), 0, 60);
    Add(std::make_shared<WidgetBar>(100, 20, clrGreen, &current_energy, &max_energy), 0, 80);
    Add(std::make_shared<WidgetLabel>(wave_number_str, Alignment::TopLeft, 18), 0, 100);
    wave_number_str[0] = 0;
}
void WidgetEnemyInformation::Refresh()
{
    snprintf(wave_number_str, sizeof(wave_number_str), "Wave %d", wave);
}

MultiplayerGameClient::MultiplayerGameClient(SpaceGame* game)
{
    widgetMultiplayerInfo = std::make_shared<WidgetGroup>(300, 200);
    game->Gui().Add(widgetMultiplayerInfo, 0, 100);
}

MultiplayerGameClient::~MultiplayerGameClient()
{
    if (websocket != -1) {
		emscripten_websocket_close(websocket, 0, "quit");
	}
}

void MultiplayerGameClient::Update(float deltatime)
{
    fSecondsSinceLastSend += deltatime;

    if (connected)
    {
        if (fSecondsSinceLastSend > 0.1f)
        {
            SendGameStatus();
            fSecondsSinceLastSend = 0.0f;
        }
    }
}

const char* MultiplayerGameClient::Status()
{
    if (connected)
        return "Connected";
    else
        return "Not Connected";
}

void MultiplayerGameClient::SendGameStatus()
{
    nlohmann::json status_json;
    auto& player = SpaceGame::Instance().GetPlayer();

    status_json["name"] = player.Name;
    status_json["energy"] = player.fEnergy;
    status_json["max_energy"] = player.fMaxEnergy;
    status_json["wave"] = SpaceGame::Instance().Wave();
    status_json["health"] = player.fHealth;
    status_json["max_health"] = player.fMaxHealth;
    status_json["snails_sent"] = num_snails_sent;
    num_snails_sent = 0;

    std::string status = status_json.dump();

    emscripten_websocket_send_binary(sock, (void*)status.c_str(), status.length());
}

void MultiplayerGameClient::DataReceive(const char* data)
{
    nlohmann::json server_data = nlohmann::json::parse(data);
    
    if (server_data.contains("players"))
    {
        //assert(0);
        size_t count = server_data["players"].size();

        auto& widgets = widgetMultiplayerInfo->Children();

        //printf("Count %d\n", 0);
        for (int i = 0; i < count; i++)
        {
            WidgetEnemyInformation* widget;
            if (i < widgets.size())
            {
                widget = (WidgetEnemyInformation*)widgets[i].widget.get();
            }
            else
            {
                auto new_widget = std::make_shared<WidgetEnemyInformation>();
                widgetMultiplayerInfo->Add(new_widget, 0, 50 * i);
                widget = new_widget.get();
            }

            auto player = server_data["players"].at(i);
            widget->current_hp = player["health"];
            widget->max_hp = player["max_health"];
            widget->current_energy = player["energy"];
            widget->max_energy = player["max_energy"];
            widget->wave = player["wave"];
            widget->name = player["name"];
            widget->Refresh();
            //printf("hp: %f", widget->current_hp);
        }
    }

    if (server_data.contains("snails_received") && (int)server_data["snails_received"] > 0) {
        SpaceGame::Instance().Entities.Add(new Snail());
        printf("SNAIL\n");
    }

    if (server_data.contains("winner"))
    {
        if (server_data["winner"].is_string())
        {

        }
    }
}
#endif