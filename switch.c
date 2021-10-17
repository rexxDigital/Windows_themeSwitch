// I know that pure winapi gui is cancer, will switch to qt or dx rendering when i get the motivation and time
#include <stdio.h>
#include <windows.h>
#include <WinReg.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define ID_TOGGLE 1
#define BUFFER 8192

const char* KEYPATH = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
const char* APPTHEME = "AppsUseLightTheme";
const char* SYSTHEME = "SystemUsesLightTheme";

const char* setTheme()
{
    HKEY hKey;
    DWORD val;
    DWORD holder;
    DWORD buff = BUFFER;

    if(RegCreateKeyEx(HKEY_CURRENT_USER, KEYPATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) != ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "failed to find reg directory.";
    }

    if(RegOpenKeyEx(HKEY_CURRENT_USER, KEYPATH, 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "failed to open reg directory.";
    }

    RegGetValueA(HKEY_CURRENT_USER, KEYPATH, APPTHEME, RRF_RT_DWORD, NULL, (PVOID)&holder, &buff);
    if(holder == 0)
        val = 1;  
    else 
        val = 0;

    if(RegSetValueEx(hKey, APPTHEME, 0, REG_DWORD, (const BYTE*)&val, sizeof(val)) == ERROR_SUCCESS && RegSetValueEx(hKey, SYSTHEME, 0, REG_DWORD, (const BYTE*)&val, sizeof(val)) == ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "Changed theme.";
    }

    RegCloseKey (hKey);
    return "Something went wrong bud.";
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"Theme Switch";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  150, 150, 150, 100, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0))
    {
    
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
    switch(msg) {

        case WM_CREATE:

            CreateWindowW(L"Button", L"Switch Theme",
                WS_VISIBLE | WS_CHILD ,
                15, 15, 100, 25, hwnd, (HMENU) ID_TOGGLE, NULL, NULL);
            break;

        case WM_COMMAND:

            if (LOWORD(wParam) == ID_TOGGLE) {
            
                setTheme();
            }

            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}