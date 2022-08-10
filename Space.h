#pragma once
#include "Game.h"
#include "Texture.h"
#include "BackgroundObject.h"
#include "EventHandler.h"
#include "GUI.h"

#include <vector>
#include <memory>

class Item;
class Entity;
class Player;

class SpaceGame : public Level
{	
	static std::shared_ptr<SpaceGame> game;
	WidgetGroup gui;

private:
	std::vector<std::shared_ptr<Entity>> vEntities;
	int nCurrentItem = 0;
	std::vector<std::shared_ptr<Item>> vItems;

	std::vector<std::shared_ptr<BackgroundObject>> vBackgroundObjects;

	std::shared_ptr<Player> plPlayer;

	//Multiplayer
	std::shared_ptr<WidgetGroup> widgetMultiplayerInfo;
	int websocket = -1;
	float fSecondsSinceLastSendReceive = 0.0f;

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

public:
	static void LoadResources();
	static void UnloadResources();

	void Render() override;
	void Update(float deltatime) override;
	void LeftClick() override;
	void KeyDown(int key) override;
	void NextWave();
	SpaceGame();
	~SpaceGame();

	#ifdef MULTIPLAYER
	void NetworkPacket(const EmscriptenWebSocketMessageEvent* event, void* data);
	#endif

	const std::vector<std::shared_ptr<Entity>>& Entities() { return vEntities; }
	void AddEntity(const std::shared_ptr<Entity>& entity) { vEntities.push_back(entity); }
	
	const std::vector<std::shared_ptr<Item>>& Items() { return vItems; }
	void AddItem(const std::shared_ptr<Item>& entity) { vItems.push_back(entity); }
	Item* GetItem(const char* name);

	const std::vector<std::shared_ptr<BackgroundObject>>& BackgroundObjects() { return vBackgroundObjects; }
	void AddBackgroundObject(const std::shared_ptr<BackgroundObject>& background_object) { vBackgroundObjects.push_back(background_object); }

	EventHandler& EventHandler() { return m_EventHandler; };

	std::shared_ptr<Player> Player() {
		return plPlayer;
	}

	float Difficulty() { return fDifficulty; }
	int& EnemyCount() { return nEnemies; }
	int& PlayerCount() { return nPlayers; }
	bool ShowingHitboxes() { return bShowHitboxes; }
	int Wave() { return nWave; }
	bool WaveFinished() { return bWaveFinished; }
	float WaveSecondsLeft() { return fSecondsUntilNextWave; }
	void SetConnectionWebsocket(int websocket) {
		this->websocket = websocket;
	}

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

extern Texture* textures[];
namespace TextureID
{
	enum TextureID
	{
		Character, Orb, Background, Laser, Laserbeam, Enemy, Bomb, Crab, Comet, None, BombAnimation, EnergyPowerup, RegenerationPowerup, Light, Foreground, Coin, EnemyLaser
	};
};
//extern Texture* tCharacterTexture, * tOrbTexture, * tBackground, * tLaserTexture, * tLaserBeamTexture, * tEnemyTexture, * tBombTexture, * tCrabTexture;
//extern Texture* tCometTexture, * tNoTexture, * tBombAnimationTexture, *tEnergyPowerupTexture, * tRegenerationPowerupTexture, * tLight;

static const float fPlayerAcceleration = 3000.0f;
static const float fPlayerDecceleration = 1500.0f;
static const float fPlayerMaxSpeed = 150.0f;
static const float fGravity = 250.0f;
static const float fPlayerMoveDownSpeed = 500.0f;
static const float fFriction = 100.0f;