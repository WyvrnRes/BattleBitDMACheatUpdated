#include "pch.h"
#include "GUI.h"
#include "entity.h"
#include "Form.h"
#include "Button.h"
#include "ColourPicker.h"
#include "Label.h"
#include "tab.h"
#include "TabController.h"
#include "Toggle.h"
#include "Slider.h"
#include "DropDown.h"
#include "ComboBox.h"
#include "KeyBind.h"
#include "TabListBox.h"
#include "TabListBoxController.h"
#include "TextBox.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "PlayerConfig.h"
#include "Manager.h"

int SelectedTab = 1;
int SelectedSubTab = 0;
int TabCount = 0;
int KeyBindClipBoard = 0;
EntityVector MenuEntity;
bool MenuOpen = true;
D2D1::ColorF ColourPickerClipBoard = Colour(255,255,255);
D2D1::ColorF ColourPick = Colour(0, 150, 255, 255);

void CreateGUI()
{
	MenuEntity = std::make_shared<Container>();
	auto form = std::make_shared<Form>(100, 100.0f, 400, 600, 2, 30, LIT(L"Battlebit DMA - Res :3"), false);
	{
		auto health = std::make_shared<Toggle>(10, 10, LIT(L"Health"), &ConfigInstance.Player.Health);
		auto colourpicker = std::make_shared<ColourPicker>(80, 10, &ConfigInstance.Player.TextColour);
		auto distance = std::make_shared<Toggle>(10, 30, LIT(L"Distance"), &ConfigInstance.Player.Distance);
		auto primary = std::make_shared<Toggle>(10, 50, LIT(L"Primary Weapon"), &ConfigInstance.Player.PrimaryWeapon);
		auto circle = std::make_shared<Toggle>(10, 70, LIT(L"Circle"), &ConfigInstance.Player.Circle);
		auto maxdistance = std::make_shared<Slider<int>>(10, 90, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1500, &ConfigInstance.Player.MaxDistance);
		auto fontsize = std::make_shared<Slider<int>>(10, 115, 150, LIT(L"Font Size"), LIT(L"px"), 1, 16, &ConfigInstance.Player.FontSize);
		auto circlesize = std::make_shared<Slider<int>>(10, 140, 150, LIT(L"Circle Size"), LIT(L"px"), 1, 16, &ConfigInstance.Player.CircleSize);
		auto transparent = std::make_shared<Toggle>(10, 170, LIT(L"Transparency"), &ConfigInstance.Player.Transparent);
		auto lines = std::make_shared<Toggle>(10, 190, LIT(L"Lines"), &ConfigInstance.Player.Lines);
		auto linesorigin = std::make_shared<DropDown>(260, 190, LIT(L"Lines Origin"), &ConfigInstance.Player.LineOrigin, std::vector<std::wstring>{LIT(L"Top"), LIT(L"Middle"), LIT(L"Bottom")});
		auto espcircle = std::make_shared<Toggle>(10, 210, LIT(L"Esp Circle"), &ConfigInstance.Player.EspCircle);
		auto espcircleshow = std::make_shared<Toggle>(170, 230, LIT(L"Show?"), &ConfigInstance.Player.EspCircleShow);
		auto espcirclesize = std::make_shared<Slider<int>>(10, 230,150, LIT(L"Esp Circle Size"),LIT(L"px"), 10, 800, &ConfigInstance.Player.EspCircleSize);
		auto aimbot = std::make_shared<Toggle>(10, 260, LIT(L"Aimbot"), &ConfigInstance.Player.Aimbot);
		auto aimkey = std::make_shared<KeyBind>(260, 260, LIT(L"Aimkey"), &ConfigInstance.Player.Aimkey);
		auto fov = std::make_shared<Slider<int>>(10, 280, 150, LIT(L"Aimbot Fov"), LIT(L"px"), 10, 800, &ConfigInstance.Player.FOV);
		auto showaimbotfov = std::make_shared<Toggle>(170, 280, LIT(L"Show?"), &ConfigInstance.Player.ShowAimbotFov);
		auto smoothingx = std::make_shared<Slider<int>>(10, 310, 150, LIT(L"Smoothing X"), LIT(L"px"), 1, 100, &ConfigInstance.Player.AimbotXSmooth);
		auto smoothingy = std::make_shared<Slider<int>>(10, 345, 150, LIT(L"Smoothing Y"), LIT(L"px"), 1, 100, &ConfigInstance.Player.AimbotYSmooth);

		
		auto saveconfig = std::make_shared<Button>(10, 520, LIT(L"Save"), []()
			{
				SaveConfig(L"Default.json");
				CreateGUI();
			});
		auto loadconfig = std::make_shared<Button>(70, 520, LIT(L"Load"), []()
			{
				LoadConfig(L"Default.json");
				CreateGUI();
			});
		
		auto kmboxtest = std::make_shared<Button>(130, 520, LIT(L"Test Kmbox Net"), []()
			{
				Kmbox.Mouse.Move(100, 100);
			});
		
		auto info = std::make_shared<Label>(LIT(L"WinKey + Shift + Arrow To Change Monitor"), 10, 550);

		form->Push(health);
		form->Push(colourpicker);
		form->Push(distance);
		form->Push(primary);
		form->Push(circle);
		form->Push(maxdistance);
		form->Push(fontsize);
		form->Push(circlesize);
		form->Push(transparent);
		form->Push(lines);
		form->Push(linesorigin);
		form->Push(espcircle);
		form->Push(espcircleshow);
		form->Push(espcirclesize);
		form->Push(saveconfig);
		form->Push(loadconfig);
		form->Push(kmboxtest);
		form->Push(aimbot);
		form->Push(aimkey);
		form->Push(fov);
		form->Push(smoothingx);
		form->Push(smoothingy);
		form->Push(showaimbotfov);
		form->Push(info);

	}

	MenuEntity->Push(form);
	MenuEntity->Draw();
	MenuEntity->Update();
}

void SetWindowAttributes(HWND hWnd, bool Transparent)
{
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if (Transparent == false)
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	else
	{
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	}
}


void SetFormPriority()
{
	// This sorts the host container (containerptr) which contains forms, as long as a form isn't parented to another form then this will allow it to draw over when clicked.
	// I swear to god if i need to make this work for forms inside forms for some odd reason in the future then i am going to throw a monitor out the window.
	std::sort(MenuEntity->GetContainer().begin(), MenuEntity->GetContainer().end(),
	          [](child a, child b) { return b->GetLastClick() < a->GetLastClick(); }
	);
}

float LastOpen = 0;

void Render()
{
	if (IsKeyClicked(VK_INSERT) && LastOpen < clock() * 0.00001f)
	{
		LastOpen = (clock() * 0.00001f) + 0.002f;
		MenuOpen = !MenuOpen;
	}

	MenuEntity->Draw();
	MenuEntity->GetContainer()[0]->Update(); // only allow stretching,dragging and other update stuff if it is the main form, prevents dragging and sizing the wrong forms.
	SetFormPriority();
}
