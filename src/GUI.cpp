#include "GUI.h"
#include "Graphics.h"
#include "Game.h"
#include "Colours.h"
#include "Utilities.h"


Bar::Bar(int left, int top, int right, int bottom, SDL_Color clrColour, float* fValue, float* fMaxValue)
	: clrColour(clrColour)
{
	this->rect = { left, top, right, bottom }; this->fValue = fValue; this->fMaxValue = fMaxValue;
}

void Bar::Draw()
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * ((*fValue / *fMaxValue) * (rect.right - rect.left)), fScaleV * (rect.bottom - rect.top), clrColour);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
}

Button::Button(int left, int top, int right, int bottom, std::function<void()> function, const char* text, int text_size)
{
	this->rect = { left, top, right, bottom };
	this->function = function;
	this->text = text;
	this->text_size = text_size;
	bClickable = true;
};

void Button::Click()
{
	function();
}

void Button::Draw(bool bHover)
{
	Graphics::FillRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), bClickable ? (bHover ? clrDarkGrey : clrBlack) : clrDarkerGrey);
	Graphics::DrawRectangle(fScaleH * rect.left, fScaleV * rect.top, fScaleH * (rect.right - rect.left), fScaleV * (rect.bottom - rect.top), clrDarkGrey);
	TextSize textsize;
	Graphics::TextMetrics(text, Graphics::GetFont(text_size), textsize);
	Graphics::WriteText(text,
		fScaleH * ((rect.right + rect.left) / 2) - textsize.width / 2,
		fScaleV * ((rect.bottom + rect.top) / 2) - textsize.height / 2,
		Graphics::GetFont(text_size));
}


//New GUI
WidgetGroup::WidgetGroup(int width, int height)
{
	this->width = width;
	this->height = height;
	nCurrentHover = -1;
}
void WidgetGroup::ClickOn(int x, int y)
{
	for (int i = 0; i < children.size(); i++)
	{
		auto& child = children[i];
		if (PointInRect(Rect(child.x,child.y, child.x + child.widget->width, child.y + child.widget->height), x, y))
			child.widget->ClickOn(x - child.x, y - child.y);
		else
			child.widget->ClickOff(x - child.x, y - child.y);
	}
}
void WidgetGroup::ClickOff(int x, int y)
{
	for (auto& child : children)
		child.widget->ClickOff(x, y);
}
void WidgetGroup::OnMouseEnter()
{
}
void WidgetGroup::OnMouseLeave()
{
}
void WidgetGroup::Update(float deltatime, int mouse_x, int mouse_y)
{	
	//Check if still hovering over previous widget
	if (nCurrentHover >= 0)
	{
		auto& child = children[nCurrentHover];

		if (PointInRect(Rect(child.x, child.y, child.x + child.widget->width, child.y + child.widget->height), mouse_x, mouse_y) == false) {
			child.widget->OnMouseLeave();
			nCurrentHover = -1;
		}
	}
	
	//Check if hovering over chidren
	if (nCurrentHover == -1)
	{
		for (int i = children.size() - 1; i >= 0; i--)
		{
			auto& child = children[i];
			if (PointInRect(Rect(child.x, child.y, child.x + child.widget->width, child.y + child.widget->height), mouse_x, mouse_y))
			{
				child.widget->OnMouseEnter();
				nCurrentHover = i;
				break;
			}
		}
	}

	//Update children
	for (auto& child : children)
		child.widget->Update(deltatime, mouse_x - child.x, mouse_y - child.y);
}
void WidgetGroup::TextInput(const char* text)
{
	for (auto& child : children)
		child.widget->TextInput(text);
}
void WidgetGroup::KeyDown(int key)
{
	for (auto& child : children)
		child.widget->KeyDown(key);
}
void WidgetGroup::Draw(int x, int y)
{
	for (auto& child : children)
		child.widget->Draw(x + child.x, y + child.y);
}

WidgetBitmapButton::WidgetBitmapButton(int width, int height, std::function<void()> function, Texture* tTexture, int nFrame)
{
	this->width = width;
	this->height = height;
	this->function = function;
	this->tTexture = tTexture;
	this->nFrame = nFrame;
	hovering = false;
}

