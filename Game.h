#pragma once

#include <stdint.h>
#include <memory>

class Level
{
public:
	virtual void Render() = 0;
	virtual void Update(float deltatime) = 0;
	virtual void LeftClick() {};
	virtual void KeyDown(int key) {};
	virtual void KeyPress(const char* keys) {};
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

