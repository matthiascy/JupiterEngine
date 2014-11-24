/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__  ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/__/\ \ \_/\  __/\ \_/
 *         /_\ \ \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \__\_\/_/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
 *                            \ \_\
 *                             \/_/
 *      @file:      jupiter.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 15:36
 *      @brief:     Implementation of Jupiter. You need to add additional
 *                  dependencies user32.lib, winmm.lib, gdi32.lib.
 *                  Have fun! :-)
 */

#include "jupiter.h"
#include "winuser.h"
#include "mmsystem.h"
#include "wingdi.h"

Jupiter* Jupiter::ptrJupiter = NULL;

Jupiter::Jupiter(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
                 WORD wIcon, WORD wSmallIcon, 
                 bool bFullScreen/* = false*/, int nColorbit/* = 32*/, 
                 int nWidth/* = 800*/, int nHeight/* = 600*/)
{
    ptrJupiter = this;
    jupiInst = hInstance;

    lstrcpy(jupiWndCls, szWindowClass);
    lstrcpy(jupiTitle, szTitle);

    jupiFullScreen = bFullScreen;
    jupiColorbit = nColorbit;
    jupiWndWidth = nWidth;
    jupiWndHeight = nHeight;

    jupiIcon = wIcon;
    jupiSmIcon = wSmallIcon;

    jupiStartTime = timeGetTime();
    jupiNextTick = jupiStartTime;
}

Jupiter::~Jupiter()
{

}

bool Jupiter::createJupiterWindow()
{
    WNDCLASSEX wcApp;
    HWND hWnd = NULL;
    int posX, posY, wndWidth, wndHeight;
    DWORD winStyle;
    wcApp.cbSize = sizeof(wcApp);

    wcApp.lpszClassName = jupiWndCls;
    wcApp.style = CS_HREDRAW;
    wcApp.lpfnWndProc = WndProc;
    wcApp.hInstance = jupiInst;
    wcApp.cbWndExtra = 0;
    wcApp.cbClsExtra = 0;
    wcApp.hIconSm = LoadIcon(jupiInst, MAKEINTRESOURCE(jupiIcon));
    wcApp.hIcon = LoadIcon(jupiInst, MAKEINTRESOURCE(jupiSmIcon));
    wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcApp.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcApp.lpszMenuName = NULL;

    RegisterClassEx(&wcApp);

    DEVMODE devMode;
    ZeroMemory(&devMode, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

    bool displayChange = false;

    if (devMode.dmBitsPerPel != (unsigned long)jupiColorbit) {
        devMode.dmBitsPerPel = jupiColorbit;
        displayChange = true;
    }

    if ((jupiFullScreen && jupiWndWidth != GetSystemMetrics(SM_CXSCREEN))
        || jupiWndWidth > GetSystemMetrics(SM_CXSCREEN)) {
        devMode.dmPelsWidth = jupiWndWidth;
        devMode.dmPelsHeight = jupiWndHeight;
        displayChange = true;
    }

    if (displayChange) {
        LONG result = ChangeDisplaySettings(&devMode, 0);
        if (result == DISP_CHANGE_SUCCESSFUL) {
            ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
        } else {
            ChangeDisplaySettings(NULL, 0);
            return false;
        }
    }

    if (jupiFullScreen) {
        posX = 0;
        posY = 0;
        winStyle = WS_POPUP;
        wndWidth = jupiWndWidth;
        wndHeight = jupiWndHeight;
    } else {
        wndWidth = jupiWndWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
        wndHeight = jupiWndHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 10;

        posX = (GetSystemMetrics(SM_CXSCREEN) - wndWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - wndHeight) / 2;

        winStyle = WS_SYSMENU | WS_CAPTION;
    }

    hWnd = CreateWindow(jupiWndCls, jupiTitle, winStyle,
                        posX, posY, wndWidth, wndHeight, NULL, NULL,
                        jupiInst, NULL);
    if (!hWnd)
        return false;
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return true;
}
