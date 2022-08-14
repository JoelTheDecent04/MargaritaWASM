#include "Space.h"
#include "Player.h"
#include "Orb.h"
#include "Utilities.h"
#include "Laser.h"
#include "EntityHealthChangeText.h"
#include "Enemy.h"
#include "DeathScreen.h"
#include "Bomb.h"
#include "PauseScreen.h"
#include "Shop.h"
#include "Crab.h"
#include "Comet.h"
#include "EnergyPowerup.h"
#include "RegenerationPowerup.h"
#include "ControlsScreen.h"
#include "Graphics.h"
#include "Colours.h"
#include "Turret.h"
#include "ControlManager.h"
#include <math.h>
#include <random>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "Block.h"
#include "AirStrike.h"
#include "Coin.h"

#include "Multiplayer.h"
#ifdef MULTIPLAYER
#include "json.hpp"
#include <emscripten/websocket.h>
#endif

using namespace Game;


Texture* textures[18];

std::shared_ptr<SpaceGame> SpaceGame::game = nullptr;

SpaceGame::SpaceGame() 
: gui(1280, 720)
{
	plPlayer = new Player(384.0f, 380.f);

	vEntities.push_back(plPlayer);
	nPlayers++;

	vItems.push_back(std::make_shared<LaserWeapon>(plPlayer, LaserWeapon::LaserLevel::Normal));
	vItems.push_back(std::make_shared<OrbWeapon>(plPlayer));
	//vItems.push_back(std::make_shared<AirStrikeItem>(plPlayer));
	//vItems.push_back(std::make_shared<BlockItem>(plPlayer));
	//vItems.push_back(std::make_shared<TurretPlacer>(plPlayer));

	fSecondsUntilNextComet = 40 + randomf() * 40;
	NextWave();

	widgetMultiplayerInfo = std::make_shared<WidgetGroup>(300, 200);
	gui.Add(widgetMultiplayerInfo, 0, 200);
	#ifdef MULTIPLAYER

	widgetMultiplayerInfo->Add(std::make_shared<WidgetEnemyInformation>(), 0, 0);
	#endif
};

