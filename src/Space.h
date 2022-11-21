#pragma once
#include "Game.h"
#include "Texture.h"
#include "BackgroundObject.h"
#include "EventHandler.h"
#include "GUI.h"
#include "Multiplayer.h"
#include "Utilities.h"

#include <vector>
#include <memory>

class Item;
class Entity;
class Player;

class SpaceGame : public Level
{	
	static std::shared_ptr<SpaceGame> game;

private:
	int nCurrentItem = 0;

	Player* plPlayer;

	float fFps;

	float fNextEnemySpawn = 0.0f;
	float fDifficulty = 80.0f;

	int nWave = 0;
	float fSecondsUntilNextWave = 0.0f;
	int nEnemies = 0;
	int nPlayers = 0;
	bool bWaveFinished = false;

	float fBackgroundPosition = 0.0f;
	bool bShowHitboxes = false;

	bool bShowMap = true;

	float fSecondsUntilNextComet;

	EventHandler m_EventHandler;

#ifdef MULTIPLAYER
	std::unique_ptr<MultiplayerGameClient> client;
#endif

public:
	static void LoadResources();
	static void UnloadResources();

	SpaceGame(SpaceGame&) = delete;
	SpaceGame& operator=(SpaceGame&) = delete;
	
	void render() override;
	void update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void NextWave();
	SpaceGame();
	~SpaceGame();

	#ifdef MULTIPLAYER
	MultiplayerGameClient& GameClient() { return *client; }
	#endif

	uint8_t Blocks[160][19];

	List<Entity> Entities;
	List<Item> Items;
	Item* GetItem(const char* name);

	List<BackgroundObject> BackgroundObjects;

	EventHandler& EventHandler() { return m_EventHandler; };

	Player& GetPlayer() {
		return *plPlayer;
	}

	float Difficulty() { return fDifficulty; }
	int& EnemyCount() { return nEnemies; }
	int& PlayerCount() { return nPlayers; }
	bool ShowingHitboxes() { return bShowHitboxes; }
	int Wave() { return nWave; }
	bool WaveFinished() { return bWaveFinished; }
	float WaveSecondsLeft() { return fSecondsUntilNextWave; }


public:
	static SpaceGame& Instance() {
		assert(game);
		return *game;
	}

	static bool InstanceExists() {
		return game.get();
	}

	static std::shared_ptr<SpaceGame> InstanceAsSharedPtr()
	{
		return game;
	}

	static void SetInstance(const std::shared_ptr<SpaceGame>& new_inst)
	{
		game = new_inst;
	}

	static float RenderPosition() { assert(game);  return game->fBackgroundPosition; }
};

//extern Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
//extern Texture* tCometTexture, * tNoTexture, * tBombAnimationTexture, *tEnergyPowerupTexture, * tRegenerationPowerupTexture, * tLight;

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;

