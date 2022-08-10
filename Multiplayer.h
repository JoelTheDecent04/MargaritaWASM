//#define MULTIPLAYER

#ifdef MULTIPLAYER
#include "Game.h"
#include "GUI.h"
#include <string>

class WidgetEnemyInformation : public WidgetGroup
{
    float current_hp = 0;
    float max_hp = 200;
    float current_energy = 0;
    float max_energy = 150;
public:
    WidgetEnemyInformation();
};

class LoginScreen : public Level
{
    std::shared_ptr<Level> lPrevLevel;

    std::shared_ptr<WidgetTextbox> textbox;

    std::string login_request;
    bool connecting = false;

    bool first_input = true;

public:
    WidgetGroup gui;

    LoginScreen();
    void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
    void KeyDown(int key) override;
	void KeyPress(const char* text) override;

    void Login();
};

void SendGameStatus();

#endif