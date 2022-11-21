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
#include "Snail.h"

#include "Multiplayer.h"
#ifdef MULTIPLAYER
#include "json.hpp"
#include <emscripten/websocket.h>
#endif

using namespace Game;

std::shared_ptr<SpaceGame> SpaceGame::game = nullptr;

SpaceGame::SpaceGame() 
{
	memset(Blocks, 0, sizeof(Blocks));

	plPlayer = new Player(384.0f, 380.f);

	Entities.Add(plPlayer);
	nPlayers++;

	Items.Add(new LaserWeapon(plPlayer, LaserWeapon::LaserLevel::Normal));
	Items.Add(new OrbWeapon(plPlayer));
	//Items.push_back(std::make_shared<AirStrikeItem>(plPlayer));
	//Items.push_back(std::make_shared<BlockItem>(plPlayer));
	//Items.push_back(std::make_shared<TurretPlacer>(plPlayer));

	fSecondsUntilNextComet = 40 + randomf() * 40;
	NextWave();

	client = std::make_unique<MultiplayerGameClient>(this);
	//widgetMultiplayerInfo = std::make_shared<WidgetGroup>(300, 200);
	//gui.Add(widgetMultiplayerInfo, 0, 200);
};

SpaceGame::~SpaceGame()
{

	#ifdef MULTIPLAYER
	
	#endif
}
void SpaceGame::LoadResources()
{
}
void SpaceGame::render()
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
	SDL_RenderCopyF(Graphics::pRenderer, Textures::Background->Handle(), &rSrc, &rDest);

	//-------------------------------------------------------------

	//Draw entities in reverse order
	for (int i = Entities.size() - 1; i >= 0; i--)
		Entities[i].Draw();

	//Draw scenery / background objects
	for (auto& backgroundobject : BackgroundObjects)
		backgroundobject.Draw();

	//Tell current item to render if applicable
	Items[nCurrentItem].HoldRender();

	//Floor layer
	rSrc = { (int)(fBackgroundScale * fBackgroundPosition), (int)(594.0f * fBackgroundScale), (int)(fBackgroundScale * 1280.0f), (int)((720 - 594) * fBackgroundScale) };
	rDest = { 0, (float)594.0f, (float)nScreenWidth, (float)(720.0f - 594.0f) };
	SDL_RenderCopyF(Graphics::pRenderer, Textures::Background->Handle(), &rSrc, &rDest);

	//Draw foreground
	Textures::Foreground->Draw(0, -fBackgroundPosition - 65.0f, 475.0f);

	//Draw health and health bar
	TextRenderer().SetPosition(5, 2).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf() << "HP " << (int)plPlayer->fHealth << " / " << (int)plPlayer->fMaxHealth);

	Graphics::FillRectangle(5, 2 + 14 + 4, 100.0f * (plPlayer->fHealth / plPlayer->fMaxHealth), 20, clrRed); //18
	Graphics::DrawRectangle(5, 2 + 14 + 4, 100, 20, clrDarkGrey);

	//Draw energy and energy bar
	TextRenderer().SetPosition(5, 40).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf() << "Energy " << (int)plPlayer->fEnergy << " / " << (int)plPlayer->fMaxEnergy);

	Graphics::FillRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100.0f * (plPlayer->fEnergy / plPlayer->fMaxEnergy), 20, clrBlue); //56
	Graphics::DrawRectangle(5, 2 + 14 + 2 + 20 + 2 + 14 + 5, 100, 20, clrDarkGrey);

	//Draw name of current item
	TextSize textsize;
	Graphics::TextMetrics(Items[nCurrentItem].strName, Graphics::GetFont(14, Graphics::Absolute), textsize);
	Graphics::FillRectangle(0, (float)(nScreenHeight - 54), std::max((int)(32 * Items.size()), textsize.width) + 10.0f, 14 + 4 + 32 + 4, clrBlack);
	TextRenderer().SetPosition(5, nScreenHeight - 4 - 32 - 4 - 14).SetAbsoluteCoordinates().SetSize(14).SetText(TextBuf(Items[nCurrentItem].strName));

	//Draw items and draw number if applicable
	int nItem = 0;
	for (auto& item : Items)
	{
		item.texture->Draw(item.nTextureFrame, 4 + nItem * 32, nScreenHeight - 4 - 32, true);
		Graphics::DrawRectangle(4.0f + nItem * 32.0f, (float)nScreenHeight - 4.0f - 32.0f, (float)32, (float)32, nItem == nCurrentItem ? clrWhite : clrDarkGrey);
		if (item.nCount >=0)
			TextRenderer().SetAbsoluteCoordinates().SetPosition(6 + nItem * 32, nScreenHeight - 4).SetSize(14).SetAlignment(Alignment::BottomLeft).SetText(std::stringstream() << item.nCount);
		nItem++;
	}

	//Draw money
	TextRenderer().SetSize(24).SetPosition(1275, 5).SetAlignment(Alignment::TopRight).SetText(TextBuf() << "$" << (int)SpaceGame::Instance().GetPlayer().fMoney);
	//TextRenderer().SetAbsoluteCoordinates().SetPosition( nScreenWidth - 5, 2 ).SetAlignment( Alignment::TopRight ).SetText( std::stringstream() << "$" << (int)plPlayer->fMoney );

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
		Textures::Background->DrawDifferentSize(0, 640 - 300, 50, 600, 85);
		RectRenderer().SetPosition(640 - 300, 50).SetSize(600, 85).Draw();
		for (auto& entity : Entities) {
			if (entity.IsEnemy()) {
				RectRenderer().SetPosition(640 - 300 + 600.f * (entity.fX / 5120), 50.0f + 85.f * (entity.fY / 720.0f)).SetSize(5.f, 5.f).SetFilled().SetAlignment(Alignment::Centre).SetColour(clrRed).Draw();
			}
			else if (entity.GetType() == Entity::Type::Player) {
				RectRenderer().SetPosition(640 - 300 + 600.f * (entity.fX / 5120), 50.0f + 85.f * (entity.fY / 720.0f)).SetSize(5.f, 5.f).SetFilled().SetAlignment(Alignment::Centre).SetColour(clrGreen).Draw();
			}
		}
	}

	//Draw connection status
	#ifdef MULTIPLAYER
	TextRenderer().SetPosition(640.0f, 5.0f).SetAlignment(Alignment::TopCentre).SetText((TextBuf)client->Status());
	#endif
}
void SpaceGame::UnloadResources()
{

}
void SpaceGame::update(float deltatime)
{
	//Entity updates
	for (size_t i = 0; i < Entities.size(); i++)
	{
		auto& entity = Entities[i];
		
		if (entity.Update(deltatime) == Entity::Status::REMOVE)
		{
			if (entity.GetType() != Entity::Type::Player) //Do not delete player
				Entities.RemoveAt(i--);
		}
	}

	// If there are no players left the game has ended. Hence, go to death screen
	if (nPlayers == 0)
	{
		LoadLevel(std::make_shared<DeathScreen>());
		return;
	}

	//Background / Scenery updates
	for (size_t i = 0; i < BackgroundObjects.size(); i++)
	{
		auto& object = BackgroundObjects[i];
		//Remove if 'Update' returns false
		if (!object.Update(deltatime))
			BackgroundObjects.RemoveAt(i--);
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
				Entities.Add(enemy);
				nEnemies++;
			}
			else
				delete enemy;
		}
		else
		{
			auto* crab = new Crab(random_off_screen());
			if (crab->bLegalPosition)
			{
				Entities.Add(crab);
				nEnemies++;
			}
			else
				delete crab;
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
		BackgroundObjects.Add(new Comet());
		fSecondsUntilNextComet = 40 + randomf() * 40;
	}

	//Update item that is being held
	Items[nCurrentItem].HoldUpdate();

	//Event handler to handle scheduled events
	m_EventHandler.Update(deltatime);

	//Multiplayer
	#ifdef MULTIPLAYER
	client->Update(deltatime);
	#endif
}
void SpaceGame::LeftClick()
{
	//Get cursor position and angle relative to player
	float fAngle = atan2(Mouse::Y - plPlayer->fY, Mouse::X + fBackgroundPosition - plPlayer->fX);
	
	//Use item
	//Delete item if asked to
	if (Items[nCurrentItem].Use(plPlayer->fX, plPlayer->fY, fAngle) == false)
	{
		Items.RemoveAt(nCurrentItem);
		if (nCurrentItem >= (int)Items.size())
			nCurrentItem = Items.size() - 1;
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
	default:
		break;
	}

	if (action >= Control::Action::Slot1 && action <= Control::Action::Slot8)
	{
		int index = (int)action - (int)Control::Action::Slot1;
		if (Items.size() > index)
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
	else if (key == SDL_SCANCODE_H)
		bShowHitboxes = !bShowHitboxes;
	else if (key == SDL_SCANCODE_O) {
		SDL_RenderSetLogicalSize(Graphics::pRenderer, 1280, 720);
		fScaleH = 1.0f;
		fScaleV = 1.0f;
		nScreenWidth = 1280;
		nScreenHeight = 720;
	}
	else if (key == SDL_SCANCODE_U) {
		Entities.Add(new Snail());
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
		client->SendGameStatus();
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

	Entities.Add(new CoinPickup(randomf() * 5120.f, randomf() * 400.0f));
}

//Find player item
Item* SpaceGame::GetItem(const char* name) {
	for (Item& item : Items) {
		if (strcmp(item.Name(), name) == 0)
			return &item;
	}

	return nullptr;
}

#ifdef MULTIPLAYER
/*void SpaceGame::NetworkPacket(const EmscriptenWebSocketMessageEvent* event, void* data)
{
	
}*/
#endif