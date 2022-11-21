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
#include "Colours.h"
#include "Block.h"

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
		SetClickable(SpaceGame::Instance().GetPlayer().fMoney > upgrade->Cost());

		WidgetButton::Update(deltatime, mouse_x, mouse_y);
	}

	void ClickOn(int, int) override
	{
		if (this->IsClickable()) {
			assert(SpaceGame::Instance().GetPlayer().fMoney >= upgrade->Cost());
			SpaceGame::Instance().GetPlayer().fMoney -= upgrade->Cost();
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
	int price;
	std::string description;
	Texture* texture;

	ShopScreen* parent;
public:
	WidgetItemUpgrade(ShopScreen* parent, std::unique_ptr<ItemUpgrade>&& _upgrade)
		: WidgetButton(130, 200, nullptr , "")
	{
		this->parent = parent;
		upgrade = std::move(_upgrade);
		ItemUpgrade* upgrade_ptr = upgrade.get();

		texture = upgrade->texture;
		description = upgrade->Description();
		price = upgrade->Cost();
		
		function = [parent, upgrade_ptr] { 
			SpaceGame::Instance().GetPlayer().fMoney -= upgrade_ptr->Cost();
			upgrade_ptr->DoUpgrade();
			parent->ReloadUpgrades();
		};
	}

	WidgetItemUpgrade(ShopScreen* parent, const std::string& description, Texture* texture, int price, std::function<void()> function)
		: WidgetButton(130, 200, function, "")
	{
		this->parent = parent;
		this->price = price;
		this->texture = texture;
		this->description = description;
	}

	void Update(float, int, int) override {
		SetClickable(SpaceGame::Instance().GetPlayer().fMoney >= price);
	}

	void Draw(int x, int y) override
	{
		Graphics::FillRectangle(x, y, width, height, clickable ? (hovering ? clrDarkGrey : clrBlack) : clrDarkerGrey);
		TextRenderer().SetPosition(x + width / 2, y).SetAlignment(Alignment::TopCentre).SetText((TextBuf)description);
		texture->DrawDifferentSize(upgrade->nTextureFrame, x + 5, y + 30, width - 10, width - 10);
		TextRenderer().SetPosition(x + width / 2, y + height - 4).SetAlignment(Alignment::BottomCentre).SetText((TextBuf)std::to_string(price));
		//TODO make it look better
	}

};

ShopScreen::ShopScreen()
{
	widget_item_list = std::make_shared<WidgetGroup>(600, 420);
	widget_player_upgrades = std::make_shared<WidgetGroup>(360, 500);

	ReloadUpgrades();

	//Back Button
	gui->Add(std::make_shared<WidgetButton>(240, 80, [&] { this->Resume(); }, "Back", 24), 520, 600);	

	//Item Upgrades
	gui->Add(std::make_shared<WidgetLabel>("Item Upgrades", Alignment::Centre, 36), 400, 60);
	gui->Add(widget_item_list, 100, 100);

	//Player Upgrades
	gui->Add(std::make_shared<WidgetLabel>("Player Upgrades", Alignment::Centre, 36), 1000, 60);
	gui->Add(widget_player_upgrades, 1000 - 180, 100);
}

void ShopScreen::render()
{
	Textures::TitleScreenBackground->Draw();

	TextRenderer().SetSize(24).SetPosition(1275, 5).SetAlignment(Alignment::TopRight).SetText(TextBuf() << "$" << (int)SpaceGame::Instance().GetPlayer().fMoney);
}

void ShopScreen::LeftClick()
{
	gui->ClickOn(Mouse::X, Mouse::Y);
}

void ShopScreen::Resume()
{
	LoadLevel(SpaceGame::InstanceAsSharedPtr());
}

void ShopScreen::ReloadUpgrades()
{
	widget_item_list->Clear();
	widget_player_upgrades->Clear();

	//Item Upgrades
	int count = 0;
	for (size_t i = 0; i < SpaceGame::Instance().Items.size(); i++)
	{
		auto item_upgrades = SpaceGame::Instance().Items[i].GetPossibleUpgrades();
		while (item_upgrades.size() > 0)
		{
			widget_item_list->Add(std::make_shared<WidgetItemUpgrade>(this, item_upgrades.Take(0)), (count % 4) * 150, (count / 4) * 220);
			count++;
		}
	}

	//Powerups
	//Energy Powerup
	widget_item_list->Add(std::make_shared<WidgetItemUpgrade>(this, "Energy Powerup", Textures::EnergyPowerup, 250, [] { 
		SpaceGame::Instance().GetPlayer().fMoney -= 250;
		SpaceGame::Instance().Items.Add(new EnergyPowerupItem(&SpaceGame::Instance().GetPlayer()));
	}), (count % 4) * 150, (count / 4) * 220);
	count++;

	//Regeneration Powerup
	widget_item_list->Add(std::make_shared<WidgetItemUpgrade>(this, "Regeneration Powerup", Textures::RegenerationPowerup, 250, [] { 
		SpaceGame::Instance().GetPlayer().fMoney -= 250;
		SpaceGame::Instance().Items.Add(new RegenerationPowerupItem(&SpaceGame::Instance().GetPlayer()));
	}), (count % 4) * 150, (count / 4) * 220);
	count++;

	//Block
	widget_item_list->Add(std::make_shared<WidgetItemUpgrade>(this, "Blocks (Testing)", Textures::Block, 250, [] { 
		SpaceGame::Instance().GetPlayer().fMoney -= 250;
		BlockItem* item = new BlockItem(&SpaceGame::Instance().GetPlayer());
		item->nCount = 10;
		SpaceGame::Instance().Items.Add(item); 
	}), (count % 4) * 150, (count / 4) * 220);
	count++;

	//Multiplayer
	#ifdef MULTIPLAYER
	widget_item_list->Add(std::make_shared<WidgetItemUpgrade>(this, "Send Snail", Textures::Snail, 600, [] { 
		SpaceGame::Instance().GetPlayer().fMoney -= 600;
		SpaceGame::Instance().GameClient().SendSnail();
	}), (count % 4) * 150, (count / 4) * 220);
	#endif

	//Player Upgrades
	count = 0;
	auto player_upgrades = SpaceGame::Instance().GetPlayer().GetPossibleUpgrades();
	while (player_upgrades.size() > 0)
	{
		widget_player_upgrades->Add(std::make_shared<WidgetPlayerUpgrade>(this, player_upgrades.Take(0)), 0, count * 55);
		count++;
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