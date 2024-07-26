#pragma once
void CreateGUI();
void Render();
void SetWindowAttributes(HWND hWnd, bool Transparent);
extern bool MenuOpen;
extern int TabCount;
extern D2D1::ColorF ColourPickerClipBoard;
extern int KeyBindClipBoard;
