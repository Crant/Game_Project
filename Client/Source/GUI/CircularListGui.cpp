#include "CircularListGui.h"
#include <Safe.h>

static const int PICONEXPOS = 60;
static const int PICONEYPOS = 60;

CircularListGui::CircularListGui() 
	: GuiElement()
{
	this->zPressed = false;
	this->zHovered = false;
	std::fill_n(this->zOptions, 4, NOTHING);
	for(int i = 0; i < 4; i++)
		this->zTextImages[i] = NULL;

	for(int i = 0; i < 4; i++)
		zTextImagePos[i] = Vector2(0, 0);
}

CircularListGui::CircularListGui(float x, float y, float width, float height, std::string textureName, int options[]) 
	: GuiElement(x, y, width, height, textureName)
{
	GraphicsEngine* ge = GetGraphics();
	this->zPressed = false;
	this->zHovered = false;
	memcpy(this->zOptions, options, 4*4);

	float windowWidth = (float)(ge->GetEngineParameters().WindowWidth);
	float windowHeight = (float)(ge->GetEngineParameters().WindowHeight);

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	for(int i = 0; i < 4; i++)
		this->zTextImages[i] = NULL;

	zTextImagePos[0] = Vector2(x + (60.0f / 1024.0f) * dx, x + (60.0f / 768.0f) * windowHeight);
	zTextImagePos[1] = Vector2(x + (140.0f / 1024.0f) * dx, x + (60.0f / 768.0f) * windowHeight);
	zTextImagePos[2] = Vector2(x + (60.0f / 1024.0f) * dx, x + (140.0f / 768.0f) * windowHeight);
	zTextImagePos[3] = Vector2(x + (140.0f / 1024.0f) * dx, x + (140.0f / 768.0f) * windowHeight);
}

CircularListGui::~CircularListGui()
{
	for(int i = 0; i < 4; i++)
	{
		if(this->zTextImages[i])
		{
			if ( this->zTextImages[i] ) throw("Image leaked!");
		}
	}
}

bool CircularListGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	this->SetStrata(110.0f);
	if (ge)
	{
		float windowWidth = (float)(ge->GetEngineParameters().WindowWidth);
		float windowHeight = (float)(ge->GetEngineParameters().WindowHeight);

		float dx = ((float)windowHeight * 4.0f) / 3.0f;
		float offSet = (float)(windowWidth - dx) / 2.0f;
		float imageWidth;
		float imageHeight;
		int value;
		std::string texName = "";
		for(int i = 0; i < 4; i++)
		{
			value = this->zOptions[i];
			imageWidth = (80.0f / 1024.0f) * dx;
			imageHeight = (20.0f / 768.0f) * windowHeight;
			switch(value)
			{
			case USE:
				texName = "Media/Icons/Use.png";
				break;
			case CRAFT:
				texName = "Media/Icons/Craft.png";
				break;
			case DROP:
				texName = "Media/Icons/Drop.png";
				break;
			case UNEQUIP:
				texName = "Media/Icons/Unequip.png";
				break;
			case EQUIP:
				texName = "Media/Icons/Equip.png";
				break;
			case LOOT:
				texName = "Media/Icons/Loot.png";
				break;
			case FILL:
				texName = "Media/Icons/Fill.png";
				break;
			case NOTHING:
				texName = "Media/Icons/Cancel.png";
				break;
			default:
				texName = "";
				break;
			}
			this->zTextImages[i] = ge->CreateImage(Vector2(this->zTextImagePos[i].x - (imageWidth * 0.5f), 
			this->zTextImagePos[i].y - (imageHeight * 0.5f)), Vector2(imageWidth, imageHeight), texName.c_str());
			this->zTextImages[i]->SetStrata(100.0f);
		}
		return true;
	}
	
	return true;
}

bool CircularListGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	if (ge)
	{
		GuiElement::RemoveFromRenderer(ge);
		if (this->zGuiImage)
		{
			ge->DeleteImage(this->zGuiImage);
			this->zGuiImage = 0;
		}
		for(int i = 0; i < 4; i++)
			if(this->zTextImages[i])
			{
				ge->DeleteImage(this->zTextImages[i]);
				this->zTextImages[i] = NULL;
			}

		return true;
	}
	

	return false;
}

