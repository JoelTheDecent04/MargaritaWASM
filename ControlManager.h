#include <unordered_map>
#include <vector>

class Control
{
public:
	enum class Action
	{
		None,
		Jump,
		Left,
		Right,
		Down,
		OpenShop,
		NextWave,
		Slot1,
		Slot2,
		Slot3,
		Slot4,
		Slot5,
		Slot6,
		Slot7,
		Slot8,
		ShowHideMap
	};
private:
	const char* description;
	int primary;
	int secondary;
	Action action;

public:
	Control(Action action, const char* description, int primary, int secondary);
	const char* Description() const { return description; }
	int Primary() const { return primary; }
	int Secondary() const { return secondary; }
	void SetPrimary(int key) { primary = key; }
	void SetSecondary(int key) { secondary = key; }
	Action GetAction() const { return action; }

	const char* PrimaryKeyText() const;
};

class ControlManager
{
	static ControlManager control_manager;

private:
	ControlManager();
	std::vector<Control> controls;


public:
	const Control& GetControl(int key);
	const Control& GetControlFromAction(Control::Action action);
	
	void AddControl(Control::Action action, const char* description, int primary, int secondary = 0);

	std::vector<Control>& AllControls() { return controls; }

public:
	static ControlManager& Instance() {
		return control_manager;
	}

	static bool IsControlActivated(const Control& control);
	static const char* KeyText(int nKey);
};