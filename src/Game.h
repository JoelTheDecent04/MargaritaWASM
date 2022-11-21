#pragma once

#include <stdint.h>
#include <memory>
#include "GUI.h"

class Level
{
protected:
	std::shared_ptr<WidgetGroup> gui = std::make_shared<WidgetGroup>(1280, 720);
public:
	virtual void render() {};
	void Render()
	{
		render();
		gui->Draw(0, 0);
	}

	virtual void update(float deltatime) {};
	void Update(float deltatime)
	{
		update(deltatime);
		gui->Update(deltatime, Mouse::X, Mouse::Y);
	}

	virtual void LeftClick() {};
	virtual void KeyDown(int key) {};
	virtual void KeyPress(const char* keys) {};

	WidgetGroup& Gui() { return *gui; }
};

class SpaceGame;

namespace Game {
	void GameMain();
	void LoadLevel(const std::shared_ptr<Level>& lNewLevel);
	void LeftClick();
	void KeyDown(int key);
	void KeyPress(const char* keys);
	void Resize();
	void Quit();

	
	//extern std::shared_ptr<SpaceGame> sgSpaceGame;
	extern std::shared_ptr<Level> lCurrentLevel;

	extern const uint8_t* pKeyStates;
	extern int pKeyStatesLength;
}



extern int32_t nScreenWidth, nScreenHeight;
extern int32_t nWindowWidth, nWindowHeight;
extern float fWindowScale, fGUIScale;
extern int32_t nRenderTargetWidth, nRenderTargetHeight;
extern float fScaleH, fScaleV;