SpaceGame::~SpaceGame()
{
	#ifdef MULTIPLAYER
	if (websocket != -1) {
		emscripten_websocket_close(websocket, 0, "quit");
	}
	#endif
}
void SpaceGame::LoadResources()
{
	textures[TextureID::Character] =		new Texture("player.png", 186, 116, 83, 58);
	textures[TextureID::Orb] =				new Texture("orb.png", 2497, 2497, 32, 32);
	textures[TextureID::Background] =		new Texture("background.png", 8192, 1152, 5120, 720);
	textures[TextureID::Laser] =			new Texture("Laser.png", 32, 32, 32.0f, 32.0f);
	textures[TextureID::Laserbeam] =		new Texture("LaserBeam.png", 2569, 765, 32, 10);
	textures[TextureID::Enemy] =			new Texture("enemy.png", 77, 110, 38, 55);
	textures[TextureID::Crab] =				new Texture("crab.png", 700, 350, 96, 48);
	textures[TextureID::Bomb] =				new Texture("bomb.png", 2218, 2223, 32.0f, 32.0f);
	textures[TextureID::Comet] =			new Texture("comet.png", 640, 360, 100, 50);
	textures[TextureID::None] =				new Texture("notexture.png", 32, 32, 32, 32);
	textures[TextureID::BombAnimation] =	new Texture("bomb_animation.png", 1280, 720, 100.0f, 100.0f);
	textures[TextureID::EnergyPowerup] =	new Texture("energy_powerup.png", 2600, 2600, 32.0f, 32.0f);
	textures[TextureID::RegenerationPowerup] = new Texture("regen_powerup.png", 2415, 2415, 32.0f, 32.0f);
	textures[TextureID::Foreground] =		new Texture("foreground.png", 8192, 204, 5120, 127.33f);
	textures[TextureID::Coin] =				new Texture("coin.png", 96, 96, 32.f, 32.f);
	textures[TextureID::EnemyLaser] = 		new Texture("enemy_laser.png", 100, 48, 48.f, 24.f);
	textures[TextureID::Block] = 			new Texture("blocks.png", 32, 32, 32.f, 32.f);
}
void SpaceGame::Render()
{
	fBackgroundPosition = plPlayer->fX - (1280 / 2);
	if (plPlayer->fX < (1280 / 2))
		fBackgroundPosition = 0;
	if (plPlayer->fX > 5120 - (1280 / 2))
		fBackgroundPosition = 5120 - 1280;

	//Draw background
	//textures[TextureID::Background]->DrawPanorama(fBackgroundPosition);

	float fBackgroundScale = 1.6f;
	
	//Moon layer
	SDL_Rect rSrc = { (int)(fBackgroundScale * fBackgroundPosition / 2.0f), 0, (int)(fBackgroundScale * 1280.0f), (int)(fBackgroundScale * 594.0f)};
	SDL_FRect rDest{ 0, 0, (float)nScreenWidth, (float)594.0f };
	SDL_RenderCopyF(Graphics::pRenderer, textures[TextureID::Background]->Handle(), &rSrc, &rDest);

	//Floor layer
	rSrc = { (int)(fBackgroundScale * fBackgroundPosition), (int)(594.0f * fBackgroundScale), (int)(fBackgroundScale * 1280.0f), (int)((720 - 594) * fBackgroundScale) };
	rDest = { 0, (float)594.0f, (float)nScreenWidth, (float)(720.0f - 594.0f) };
	SDL_RenderCopyF(Graphics::pRenderer, textures[TextureID::Background]->Handle(), &rSrc, &rDest);

	//-------------------------------------------------------------

	//Draw entities in reverse order
	for (int i = vEntities.size() - 1; i >= 0; i--)
		vEntities[i]->Draw();

	//Tell current item to render if applicable
	vItems[nCurrentItem]->HoldRender();

	//Draw foreground
	textures[TextureID::Foreground]->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);

	//Draw scenery / background objects
	for (auto& backgroundobject : vBackgroundObjects)
		backgroundobject->Draw();

	//Draw health and health bar
	TextRenderer().SetPosition(5, 2).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf() << "HP " << (int)plPlayer->fHealth << " / " << (int)plPlayer->fMaxHealth);
	//GraphicsStreamText(5, 2, 14) << "HP " << (int)plPlayer->fHealth << " / " << (int)plPlayer->fMaxHealth << EndStream();

	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (plPlayer->fHealth / plPlayer->fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	//Draw energy and energy bar
	TextRenderer().SetPosition(5, 40).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf() << "Energy " << (int)plPlayer->nEnergy << " / " << (int)plPlayer->nMaxEnergy);
	//GraphicsStreamText(5, 40, 14) << "Energy " << (int)plPlayer->nEnergy << " / " << (int)plPlayer->nMaxEnergy << EndStream();

	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (plPlayer->nEnergy / plPlayer->nMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	//Draw name of current item
	
	TextSize textsize;
	Graphics::TextMetrics(vItems[nCurrentItem]->strName, Graphics::GetFont(14, Graphics::Absolute), textsize);
	Graphics::FillRectangle(0, (float)(nScreenHeight - 54), std::max((int)(32 * vItems.size()), textsize.width) + 10.0f, 14 + 4 + 32 + 4, clrBlack);
	TextRenderer().SetPosition(5, nScreenHeight - 4 - 32 - 4 - 14).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf(vItems[nCurrentItem]->strName));
	//Graphics::WriteText(vItems[nCurrentItem]->strName, 5, nScreenHeight - 4 - 32 - 4 - 14, Graphics::GetFont(14, Graphics::Absolute));

	//Draw items and draw number if applicable
	int nItem = 0;
	for (auto& item : vItems)
	{
		textures[item->nTexture]->Draw(item->nTextureFrame, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4.0f + nItem * 32.0f, (float)nScreenHeight - 4.0f - 32.0f, (float)32, (float)32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item->nCount >=0)
			TextRenderer().SetAbsoluteCoordinates().SetPosition(6 + nItem * 32, nScreenHeight - 4).SetSize(14).SetAlignment(Alignment::BottomLeft).SetText(std::stringstream() << item->nCount);
		nItem++;
	}

	//Draw money
	TextRenderer().SetAbsoluteCoordinates().SetPosition( nScreenWidth - 5, 2 ).SetAlignment( Alignment::TopRight ).SetText( std::stringstream() << "$" << (int)plPlayer->fMoney );

	//Draw time remaining
	auto time_left = GraphicsStreamText(nScreenWidth - 5, nScreenHeight, 16, clrWhite, Alignment::BottomRight) << "Wave " << nWave << " (" << (int)fSecondsUntilNextWave << " seconds left)";
	GraphicsStreamRect(nScreenWidth - 3, nScreenHeight, time_left.Size() + RectSize(4, 0), clrBlack, Alignment::BottomRight);
	time_left << EndStream();

	//If wave has finished tell user how to continue
	if (bWaveFinished)
	{
		const char* key_text = ControlManager::Instance().GetControlFromAction(Control::Action::NextWave).PrimaryKeyText();
		TextRenderer().SetAbsoluteCoordinates().SetPosition(nScreenWidth / 2, 8.0f).SetSize(32).SetAlignment(Alignment::TopCentre).SetText(
			TextBuf() << "Wave " << nWave << " completed. Press \'" << key_text << "\' To Continue. "
		);
	}

	//Draw current objective
	TextRenderer().SetAbsoluteCoordinates().SetPosition( nScreenWidth / 2.0f, nScreenHeight ).SetSize( 24 ).SetAlignment( Alignment::BottomCentre ).SetText( TextBuf(m_EventHandler.Objective().AsText()) );

	//Draw map
	if (bShowMap)
	{
		textures[TextureID::Background]->DrawDifferentSize(0, 640 - 300, 50, 600, 85);
		RectRenderer().SetPosition(640 - 300, 50).SetSize(600, 85).Draw();
		for (auto& entity : vEntities) {
			if (entity->IsEnemy()) {
				RectRenderer().SetPosition(640 - 300 + 600.f * (entity->fX / 5120), 50.0f + 85.f * (entity->fY / 720.0f)).SetSize(5.f, 5.f).SetFilled().SetAlignment(Alignment::Centre).SetColour(clrRed).Draw();
			}
			else if (entity->GetType() == Entity::Type::Player) {
				RectRenderer().SetPosition(640 - 300 + 600.f * (entity->fX / 5120), 50.0f + 85.f * (entity->fY / 720.0f)).SetSize(5.f, 5.f).SetFilled().SetAlignment(Alignment::Centre).SetColour(clrGreen).Draw();
			}
		}
	}

	gui.Draw(0, 0);
}
void SpaceGame::UnloadResources()
{

}
void SpaceGame::Update(float deltatime)
{
	//Entity updates
	for (size_t i = 0; i < vEntities.size(); i++)
	{
		auto* entity = vEntities[i];
		
		//Remove entity if 'Update' returns false
		if (entity->Update(deltatime) == false) {
			delete entity;
			vEntities.erase(vEntities.begin() + (i--));
		}
	}

	// If there are no players left the game has ended. Hence, go to death screen
	if (nPlayers == 0)
	{
		LoadLevel(std::make_shared<DeathScreen>());
		return;
	}

	//Background / Scenery updates
	for (size_t i = 0; i < vBackgroundObjects.size(); i++)
	{
		auto& object = *vBackgroundObjects[i];
		//Remove if 'Update' returns false
		if (!object.Update(deltatime))
			vBackgroundObjects.erase(vBackgroundObjects.begin() + (i--));
	}

	//Enemy spawning
	fNextEnemySpawn -= deltatime;

	//Only spawn enemies periodically and if wave hasn't finished yet
	if (fNextEnemySpawn <= 0 && fSecondsUntilNextWave > 0.0f)
	{
		//15% chance of crab, else normal enemy
		if (randomf() >= 0.15f)
		{
			auto* enemy = new Enemy(random_off_screen(), randomf() * 500);
			if (enemy->bLegalPosition)
			{
				vEntities.push_back(enemy);
				nEnemies++;
			}
		}
		else
		{
			auto* crab = new Crab(random_off_screen());
			if (crab->bLegalPosition)
			{
				vEntities.push_back(crab);
				nEnemies++;
			}
		}

		fNextEnemySpawn = (randomf() * 10.0f) / (fDifficulty / 60.0f);
	}

	//Waves
	fSecondsUntilNextWave -= deltatime;

	if (fSecondsUntilNextWave <= 0.0f) {
		fSecondsUntilNextWave = 0.0f;
		if (nEnemies == 0)
			bWaveFinished = true;
	}

	//Comet that periodically flies across the screen
	fSecondsUntilNextComet -= deltatime;
	if (fSecondsUntilNextComet < 0.0f)
	{
		vBackgroundObjects.push_back(std::make_shared<Comet>());
		fSecondsUntilNextComet = 40 + randomf() * 40;
	}

	//Update item that is being held
	vItems[nCurrentItem]->HoldUpdate();

	//Event handler to handle scheduled events
	m_EventHandler.Update(deltatime);

	//Multiplayer
	#ifdef MULTIPLAYER
	fSecondsSinceLastSendReceive += deltatime;

	if (fSecondsSinceLastSend < 0.1f) {
		nlohmann::json status;
		status["game_running"] = true;
		status["wave"] = nWave;
		status["health"] = plPlayer->fHealth;
		status["max_health"] = plPlayer->fMaxHealth;
		status["money"] = plPlayer->fMoney;

		auto str = status.dump();

		emscripten_websocket_send_binary(websocket, (void*)str.c_str(), str.length());
	
		fSecondsSinceLastSendReceive = 0.0f;
	}
	#endif
}
void SpaceGame::LeftClick()
{
	//Get cursor position and angle relative to player
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	
	nCursorX += (int)fBackgroundPosition;

	float fGradient = (nCursorY - plPlayer->fY) / (nCursorX - plPlayer->fX);
	float fAngle = atan(fGradient);
	if (nCursorX < plPlayer->fX) fAngle += 3.1415926f;
	
	//Use item
	//Delete item if asked to
	if (vItems[nCurrentItem]->Use(plPlayer->fX, plPlayer->fY, fAngle) == false)
	{
		vItems.erase(vItems.begin() + nCurrentItem);
		if (nCurrentItem >= (int)vItems.size())
			nCurrentItem = vItems.size() - 1;
	}

}

