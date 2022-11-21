#include <string>

class ItemUpgrade
{
protected:
	int cost;
	std::string description;
	int& upgrade;
	int increment;

public:
	Texture* texture;
	int nTextureFrame;
	ItemUpgrade(int cost, int& upgrade, const std::string& description, Texture* texture, int nTextureFrame = 0, int increment = 1) 
		: upgrade(upgrade)
	{
		this->cost = cost;
		this->description = description;
		this->texture = texture;
		this->nTextureFrame = nTextureFrame;
		this->increment = increment;
	}
	virtual void DoUpgrade() {
		upgrade += increment;
	};
	const std::string& Description() {
		return description;
	};
	int Cost() { return cost; }
};

class PlayerUpgrade
{
protected:
	int cost = 0;
	std::string description;
	float& current_value;
	const float& max_value;
	float increment;

public:
	PlayerUpgrade(float& current_value, const float& max_value, float increment, int cost, const std::string& description = "")
		: current_value(current_value),
		max_value(max_value)
	{
		this->description = description;
		this->cost = cost;
		this->increment = increment;
	}
	virtual void DoUpgrade() {
		current_value += increment;
	}
	virtual std::string Description() {
		return description;
	}
	int Cost() { return cost; }
	float Current() { return current_value; }
	float Max() { return max_value; }
	float Increment() { return increment; }
};