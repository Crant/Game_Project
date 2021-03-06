/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "GraphicsEngine.h"
#include "Graphics.h"
#include <string>

class GuiElement
{
public:
	GuiElement();
	GuiElement(float x, float y, float width, float height, std::string textureName);
	virtual ~GuiElement();
	/*! Sets the Position of the GuiElement*/
	void SetPosition(const float x, const float y) {this->zX = x; this->zY = y; if(zGuiImage) zGuiImage->SetPosition(Vector2(x, y));}
	/*! Sets the Position of the GuiElement*/
	void SetPosition(Vector2 pos) {this->zX = pos.x; this->zY = pos.y; if(zGuiImage) zGuiImage->SetPosition(pos);}
	/*! Saves the Position to the Parameters*/
	void GetPosition(float& x, float& y) {x = this->zX; y = this->zY;}
	/*! Returns position in a Vector2*/
	Vector2 GetPosition() { return Vector2(this->zX, this->zY); }
	/*! Sets the Width of the GuiElement*/
	void SetWidth(float width) {this->zWidth = width;}
	/*! Saves the Width to the Parameter*/
	void GetWidth(float& width) {width = this->zWidth;}
	/*! Sets the Height of the GuiElement*/
	void SetHeight(float height) {this->zHeight = height;}
	/*! Saves the Height to the Parameter*/
	void GetHeight(float& height) {height = this->zHeight;}
	/*! Returns the GuiElement Dimensions as a Vector2*/
	Vector2 GetDimension() {return Vector2(this->zWidth, this->zHeight);}
	/*! Sets the Texture Variable*/
	void SetTextureName(const std::string& textureName) {this->zTextureName = textureName;}
	/*! Saves the Texture Variable to the Parameter*/
	void GetTextureName(std::string& textureName) {textureName = this->zTextureName;}
	/*! Sets the Texture Variable*/
	void SetImage(iImage* image) {this->zGuiImage = image;}
	/*! Saves the Texture Variable to the Parameter*/
	iImage* GetImage() {return this->zGuiImage;}
	/*! Adds the GuiElement To the Renderer and starts Rendering*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the GuiElement From the Renderer and Stops Rendering it*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);
	void SetStrata(float value){ if(this->zGuiImage) this->zGuiImage->SetStrata(value); this->zStrata = value; }

	void SetDimension(Vector2 dim){ if(this->zGuiImage) zGuiImage->SetDimensions(dim); this->SetWidth(dim.x); this->SetHeight(dim.y); }

	virtual bool IsHidden() {return this->zHidden;}
	void HideGui();
	void ShowGui();
	void FadeOut(float value);

protected:
	float zX;
	float zY;
	float zWidth;
	float zHeight;
	bool zHidden;
	bool zPressed;
	bool zHovered;
	float zStrata;
	float zOpacity;
	iImage* zGuiImage;
	std::string zTextureName;
};