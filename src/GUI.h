#pragma once
#include "Utilities.h"
#include "Graphics.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>
#include <functional>

class Bar //TODO add horizontal / vertical
{
public:
	Rect rect;
	SDL_Color clrColour;
	float* fValue;
	float* fMaxValue;
	Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue);
	void Draw();
};

class Button
{
	std::function<void()> function;
	const char* text;
	int text_size;
	bool bClickable;
public:
	Rect rect;
	Button(int left, int top, int right, int bottom, std::function<void()> function, const char* text, int text_size = 24);
	void Click();
	void Draw(bool bHover);
};

class Widget
{
public:
	int width, height;
	virtual void ClickOn(int x, int y) {}; //Relative coordinates
	virtual void ClickOff(int x, int y) {};
	virtual void OnMouseEnter() {};
	virtual void OnMouseLeave() {};
	virtual void Update(float deltatime, int mouse_x, int mouse_y) {};
	virtual void TextInput(const char* text) { };
	virtual void KeyDown(int key) { };
	virtual void Draw(int x, int y) {}; //Real screen coordinates
};

struct WidgetChild {
	std::shared_ptr<Widget> widget;
	
public:
	int x, y;

	WidgetChild(const std::shared_ptr<Widget>& widget, int x, int y) {
		this->widget = widget;
		this->x = x;
		this->y = y;
	}
	std::shared_ptr<Widget> Widget() {
		return widget;
	}
};

class WidgetGroup : public Widget
{
	int nCurrentHover;
	std::vector<WidgetChild> children;
public:
	WidgetGroup(int width, int height);
	void ClickOn(int x, int y) override;
	void ClickOff(int x, int y) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void Update(float deltatime, int mouse_x, int mouse_y) override;
	void TextInput(const char* text) override;
	void KeyDown(int key) override;
	void Draw(int x, int y) override;
	std::vector<WidgetChild>& Children() { return children; }
	void Clear() {
		nCurrentHover = -1;
		children.clear();
	}
	template<typename T>
	std::shared_ptr<T> Add(const std::shared_ptr<T>& widget, int x, int y) {
		children.push_back(WidgetChild(widget,x,y));
		return widget;
	}
};

class WidgetBitmapButton : public Widget
{
	bool hovering;
public:
	std::function<void()> function;
	Texture* tTexture;
	int nFrame;
	WidgetBitmapButton(int width, int height, std::function<void()> function, Texture* tTexture, int nFrame);
	void Draw(int x, int y) override;
	void ClickOn(int x, int y) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
};

class WidgetButton : public Widget
{
	const char* text;
	int text_size;
protected:
	std::function<void()> function;
	bool hovering;
	bool clickable;
public:
	WidgetButton(int width, int height, std::function<void()> function, const char* text, int text_size = 24);
	void Draw(int x, int y) override;
	virtual void ClickOn(int x, int y) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void SetClickable(bool clickable) {
		this->clickable = clickable;
	}
	void SetText(const char* text) {
		this->text = text;
	}
	bool IsClickable() { return clickable; }
	bool Hovering() { return hovering; }
};

class WidgetBar : public Widget
{
	SDL_Color clrColour;
	float* fValue;
	float* fMaxValue;
public:
	WidgetBar(int width, int height, SDL_Color clrColour, float* fValue, float* fMaxValue);
	void Draw(int x, int y) override;
};

enum class TextAlign
{
	Left, Top, Right, Bottom, Centered
};

class WidgetLabel : public Widget
{
	const char* text;
	int text_size;
	Alignment align;
public:
	WidgetLabel(const char* text, Alignment align = Alignment::Centre, int text_size = 24);
	void SetText(const char* text) { this->text = text; };
	void Draw(int x, int y) override;
};

class WidgetTextbox : public Widget
{
	std::string text;
	int text_size;
	Alignment align;
	bool active;
public:
	enum
	{
		StartInactive = 0,
		StartActive = 1
	};
	WidgetTextbox(int width, Alignment align = Alignment::TopLeft, bool bStartActive = false, int text_size = 16);
	void ClickOn(int x, int y) override;
	void TextInput(const char* text) override;
	void KeyDown(int key) override;
	void Draw(int x, int y) override;
	std::string& Text() { return text; }
};

class WidgetBitmap : public Widget
{
protected:
	Texture* tTexture;
	int nFrame;
public:
	WidgetBitmap(int width, int height, Texture* tTexture, int nFrame = 0);
	void Draw(int x, int y) override;
};