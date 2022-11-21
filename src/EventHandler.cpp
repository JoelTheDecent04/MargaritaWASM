#include "EventHandler.h"
#include "Utilities.h"
#include "Entity.h"
#include "Player.h"

#include <sstream>
#include <string>

Entity::Type anWeapons[3] = { Entity::Type::Bomb, Entity::Type::Laser, Entity::Type::Orb };
Entity::Type anEnemies[2] = { Entity::Type::Enemy, Entity::Type::Crab };

EventHandler::EventHandler()
{
	objective = Objective::Make();
}

void EventHandler::Event(EventType nType, int nCount, Entity* a, Entity* b)
{
	objective->CheckEvent(nType, nCount, a, b);
	
	if (objective->Completed())
		objective = Objective::Make();
}

void EventHandler::Update(float deltatime)
{
	/*
	for (size_t i = 0; i < vCallbacks.size(); i++) {
		auto& callback = *vCallbacks[i];
		callback.time_left -= deltatime;
		if (callback.time_left <= 0.0f) {
			callback.Run();
			vCallbacks.erase(vCallbacks.begin() + (i--));
		}
	}*/

	for (size_t i = 0; i < vDelayedFunctions.size(); i++) {
		auto& func = vDelayedFunctions[i];
		func.time_left -= deltatime;

		if (func.time_left <= 0.0f) {
			func.fn(func.data);
			vDelayedFunctions.erase(vDelayedFunctions.begin() + (i--));
		}
	}
}
/*
void EventHandler::RegisterCallback(CallbackBase* callback, int ms)
{
	callback->time_left = (float)ms / 1000.0f;
	vCallbacks.push_back(std::unique_ptr<CallbackBase>(callback));
}
*/
void EventHandler::Reset()
{
	//vCallbacks.clear();
	vDelayedFunctions.clear();
}

class EnemyKillWithWeaponObjective : public Objective
{
private:
	int enemies_left;
	Entity::Type enemy_type;
	Entity::Type weapon_type;

public:
	EnemyKillWithWeaponObjective() {
		enemies_left = (int)(randomf() * 6) * 5 + 10;
		enemy_type = anEnemies[(int)roundf((randomf() * 1.0f))];
		weapon_type = anWeapons[(int)roundf((randomf() * 2.0f))];
	}
	std::string AsText() const override {
		std::stringstream ss;
		ss << "Objective: Kill " << enemies_left << " x " << astrEntityName[(int)enemy_type] << " using " << astrEntityName[(int)weapon_type];
		return ss.str();
	}

	void CheckEvent(EventType type, int count, Entity* a, Entity* b) override
	{
		if (type == EventType::Kill)
		{
			if (a && b && a->GetType() == weapon_type && b->GetType() == enemy_type)
			{
				enemies_left--;
				if (enemies_left == 0)
				{
					SpaceGame::Instance().GetPlayer().fMoney += 120.0f;
					SpaceGame::Instance().GetPlayer().ChangeHealth(100.0f, nullptr);
					completed = true;
				}
			}
		}
	}
};

std::unique_ptr<Objective> Objective::Make()
{
	return std::make_unique<EnemyKillWithWeaponObjective>();
}
