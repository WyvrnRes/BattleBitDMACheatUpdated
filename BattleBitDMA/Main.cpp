#include "pch.h"
#include "Memory.h"
#include "CheatFunction.h"
#include "PlayerNetwork.h"
#include "Globals.h"
#include "MainCamera.h"
#include "Init.h"
#include "GUI.h"
#include "Manager.h"
#include "ConfigInstance.h"



void main()
{
	if (!TargetProcess.Init("BattleBit.exe"))
	{
		printf("Failed to initialize process\n");
		return;
	}
	TargetProcess.GetBaseAddress("GameAssembly.dll");
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InputWndProc(hWnd, message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, LIT("CONIN$"), LIT("r"), stdin);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stderr);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stdout);
	printf(LIT("Debugging Window:\n"));

	main();
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GUI Framework";
	RegisterClassEx(&wc);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_LAYERED, wc.lpszClassName, L"GUI Framework",
		WS_POPUP,
		0, 0, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return -1;


	ShowWindow(hWnd, nCmdShow);

	Kmbox.KmBoxNetManager::InitDevice(ConfigInstance.Player.Ip, ConfigInstance.Player.Port, ConfigInstance.Player.Uuid);

	InitD2D(hWnd);
	CreateGUI();
	MSG msg;
	SetProcessDPIAware();
	SetInput();
	while (TRUE)
	{
		SetWindowAttributes(hWnd, ConfigInstance.Player.Transparent);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		RenderFrame();
	}
	CleanD2D();
	return msg.wParam;
}
