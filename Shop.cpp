#include "Shop.h"
#include "Game.h"
#include "Space.h"
#include "Player.h"
#include "EnergyPowerup.h"
#include "RegenerationPowerup.h"
#include "Laser.h"
#include "Bomb.h"
#include "Utilities.h"
#include "Colours.h"
#include "ControlManager.h"

using namespace Game;
using namespace Graphics;

class WidgetPlayerUpgrade : public WidgetButton
{
	std::unique_ptr<PlayerUpgrade> upgrade;
	ShopScreen* parent;

public:
	WidgetPlayerUpgrade(ShopScreen* parent, std::unique_ptr<PlayerUpgrade>&& upgrade)
		: WidgetButton(360, 40, nullptr, nullptr)
	{
		this->parent = parent;
		this->upgrade = std::move(upgrade);
		
	}

	void Update(float deltatime, int mouse_x, int mouse_y) override
	{
		SetClickable(SpaceGame::Instance().Player()->fMoney > upgrade->Cost());

		WidgetButton::Update(deltatime, mouse_x, mouse_y);
	}

	void ClickOn(int, int, void*) override
	{
		if (this->IsClickable()) {
			assert(SpaceGame::Instance().Player()->fMoney >= upgrade->Cost());
			SpaceGame::Instance().Player()->fMoney -= upgrade->Cost();
			upgrade->DoUpgrade();
			parent->ReloadUpgrades();
		}
	}

	void Draw(int x, int y) override
	{
		WidgetButton::Draw(x, y);
		
		//Draw description
		auto text = TextBuf() << '+' << upgrade->Increment() << ' ' << upgrade->Description() << " ($" << upgrade->Cost() << ')';
		TextRenderer().SetPosition(x + width / 2, y + height / 2 - 7).SetAlignment(Alignment::Centre).SetText(text);

		//Draw visual representation
		float proportion = upgrade->Current() / upgrade->Max();
		RectRenderer().SetFilled().SetPosition(x + 80, y + 25).SetColour(clrCustomDarkBlue).SetSize(200 * proportion, 12).Draw();
		RectRenderer().SetPosition(x + 80, y + 25).SetSize(200, 12).SetColour(clrDarkGrey).Draw();

		TextRenderer().SetPosition(x + width / 2, y + 31).SetAlignment(Alignment::Centre).SetSize(12).SetText(TextBuf() << upgrade->Current() << " / " << upgrade->Max());
	}
};

class WidgetItemUpgrade : public WidgetButton
{
	std::unique_ptr<ItemUpgrade> upgrade;
	std::string description;

	ShopScreen* parent;
public:
	WidgetItemUpgrade(ShopScreen* parent, std::unique_ptr<ItemUpgrade>&& upgrade)
		: WidgetButton(190, 40, nullptr, nullptr)
	{
		this->parent = parent;

		auto descbuf = TextBuf() << upgrade->Description() << " ($" << upgrade->Cost() << ')';
		description = descbuf.str();
		SetText(description.c_str());
	}

	void Update(float, int, int) override {
		SetClickable(SpaceGame::Instance().Player()->fMoney > upgrade->Cost());
	}

	void ClickOn(int, int, void*) override {
		SpaceGame::Instance().Player()->fMoney -= upgrade->Cost();
		upgrade->DoUpgrade();

		parent->ReloadUpgrades();
	}
};

ShopScreen::ShopScreen()
	: gui(1280, 720)
{
	player_upgrades = std::make_shared<WidgetGroup>(360, 190);
	item_upgrades = std::make_shared<WidgetGroup>(360, 190);

	ReloadUpgrades();
	gui.Add(player_upgrades, 130, 150);

	gui.Add(std::make_shared<WidgetButton>(240, 80, [](void* s) { ((ShopScreen*)s)->Resume(); }, "Back", 24), 520, 600);

	button_energy_powerup = (gui.Add(std::make_shared<WidgetButton>(190, 40, [](void* s) { ((ShopScreen*)s)->BuyEnergyPowerup(); }, "Energy Powerup ($200)",16), 610, 150));
	button_regen_powerup = (gui.Add(std::make_shared<WidgetButton>(190, 40, [](void* s) { ((ShopScreen*)s)->BuyRegenerationPowerup(); }, "Regeneration Powerup ($250)", 14), 610, 200));

	button_laser = (gui.Add(std::make_shared<WidgetButton>(190, 40, [](void* s) { ((ShopScreen*)s)->BuyLaserUpgrade(); }, "Laser ($400)", 16), 130, 430));
	button_bomb = (gui.Add(std::make_shared<WidgetButton>(190, 40, [](void* s) { ((ShopScreen*)s)->BuyBombUpgrade(); }, "Bomb ($500)", 16), 130, 480));
	
}

