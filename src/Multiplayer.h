#pragma once
#define MULTIPLAYER

#ifdef MULTIPLAYER
#include "Game.h"
#include "GUI.h"
#include <string>

class WidgetEnemyInformation : public WidgetGroup
{
    char wave_number_str[32];
public:
    std::string name;
    float current_hp = 0;
    float max_hp = 200;
    float current_energy = 0;
    float max_energy = 150;
    int wave = -1;
    WidgetEnemyInformation();
    void Refresh();
};

class LoginScreen : public Level
{
    std::shared_ptr<Level> lPrevLevel;

    std::shared_ptr<WidgetTextbox> textbox;

    std::string login_request;
    bool connecting = false;

    bool first_input = true;

public:
    LoginScreen();
	void update(float deltatime) override;
	void LeftClick() override;
    void KeyDown(int key) override;
	void KeyPress(const char* text) override;

    void Login();
};

class MultiplayerGameClient
{
    std::shared_ptr<WidgetGroup> widgetMultiplayerInfo;
	int websocket = -1;
    float fSecondsSinceLastSend = 0.0f;
    int num_snails_sent = 0;
public:
    MultiplayerGameClient(SpaceGame* game);
    ~MultiplayerGameClient();
    void Update(float deltatime);
    void SetSocket(int socket) { websocket = socket; }
    void SendGameStatus();
    void DataReceive(const char* data);
    void SendSnail() { num_snails_sent++; }
    const char* Status();
};

#endif