void SpaceGame::KeyDown(int key)
{
	Control::Action action = ControlManager::Instance().GetControl(key).GetAction();

	switch (action)
	{
	case Control::Action::OpenShop:
		LoadLevel(std::static_pointer_cast<Level>(std::make_shared<ShopScreen>()));
		break;
	case Control::Action::NextWave:
		if (bWaveFinished)
			NextWave();
		break;
	case Control::Action::ShowHideMap:
		bShowMap = !bShowMap;
		break;
	}

	if (action >= Control::Action::Slot1 && action <= Control::Action::Slot8)
	{
		int index = (int)action - (int)Control::Action::Slot1;
		if (vItems.size() > index)
			nCurrentItem = index;
	}

	//Pause
	if (key == SDL_SCANCODE_ESCAPE)
		LoadLevel(std::static_pointer_cast<Level>(std::make_shared<PauseScreen>()));

	//Debugging
	else if (key == SDL_SCANCODE_K)
		plPlayer->ChangeHealth(-20.0f, nullptr);
	else if (key == SDL_SCANCODE_J)
		plPlayer->ChangeHealth(20.0f, nullptr);
	//else if (key == SDL_SCANCODE_N)
		//plPlayer->fMoney += 200.0f;
	else if (key == SDL_SCANCODE_H)
		bShowHitboxes = !bShowHitboxes;
	else if (key == SDL_SCANCODE_O) {
		SDL_RenderSetLogicalSize(Graphics::pRenderer, 1280, 720);
		fScaleH = 1.0f;
		fScaleV = 1.0f;
		nScreenWidth = 1280;
		nScreenHeight = 720;
	}
	else if (key == SDL_SCANCODE_P) {
		SDL_RenderSetLogicalSize(Graphics::pRenderer, 1600, 900);
		fScaleH = 1600.0f / 1280.0f;
		fScaleV = 900.0f / 720.0f;
		nScreenWidth = 1600;
		nScreenHeight = 900;
	}
	else if (key == SDL_SCANCODE_U) {
		AddItem(std::make_shared<BlockItem>(plPlayer));
		AddItem(std::make_shared<TurretPlacer>(plPlayer));
	}
	else if (key == SDL_SCANCODE_N) {
		plPlayer->fMoney += 200.0f;
	}
	else if (key == SDL_SCANCODE_C) {
		#ifdef MULTIPLAYER
	//	AttemptConnection("0.0.0.0:80");
		Game::LoadLevel(std::make_shared<LoginScreen>());
		#endif
	}
	else if (key == SDL_SCANCODE_X) {
		#ifdef MULTIPLAYER
		SendGameStatus();
		#endif
	}
		
}

void SpaceGame::NextWave()
{
	//Increment wave
	nWave++;
	fSecondsUntilNextWave = 60.0f;
	fDifficulty *= 1.5f;

	//Increase maximum upgrades
	plPlayer->fMaxHealthUpgrade += 50.0f;
	plPlayer->fMaxEnergyRechargeSpeed += 1.0f;
	plPlayer->fMaxMovementSpeed += 10.0f; 
	plPlayer->fMaxHealthRegeneration += 1.0f;

	bWaveFinished = false;

	vEntities.push_back(new CoinPickup(randomf() * 5120.f, randomf() * 400.0f));
}

//Find player item
Item* SpaceGame::GetItem(const char* name) {
	for (const auto& item : vItems) {
		if (strcmp(item->Name(), name) == 0)
			return item.get();
	}

	return nullptr;
}

#ifdef MULTIPLAYER
void SpaceGame::NetworkPacket(const EmscriptenWebSocketMessageEvent* event, void* data)
{
	
}
#endif