void ShopScreen::Render()
{
	Clear(clrBlack);
	gui.Draw(0, 0);
	WriteText("Upgrades", fScaleH * 130, fScaleV * 115, Graphics::GetFont(20));
	DrawRectangle(fScaleH * 70.0f, fScaleV * 100.0f, fScaleH * 480.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);

	WriteText("Items", fScaleH * 610, fScaleV * 115, Graphics::GetFont(20));
	DrawRectangle(fScaleH * 550.0f, fScaleV * 100.0f, fScaleH * 350.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);

	//Player image
	DrawRectangle(fScaleH * 900.0f, fScaleV * 100.0f, fScaleH * 1.43f * 216.0f, fScaleV * 280.0f, clrCustomDarkBlue, 1.0f, 3.0f);

	WriteText("Weapon Upgrades", fScaleH * 130.0f, fScaleV * 395.0f, Graphics::GetFont(20));
	DrawRectangle(fScaleH * 70.0f, fScaleV * 380.0f, fScaleH * 350.0f, fScaleV * 180.0f, clrCustomDarkBlue, 1.0f, 3.0f);

	WriteText("Defences", fScaleH * 480.0f, fScaleV * 395.0f, Graphics::GetFont(20));
	DrawRectangle(fScaleH * 420.0f, fScaleV * 380.0f, fScaleH * 350.0f, fScaleV * 180.0f, clrCustomDarkBlue, 1.0f, 3.0f);
	textures[TextureID::Character]->DrawDifferentSize(1, 900.0f + 8, 140.0f, 1.43f * 200.0f, 200.0f);

	TextSize textsize;

	char txtBuf[128];

	snprintf(txtBuf, sizeof(txtBuf), "$%d", (int)SpaceGame::Instance().Player()->fMoney); //Money (corner)
	TextMetrics(txtBuf, Graphics::GetFont(16), textsize);
	WriteText(txtBuf, fScaleH * (1280 - 5) - textsize.width, 5, Graphics::GetFont(16));
}

void ShopScreen::Update(float deltatime)
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.Update(deltatime, nCursorX, nCursorY);

	button_energy_powerup->SetClickable(SpaceGame::Instance().Player()->fMoney >= 200.0f);
	button_regen_powerup->SetClickable(SpaceGame::Instance().Player()->fMoney >= 250.0f);
	button_laser->SetClickable(SpaceGame::Instance().Player()->fMoney >= 400.0f && (((LaserWeapon*)SpaceGame::Instance().Items()[0].get())->nLaserLevel != LaserWeapon::LaserLevel::DoubleShot));
	button_bomb->SetClickable(SpaceGame::Instance().Player()->fMoney >= 500.0f);
}

void ShopScreen::LeftClick()
{
	int nCursorX, nCursorY;
	GetRelativeMousePos(&nCursorX, &nCursorY);
	gui.ClickOn(nCursorX, nCursorY, this);
}

void ShopScreen::Resume()
{
	LoadLevel(SpaceGame::InstanceAsSharedPtr());
}

void ShopScreen::ReloadUpgrades()
{
	player_upgrades->Clear();
	item_upgrades->Clear();

	int y = 0;
	auto possible_player_upgrades = SpaceGame::Instance().Player()->GetPossibleUpgrades();
	for (auto& upgrade : possible_player_upgrades) {
		player_upgrades->Add(std::make_shared<WidgetPlayerUpgrade>(this, std::move(upgrade)), 0, y);
		y += 50;
	}

	y = 0;
	std::vector<std::unique_ptr<ItemUpgrade>> possible_item_upgrades;

	for (const auto& item : SpaceGame::Instance().Items()) {
		auto item_upgrades = item->GetPossibleUpgrades();
		possible_item_upgrades.insert(
			possible_item_upgrades.end(),
			std::make_move_iterator(item_upgrades.begin()),
			std::make_move_iterator(item_upgrades.end()));

		possible_item_upgrades.erase(possible_item_upgrades.begin(), possible_item_upgrades.end());
		//possible_item_upgrades.insert(std::end(possible_item_upgrades), std::begin(item_upgrades), std::end(item_upgrades));
	}
}