void WidgetBitmapButton::Draw(int x, int y)
{
	//tTexture->Draw(nFrame, x, y);
	tTexture->DrawDifferentSize(nFrame, x, y, width, height);
	if (hovering)
		Graphics::FillRectangle(fScaleH * x, fScaleV * y, fScaleH * width, fScaleV * height, clrBlack, 0.35f);
}
void WidgetBitmapButton::ClickOn(int x, int y)
{
	function();
}
void WidgetBitmapButton::OnMouseEnter()
{
	hovering = true;
}
void WidgetBitmapButton::OnMouseLeave()
{
	hovering = false;
}


WidgetButton::WidgetButton(int width, int height, std::function<void()> function, const char* text, int text_size)
{
	this->width = width;
	this->height = height;
	this->function = function;
	this->text = text;
	this->text_size = text_size;
	hovering = false;
	clickable = true;
}
void WidgetButton::Draw(int x, int y)
{
	Graphics::FillRectangle(fScaleH * x, fScaleV * y, fScaleH * width, fScaleV * height, clickable ? (hovering ? clrDarkGrey : clrBlack) : clrDarkerGrey);
	SDL_RenderFlush(Graphics::pRenderer);
	Graphics::DrawRectangle(fScaleH * x, fScaleV * y, fScaleH * width, fScaleV * height, clrDarkGrey);
	SDL_RenderFlush(Graphics::pRenderer);
	

	if (text)
		TextRenderer().SetSize(text_size).SetPosition(x + width / 2, y + height / 2).SetAlignment(Alignment::Centre).SetText(TextBuf(text));
}
void WidgetButton::ClickOn(int x, int y)
{
	if (clickable)
		function();
}
void WidgetButton::OnMouseEnter()
{
	hovering = true;
}
void WidgetButton::OnMouseLeave()
{
	hovering = false;
}

WidgetLabel::WidgetLabel(const char* text, Alignment align, int text_size)
{
	this->width = 0;
	this->height = 0;
	this->text = text;
	this->text_size = text_size;
	this->align = align;
}

void WidgetLabel::Draw(int x, int y)
{
	TextRenderer().SetPosition(x, y).SetSize(text_size).SetAlignment(align).SetText(TextBuf(text));	
}

WidgetTextbox::WidgetTextbox(int width, Alignment align, bool bStartActive, int text_size)
{
	this->width = width;
	this->height = text_size + 4;
	active = bStartActive;
	this->text_size = text_size;
	this->align = align;
}

void WidgetTextbox::ClickOn(int x, int y)
{
	active = true;
}

void WidgetTextbox::TextInput(const char* input)
{
	if (active)
		text.append(input);
}

void WidgetTextbox::KeyDown(int key)
{
	if (active && key == SDL_SCANCODE_BACKSPACE && text.size() > 0)
		text.pop_back();
}

void WidgetTextbox::Draw(int x, int y)
{
	RectRenderer().SetPosition(x - 2, y - 2).SetAlignment(Alignment::LeftCentre).SetSize(width + 4, text_size + 4).Draw();
	TextRenderer().SetAlignment(Alignment::LeftCentre).SetPosition(x, y).SetSize(text_size).SetText(TextBuf(text));
}

WidgetBitmap::WidgetBitmap(int width, int height, Texture* tTexture, int nFrame)
{
	this->width = width;
	this->height = height;
	this->tTexture = tTexture;
	this->nFrame = nFrame;
}
void WidgetBitmap::Draw(int x, int y)
{
	tTexture->DrawDifferentSize(
		nFrame, x, y,
		width, height);
}

WidgetBar::WidgetBar(int width, int height, SDL_Color clrColour, float* fValue, float* fMaxValue)
{
	this->width = width;
	this->height = height;
	this->clrColour = clrColour;
	this->fValue = fValue;
	this->fMaxValue = fMaxValue;
}

void WidgetBar::Draw(int x, int y)
{
	Graphics::FillRectangle(fScaleH * x, fScaleV * y, fScaleH * ((*fValue / *fMaxValue) * width), fScaleV * height, clrColour);
	Graphics::DrawRectangle(fScaleH * x, fScaleV * y, fScaleH * width, fScaleV * height, clrDarkGrey);
}
