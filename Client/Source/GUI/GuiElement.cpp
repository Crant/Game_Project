#include "GuiElement.h"
#include <Safe.h>

GuiElement::GuiElement()
{
	this->zX = 0;
	this->zY = 0;
	this->zWidth = 0;
	this->zHeight = 0;
	this->zGuiImage = NULL;
	this->zOpacity = 0.0f;
	this->zHidden = true;
	this->zTextureName = "Unknown";
	this->zStrata = 500.0f;
}

GuiElement::GuiElement(float x, float y, float width, float height, std::string textureName)
{
	this->zX = x;
	this->zY = y;
	this->zGuiImage = NULL;
	this->zHidden = true;
	this->zOpacity = 0.0f;
	this->zWidth = width;
	this->zHeight = height;
	this->zTextureName = textureName;
	this->zStrata = 500.0f;
}

GuiElement::~GuiElement()
{
	if ( this->zGuiImage && GetGraphics()->IsRunning()) 
	{
		GetGraphics()->DeleteImage(this->zGuiImage);
		this->zGuiImage = NULL;
	}
	else if(this->zGuiImage && !GetGraphics()->IsRunning())
		throw("Image leaked!");
}

bool GuiElement::AddToRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		if(!this->zGuiImage && this->zTextureName != "")
		{
			this->zGuiImage = ge->CreateImage(Vector2(this->zX , this->zY), this->GetDimension(), this->zTextureName.c_str());
			this->zGuiImage->SetStrata(this->zStrata);
		}
		this->ShowGui();
		return true;
	}
	return false;
}

bool GuiElement::RemoveFromRenderer(GraphicsEngine* ge)
{
	if (this->zGuiImage)
	{
		ge->DeleteImage(this->zGuiImage);
		this->zGuiImage = 0;
		this->zHidden = true;
		return true;
	}
	return false;
}

void GuiElement::HideGui()
{
	if (this->zGuiImage)
	{
		this->zHidden = true;
		this->zOpacity = 0.0f;
		this->zGuiImage->SetOpacity(this->zOpacity);
	}
}

void GuiElement::ShowGui()
{
	if (this->zGuiImage)
	{
		this->zHidden = false;
		this->zOpacity = 1.0f;
		this->zGuiImage->SetOpacity(this->zOpacity);
	}
}

void GuiElement::FadeOut(float value)
{
	if (this->zGuiImage)
	{
		if (this->zOpacity > 0.0f)
		{
			this->zOpacity -= value;
			this->zGuiImage->SetOpacity(this->zOpacity);
		}
		else
		{
			this->HideGui();
		}
	}
}
