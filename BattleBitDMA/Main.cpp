#include "pch.h"
#include "Memory.h"
#include "CheatFunction.h"
#include "PlayerNetwork.h"
#include "Globals.h"
#include "MainCamera.h"
#include "Init.h"
#include "GUI.h"
#include "ConfigInstance.h"
#include "Drawing.h"
#include "ConfigUtilities.h"
#include "Manager.h"

void InitializeGame() {
    while (!TargetProcess.Init("BattleBit.exe")) {
        printf("Waiting for Battlebit...\n");
        Sleep(2000);
    }
    TargetProcess.GetBaseAddress("GameAssembly.dll");
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    InputWndProc(hWnd, message, wParam, lParam);
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    WNDCLASSEX wc;

    // Set up console for debugging
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, LIT("CONIN$"), LIT("r"), stdin);
    freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stderr);
    freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stdout);
    printf(LIT("Debugging Window:\n"));

    InitializeGame();

    // Set up window class
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"GUI Framework";
    RegisterClassEx(&wc);

    // Create the window
    hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_LAYERED, wc.lpszClassName, L"GUI Framework",
        WS_POPUP, 0, 0, ConfigInstance.Player.screenX, ConfigInstance.Player.screenY,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);

    // Initialize
    Kmbox.InitDevice(ConfigInstance.Player.Ip, ConfigInstance.Player.Port, ConfigInstance.Player.Uuid);
    InitD2D(hWnd);
    CreateGUI();
    MSG msg;
    SetProcessDPIAware();
    SetInput();
    SetUpConfig();
    LoadConfig(L"Default.json");

    while (TRUE) {
        SetWindowAttributes(hWnd, ConfigInstance.Player.Transparent);

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                break;
            }
        }
        RenderFrame();
    }

    CleanD2D();
    return msg.wParam;
}
