#include <stdio.h>
#include <windows.h>
#include <WinReg.h>

const char* KEYPATH = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
const char* APPTHEME = "AppsUseLightTheme";
const char* SYSTHEME = "SystemUsesLightTheme";

const char* setTheme(DWORD val)
{
    HKEY hKey;

    LONG cResult = RegCreateKeyEx(HKEY_CURRENT_USER, KEYPATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    if(cResult != ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "failed to find reg directory.";
    }

    LONG oResult = RegOpenKeyEx(HKEY_CURRENT_USER, KEYPATH, 0, KEY_SET_VALUE, &hKey);
    if(oResult != ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "failed to open reg directory.";
    }

    if(RegSetValueEx(hKey, APPTHEME, 0, REG_DWORD, (const BYTE*)&val, sizeof(val)) == ERROR_SUCCESS && RegSetValueEx(hKey, SYSTHEME, 0, REG_DWORD, (const BYTE*)&val, sizeof(val)) == ERROR_SUCCESS)
    {
        RegCloseKey (hKey);
        return "Changed theme.";
    }

    RegCloseKey (hKey);
    return "Something went wrong bud.";
}

int main(int argc, char* argv[])
{
    while(1)
    {
        DWORD answer;

        puts("1 for lightmode, 0 for darkmode");
        puts("Ctrl+C to exit\n");

        printf("> ");
        scanf("%d", &answer);
        printf("%s\n", setTheme(answer));
    }
    return 0;
}