int CircularListGui::CheckCollision(float mouseX, float mouseY, bool mousePressed)
{
	Vector2 dimension = this->GetDimension();
	if(mousePressed)
	{
		if(mouseX < (this->zX + this->zWidth * 0.5f) && mouseY < (this->zY + this->zHeight * 0.5f))
			return this->zOptions[0];
		else if(mouseX > (this->zX + this->zWidth * 0.5f) && mouseY < (this->zY + this->zHeight * 0.5f))
			return this->zOptions[1];
		else if(mouseX < (this->zX + this->zWidth * 0.5f) && mouseY > (this->zY + this->zHeight * 0.5f))
			return this->zOptions[2];
		else if(mouseX > (this->zX + this->zWidth * 0.5f) && mouseY > (this->zY + this->zHeight * 0.5f))
			return this->zOptions[3];
	}

	return -1;
}

void CircularListGui::HideGui()
{
	/*if (this->zItemImage)
	{
		this->zHidden = true;
		this->zOpacity = 0.0f;
		this->zItemImage->SetOpacity(this->zOpacity);
	}*/
	GuiElement::HideGui();
}

void CircularListGui::ShowGui()
{
	/*if (this->zItemImage)
	{
		this->zHidden = false;
		this->zOpacity = 1.0f;
		this->zItemImage->SetOpacity(this->zOpacity);
	}*/
	GuiElement::ShowGui();
}

void CircularListGui::FadeOut(float value)
{
	/*if (this->zItemImage)
	{
		if (this->zOpacity > 0.0f)
		{
			this->zOpacity -= value;
			this->zItemImage->SetOpacity(this->zOpacity);
		}
		else
		{
			this->HideGui();
		}
	}*/
	GuiElement::FadeOut(value);
}

void CircularListGui::Resize( int width, int height, int dx )
{
	float tempWidth = (CIRCLISTRADIUS / 1024.0f) * dx;
	float tempHeight = (CIRCLISTRADIUS / 768.0f) * height;

	this->SetDimension(Vector2(tempWidth, tempHeight));
}

void CircularListGui::SetPosition( Vector2 pos )
{
	GuiElement::SetPosition(pos);

	float windowWidth = (float)(GetGraphics()->GetEngineParameters().WindowWidth);
	float windowHeight = (float)(GetGraphics()->GetEngineParameters().WindowHeight);

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	zTextImagePos[0] = Vector2(pos.x + (60.0f / 1024.0f) * dx, pos.y + (60.0f / 768.0f) * windowHeight);
	zTextImagePos[1] = Vector2(pos.x + (140.0f / 1024.0f) * dx, pos.y + (60.0f / 768.0f) * windowHeight);
	zTextImagePos[2] = Vector2(pos.x + (60.0f / 1024.0f) * dx, pos.y + (140.0f / 768.0f) * windowHeight);
	zTextImagePos[3] = Vector2(pos.x + (140.0f / 1024.0f) * dx, pos.y + (140.0f / 768.0f) * windowHeight);
}

void CircularListGui::Adjust(int type, int inventory)
{
	this->zOptions[0] = NOTHING;
	this->zOptions[1] = NOTHING;
	this->zOptions[2] = NOTHING;
	this->zOptions[3] = NOTHING;
	if(inventory == 1)
	{
		this->zOptions[0] = UNEQUIP;
		this->zOptions[1] = DROP;
		return;
	}
	else if(inventory == 2)
	{
		this->zOptions[0] = LOOT;
		return;
	}
	switch(type)
	{
	case ITEM_TYPE_WEAPON_RANGED:
		this->zOptions[0] = EQUIP;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_WEAPON_MELEE:
		this->zOptions[0] = EQUIP;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_GEAR:
		break;
	case ITEM_TYPE_CONTAINER:
		this->zOptions[0] = USE;
		this->zOptions[1] = FILL;
		this->zOptions[2] = DROP;
		break;
	case ITEM_TYPE_FOOD:
		this->zOptions[0] = USE;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_PROJECTILE:
		this->zOptions[0] = EQUIP;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_MATERIAL:
		this->zOptions[0] = CRAFT;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_BANDAGE:
		this->zOptions[0] = USE;
		this->zOptions[1] = DROP;
		break;
	case ITEM_TYPE_MISC:
		this->zOptions[0] = USE;
		this->zOptions[1] = DROP;
		break;
	default:
		break;
	}
}