#include "InGameMenu.h"
#include <MaloW.h>


InGameMenu::InGameMenu()
{
	this->zEng = GetGraphics();

	zNrOfSets			= 5;	
	zSets				= new GUISet[zNrOfSets];
	this->zPrimarySet	= INGAMEMAIN;

	this->zShow			= false;

	this->Init();
}

InGameMenu::~InGameMenu()
{
	delete [] zSets;
	zSets = 0;
}

int InGameMenu::Run()
{
	this->Resize();
	bool resize = false;
	int returnValue = IGNOTHING;
	bool removeMenu = false;
	GUIEvent* guiEvent = NULL;

	Vector2 mousePos = this->zEng->GetKeyListener()->GetMousePosition();
	bool mousePressed = this->zEng->GetKeyListener()->IsClicked(1);
	guiEvent = this->zSets[this->zPrimarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, mousePressed, this->zEng);
	if(guiEvent != NULL)
	{
		if(guiEvent->GetEventMessage() == "ChangeSetEvent")
		{
			ChangeSetEvent* cse = (ChangeSetEvent*) guiEvent;

			if(cse->GetSet() == IGOPTIONS)
			{
				this->SwapMenus(cse->GetSet());
				this->zPrimarySet = cse->GetSet();
				UpdateOptionsMenu();
			}
			else if(cse->GetSet() == INGAMEMAIN)
			{
				this->SwapMenus(cse->GetSet());
				this->zPrimarySet = cse->GetSet();
			}
			else if(cse->GetSet() == IGRESUME)
			{
				removeMenu = true;
				returnValue = IGRESUME;
			}
			else if(cse->GetSet() == IGQUIT)
			{
				removeMenu = true;
				returnValue = IGQUIT;
			}
		}
		else if(guiEvent->GetEventMessage() == "ApplyOptionsAndChangeSetEvent")
		{
			GraphicsEngine* ge = GetGraphics();
			iGraphicsEngineParams& GEP = ge->GetEngineParameters();

			ApplyOptionsAndChangeSetEvent* cEvent = (ApplyOptionsAndChangeSetEvent*)guiEvent;

			//Maximized
			bool maximized = this->zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox")->GetOn();
			GEP.Maximized = !maximized;
			if(!maximized)
			{
				RECT desktop;
				const HWND hDesktop = GetDesktopWindow();
				GetWindowRect(hDesktop, &desktop);
				float width = (float)desktop.right;
				float height = (float)desktop.bottom;

				int i = IGNOMENU;
				while(i != IGLASTMENU)
				{
					zSets[i].Resize((float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight, width, height);
					i++;
				}
				resize = true;
				this->zSizedForWidth = width;
				this->zSizedForHeight = height;
			}
			else if(maximized)
			{
				float oldWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
				float oldHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;

				float width = oldWidth;
				float height = oldHeight;

				GUIEvent* temp = this->zSets[this->zPrimarySet].GetEventFromDropDown("Resolutions");
				if(ChangeResEvent *cre = dynamic_cast<ChangeResEvent *>(temp))
				{
					if (NULL != cre)
					{
						width = (float)cre->GetWidth();
						height = (float)cre->GetHeight();
					}
				}

				int i = IGNOMENU;
				while(i != IGLASTMENU)
				{
					zSets[i].Resize(oldWidth, oldHeight, width, height);
					i++;
				}
				resize = true;
				this->zSizedForWidth = width;
				this->zSizedForHeight = height;
			}
			// Getting shadow
			std::string tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("ShadowQuality");
			ge->ChangeShadowQuality(MaloW::convertStringToInt(tbTemp));
			GEP.ShadowMapSettings = MaloW::convertStringToInt(tbTemp);
			//FXAA
			CheckBox* cbTemp = this->zSets[this->zPrimarySet].GetCheckBox("FXAACheckBox");
			if(cbTemp != NULL)
			{
				if(cbTemp->GetOn())
					GEP.FXAAQuality = 4;
				if(!cbTemp->GetOn())
					GEP.FXAAQuality = 0;
			}

			//View Distance
			tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("ViewDistance");
			GEP.FarClip = MaloW::convertStringToFloat(tbTemp);

			AudioManager* am = AudioManager::GetInstance();
			//Master Volume
			tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("MasterVolume");
			float masterVolume = MaloW::convertStringToFloat(tbTemp) / 100;

			//Music Volume
			tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("MusicVolume");
			float temp = (MaloW::convertStringToFloat(tbTemp) * masterVolume) / 100;
			am->SetVolume(EVENTCATEGORY_NOTDEADYET_MASTER_MUSIC, temp);

			//Normal Volume
			tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("NormalVolume");
			temp = (MaloW::convertStringToFloat(tbTemp) * masterVolume) / 100;
			am->SetVolume(EVENTCATEGORY_NOTDEADYET_MASTER_SOUND, temp);

			GEP.SaveToFile("Config.cfg");

			this->SwapMenus(cEvent->GetSet());
			zPrimarySet = cEvent->GetSet();
			if(resize)
			{
				returnValue = IGRESIZE;
			}
		}
	}
	if(this->zEng->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		returnValue = IGRESUME;
		removeMenu = true;
	}
	if(removeMenu)
		this->zSets[this->zPrimarySet].RemoveSetFromRenderer(this->zEng);

	return returnValue;
}

void InGameMenu::Init()
{
	GraphicsEngine* eng = GetGraphics();

	float windowWidth = (float)eng->GetEngineParameters().WindowWidth;
	float windowHeight = (float)eng->GetEngineParameters().WindowHeight;

	this->zSizedForWidth = windowWidth;
	this->zSizedForHeight = windowHeight;

	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	//Ingame Main
	float buttonWidth = (216.f / 1024.0f) * dx;
	float buttonHeight = (75.f / 768.0f) * windowHeight;
	float MainXPos = (windowWidth / 2) - (buttonWidth / 2);

	Element* temp = new SimpleButton(MainXPos, (200.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/InGameMenu/ResumeButton.png", 
		buttonWidth, buttonHeight, new ChangeSetEvent(IGRESUME), "Media/Menu/InGameMenu/ResumeButtonClick.png", "Media/Menu/InGameMenu/ResumeButtonOver.png", 
		MainXPos, (200.0f / 768.0f) * windowHeight, buttonWidth, buttonHeight);
	zSets[INGAMEMAIN].AddElement(temp);

	temp = new SimpleButton(MainXPos, (350.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/InGameMenu/Options.png", 
		buttonWidth, buttonHeight, new ChangeSetEvent(IGOPTIONS), "Media/Menu/InGameMenu/OptionsClick.png", "Media/Menu/InGameMenu/OptionsOver.png", 
		MainXPos, (350.0f / 768.0f) * windowHeight, buttonWidth, buttonHeight);
	zSets[INGAMEMAIN].AddElement(temp);

	temp = new SimpleButton(MainXPos, (500.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/InGameMenu/Quit.png", 
		buttonWidth, buttonHeight, new ChangeSetEvent(IGQUIT), "Media/Menu/InGameMenu/QuitClick.png", "Media/Menu/InGameMenu/QuitOver.png", 
		MainXPos, (500.0f / 768.0f) * windowHeight, buttonWidth, buttonHeight);
	zSets[INGAMEMAIN].AddElement(temp);

	//OPTIONS Menu

	//Graphics Column
	temp = new GUIPicture(offSet, (59.0f / 768.0f) * dx, 1.0f, "Media/Menu/Options/OptionsBG.png", 
		(1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp); 

	//Option Text
	temp = new GUIPicture(offSet + (40.0f / 1024.0f) * dx, (130.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/OptionsText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp); 

	//Graphics Text
	temp = new GUIPicture(offSet + (100.0f / 1024.0f) * dx, (210.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/GraphicsText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//WindowedText
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (240.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/WindowedText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Resolution Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (300.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ResolutionText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Shadow Quality Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (420.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ShadowText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//FXAA Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (360.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/FXAAText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//View Distance Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (480.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ViewdistanceText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	// Sound Column
	//Sound Text
	temp = new GUIPicture(offSet + (500.0f / 1024.0f) * dx, (210.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/SoundText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Mastervolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (240.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/MastervolumeText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Musicvolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (300.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/MusicvolumeText.png",
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Normalvolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (360.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/NormalvolumeText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	// Tech stuff
	//Graphics
	//DropDown list
	temp = new DropDownList(offSet + (240.0f / 1024.0f) * dx, (295.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/DropDownBG.png", 
		(170.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Resolutions");
	DropDownList* dropdownlist = (DropDownList*)temp;

	this->AddResolutionsToDropBox(dropdownlist);

	zSets[IGOPTIONS].AddElement(temp);

	//CheckBox FXAA
	bool checked = false;
	if(GetGraphics()->GetEngineParameters().FXAAQuality > 0)
		checked = true;

	temp = new CheckBox(offSet + (195.0f / 1024.0f) * dx, (355.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/CheckBoxFrame.png", 
		(32.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Media/Menu/OPTIONS/CheckBoxCheck.png", checked, 
		new ChangeOptionEvent("FXAA", "false"), "FXAACheckBox");
	zSets[IGOPTIONS].AddElement(temp);

	//CheckBox Windowed
	checked = !GetGraphics()->GetEngineParameters().Maximized;

	temp = new CheckBox(offSet + (240.0f / 1024.0f) * dx, (233.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/CheckBoxFrame.png", 
		(32.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Media/Menu/OPTIONS/CheckBoxCheck.png", checked, 
		new ChangeOptionEvent("Windowed", "false"), "WindowedCheckBox");
	zSets[IGOPTIONS].AddElement(temp);

	//TextBox View Distance
	temp = new TextBox(offSet + (278.0f / 1024.0f) * dx, (470.0f / 768.0f) * windowHeight, 2.0f, "", 
		(40.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().FarClip), 
		"ViewDistance", 2.0f, 3, NR, 0, 9);
	zSets[IGOPTIONS].AddElement(temp);

	//TextBox Shadow
	temp = new TextBox(offSet + (295.0f / 1024.0f) * dx, (412.0f / 768.0f) * windowHeight, 2.0f, "", 
		(40.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().ShadowMapSettings), 
		"ShadowQuality", 2.0f, 1, NR, 0, 9);
	zSets[IGOPTIONS].AddElement(temp);

	//Sound tech
	//Master volume
	temp = new TextBox(offSet + (690.0f / 1024.0f) * dx, (235.0f / 768.0f) * windowHeight, 2.0f, "", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"MasterVolume", 2.0f, 2, NR);
	zSets[IGOPTIONS].AddElement(temp);

	//Music Volume
	temp = new TextBox(offSet + (680.0f / 1024.0f) * dx, (295.0f / 768.0f) * windowHeight, 2.0f, "", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"MusicVolume", 2.0f, 2, NR);
	zSets[IGOPTIONS].AddElement(temp);

	//Normal Volume
	temp = new TextBox(offSet + (695.0f / 1024.0f) * dx, (355.0f / 768.0f) * windowHeight, 2.0f, "", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"NormalVolume", 2.0f, 2, NR);
	zSets[IGOPTIONS].AddElement(temp);


	//Buttons IGOPTIONS menu
	//Back
	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Back.png", 
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ChangeSetEvent(INGAMEMAIN), "Media/Menu/BackPress.png", "Media/Menu/BackOver.png", 
		offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);

	//Apply
	temp = new SimpleButton(offSet + (148.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Apply.png", 
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ApplyOptionsAndChangeSetEvent(INGAMEMAIN), "Media/Menu/ApplyPress.png", "Media/Menu/ApplyOver.png", 
		offSet + (148.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[IGOPTIONS].AddElement(temp);
}

void InGameMenu::AddResolutionsToDropBox( DropDownList* ddl )
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	float width = (float)desktop.right;
	float height = (float)desktop.bottom;

	float dx = (float)(width / height);


	if(dx > 1.77f && dx < 1.78f)
	{
		ddl->AddButton("Media/Menu/Options/169small.png", new ChangeResEvent(1280, 720), "", "");
		ddl->AddButton("Media/Menu/Options/169medium.png", new ChangeResEvent(1600, 900), "", "");
		ddl->AddButton("Media/Menu/Options/169big.png", new ChangeResEvent(1920, 1080), "", "");
	}
	else if(dx > 1.59f && dx < 1.61f)
	{
		ddl->AddButton("Media/Menu/Options/1610small.png", new ChangeResEvent(1280, 800), "", "");
		ddl->AddButton("Media/Menu/Options/1610medium.png", new ChangeResEvent(1440, 900), "", "");
		ddl->AddButton("Media/Menu/Options/1610big.png", new ChangeResEvent(1680, 1050), "", "");
	}
	else
	{
		ddl->AddButton("Media/Menu/Options/43small.png", new ChangeResEvent(800, 600), "", "");
		ddl->AddButton("Media/Menu/Options/43medium.png", new ChangeResEvent(1024, 768), "", "");
		ddl->AddButton("Media/Menu/Options/43big.png", new ChangeResEvent(1600, 1200), "", "");
	}
}

void InGameMenu::UpdateOptionsMenu()
{
	//Update the IGOPTIONS menu cause values might have changed.

	//Update maximized.
	CheckBox* cbTemp = zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox");
	cbTemp->SetChecked(!GetGraphics()->GetEngineParameters().Maximized);

	//Update FXAA.
	cbTemp = zSets[this->zPrimarySet].GetCheckBox("FXAACheckBox");
	if(GetGraphics()->GetEngineParameters().FXAAQuality == 0)
		cbTemp->SetChecked(false);
	else
		cbTemp->SetChecked(true);

	//Update Shadow Quality.
	TextBox* tbTemp = this->zSets[this->zPrimarySet].GetTextBox("ShadowQuality");
	tbTemp->SetText(MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().ShadowMapSettings));

	//Update View Distance.
	tbTemp = this->zSets[this->zPrimarySet].GetTextBox("ViewDistance");
	tbTemp->SetText(MaloW::convertNrToString(GetGraphics()->GetEngineParameters().FarClip));
}

void InGameMenu::SwapMenus(int primary)
{
	GraphicsEngine* eng = GetGraphics();
	if(primary == IGNOMENU)
	{
		zSets[this->zPrimarySet].RemoveSetFromRenderer(eng);
		this->zPrimarySet = primary;
	}
	else if(primary != -1)
	{
		zSets[this->zPrimarySet].RemoveSetFromRenderer(eng);
		this->zPrimarySet = primary;
		zSets[this->zPrimarySet].AddSetToRenderer(eng);
	}
}

void InGameMenu::ToggleMenu()
{
	this->zEng->GetKeyListener()->KeyUp(VK_ESCAPE);
	this->zPrimarySet = INGAMEMAIN;
	this->zSets[this->zPrimarySet].AddSetToRenderer(this->zEng);

	this->zShow = true;
}

void InGameMenu::Resize()
{
	if(this->zSizedForWidth == this->zEng->GetEngineParameters().WindowWidth &&
		this->zSizedForHeight == this->zEng->GetEngineParameters().WindowHeight)
		return;

	this->zSets[INGAMEMAIN].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)this->zEng->GetEngineParameters().WindowWidth, (float)this->zEng->GetEngineParameters().WindowHeight);
	this->zSets[IGOPTIONS].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)this->zEng->GetEngineParameters().WindowWidth, (float)this->zEng->GetEngineParameters().WindowHeight);

	this->zSizedForWidth = (float)this->zEng->GetEngineParameters().WindowWidth;
	this->zSizedForHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
}