void ShopScreen::UpgradeHealth()
{
	if (SpaceGame::Instance().Player()->fMoney >= 150.0f && SpaceGame::Instance().Player()->fMaxHealth < SpaceGame::Instance().Player()->fMaxHealthUpgrade)
	{
		SpaceGame::Instance().Player()->fMoney -= 150.0f;
		SpaceGame::Instance().Player()->fMaxHealth += 50.0f;
	}
}

void ShopScreen::UpgradeEnergyRecharge()
{
	if (SpaceGame::Instance().Player()->fMoney >= 150.0f && SpaceGame::Instance().Player()->fEnergyRechargeSpeed < SpaceGame::Instance().Player()->fMaxEnergyRechargeSpeed)
	{
		SpaceGame::Instance().Player()->fMoney -= 150.0f;
		SpaceGame::Instance().Player()->fEnergyRechargeSpeed += 1.0f;
	}
}

void ShopScreen::UpgradeMovementSpeed()
{
	if (SpaceGame::Instance().Player()->fMoney >= 50.0f && SpaceGame::Instance().Player()->fMovementSpeed < SpaceGame::Instance().Player()->fMaxMovementSpeed)
	{
		SpaceGame::Instance().Player()->fMoney -= 50.0f;
		SpaceGame::Instance().Player()->fMovementSpeed += 10.0f;
	}
}

void ShopScreen::BuyEnergyPowerup()
{
	if (SpaceGame::Instance().Player()->fMoney >= 200.0f)
	{
		SpaceGame::Instance().Player()->fMoney -= 200.0f;
		SpaceGame::Instance().AddItem(std::make_shared<EnergyPowerupItem>(SpaceGame::Instance().Player()));
	}
}

void ShopScreen::BuyRegenerationPowerup()
{
	if (SpaceGame::Instance().Player()->fMoney >= 250.0f)
	{
		SpaceGame::Instance().Player()->fMoney -= 250.0f;
		SpaceGame::Instance().AddItem(std::make_shared<RegenerationPowerupItem>(SpaceGame::Instance().Player()));
	}
}

void ShopScreen::BuyLaserUpgrade()
{
	if (SpaceGame::Instance().Player()->fMoney >= 400.0f && ((LaserWeapon*)SpaceGame::Instance().Items()[0].get())->nLaserLevel != LaserWeapon::LaserLevel::DoubleShot)
	{
		SpaceGame::Instance().Player()->fMoney -= 400.0f;
		((LaserWeapon*)SpaceGame::Instance().GetItem("Laser"))->SetLevel(LaserWeapon::LaserLevel::DoubleShot);
	}
}

void ShopScreen::BuyBombUpgrade()
{
	if (SpaceGame::Instance().Player()->fMoney >= 500.0f)
	{
		SpaceGame::Instance().Player()->fMoney -= 500.0f;
		//int nOldLevel = ((BombWeapon*)SpaceGame::Instance().Items()[2].get())->nLevel;
		((BombWeapon*)SpaceGame::Instance().GetItem("Bomb"))->Upgrade();
	}
}

void ShopScreen::UpgradeRegeneration()
{
	if (SpaceGame::Instance().Player()->fMoney >= 100.0f && SpaceGame::Instance().Player()->fHealthRegeneration < SpaceGame::Instance().Player()->fMaxHealthRegeneration)
	{
		SpaceGame::Instance().Player()->fMoney -= 100.0f;
		SpaceGame::Instance().Player()->fHealthRegeneration += 1.0f;
	}
}

void ShopScreen::KeyDown(int key)
{
	const auto& ctrl = ControlManager::Instance().GetControlFromAction(Control::Action::OpenShop);
	if (key == ctrl.Primary() || key == ctrl.Secondary() || key == SDL_SCANCODE_ESCAPE)
	{
		Resume();
	}
}