/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__  ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/___/\ \_/\  __/ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \/______/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
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

#pragma   comment(lib,"user32.lib")
#pragma   comment(lib,"winmm.lib")
#pragma   comment(lib,"gdi32.lib")

Jupiter* Jupiter::ptrJupiter = NULL;

Jupiter::Jupiter(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle, 
                 WORD wIcon, WORD wSmallIcon, bool bFullScreen, int nColorbit,
                 int nWidth, int nHeight)
{
    ptrJupiter = this;
    jupiInst = hInstance;

    lstrcpy(jupiWndCls, szWindowClass);
    lstrcpy(m_szTitle, szTitle);

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

bool Jupiter::createGameWindow()
{
    WNDCLASSEX wcApp;
    HWND hWnd = NULL;

    int nPosX, nPosY, nWndWidth, nWndHeight;
    DWORD dwWinStyle;
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

    DEVMODE DevMode;
    ZeroMemory(&DevMode, sizeof(DevMode));
    DevMode.dmSize = sizeof(DevMode);	

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);

    bool bDisplayChange = false;

    if(DevMode.dmBitsPerPel != (unsigned long)jupiColorbit) {
        DevMode.dmBitsPerPel = jupiColorbit;
        bDisplayChange = true;
    }

    if((jupiFullScreen && jupiWndWidth != GetSystemMetrics(SM_CXSCREEN))	
        || jupiWndWidth > GetSystemMetrics(SM_CXSCREEN)) {
            DevMode.dmPelsWidth = jupiWndWidth;
            DevMode.dmPelsHeight = jupiWndHeight;
            bDisplayChange = true;		
    }

    if(bDisplayChange) {
        LONG result = ChangeDisplaySettings(&DevMode, 0);
        if(result == DISP_CHANGE_SUCCESSFUL) {
            ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
        } else {
            ChangeDisplaySettings(NULL, 0);
            return false;
        }
    }

    if(jupiFullScreen) {
        nPosX=0;
        nPosY=0;
        dwWinStyle = WS_POPUP;
        nWndWidth = jupiWndWidth;
        nWndHeight = jupiWndHeight;	
    } else {
        nWndWidth = jupiWndWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
        nWndHeight = jupiWndHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 10;

        nPosX = (GetSystemMetrics(SM_CXSCREEN) - nWndWidth) / 2;		
        nPosY = (GetSystemMetrics(SM_CYSCREEN) - nWndHeight) / 2;	

        dwWinStyle = WS_SYSMENU | WS_CAPTION;		
    }

    hWnd = CreateWindow(jupiWndCls, m_szTitle, dwWinStyle, nPosX, nPosY,
        nWndWidth, nWndHeight, NULL, NULL, jupiInst, NULL);		

    if(!hWnd)
        return false;
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return true;
}

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, 
                   LPSTR lpCmdline, int nCmdShow)
{
    MSG msg;

    if(!gameInitialize(hInstance))			
        return false;

    while(true) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if (!Jupiter::getJupiter()->getPause()) {
                long nTick = timeGetTime();
                long nNextTick = Jupiter::getJupiter()->getNextGameTick();

                if(nTick > nNextTick) {
                    long nNextGameTick = nTick + Jupiter::getJupiter()->getFrameDelay();
                    Jupiter::getJupiter()->setNextGameTick(nNextGameTick);

                    keyEvent(Jupiter::getJupiter()->getWindow());
                    gameAction(Jupiter::getJupiter()->getWindow());
                }
            }
        }
    }
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
    switch (message) {
        case WM_CREATE:	
            Jupiter::getJupiter()->setWindow(hWnd);
            gameStart(hWnd);
            break;

        case WM_PAINT:
            HDC hDC;
            HDC hMemDC;
            HBITMAP hbmMem, hbmOld;
            PAINTSTRUCT ps;
            hDC=BeginPaint(hWnd, &ps);

            hMemDC=CreateCompatibleDC(hDC);
            hbmMem=CreateCompatibleBitmap(hDC, Jupiter::getJupiter()->getWidth(), 
                Jupiter::getJupiter()->getHeight());
            hbmOld=(HBITMAP)SelectObject(hMemDC, hbmMem);
            gamePaint(hMemDC);
            BitBlt(hDC, 0, 0, Jupiter::getJupiter()->getWidth(), 
                Jupiter::getJupiter()->getHeight(), hMemDC, 0, 0, SRCCOPY);
    
            SelectObject(hMemDC, hbmOld);
            DeleteObject(hbmMem);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
            break;

        case WM_LBUTTONDOWN:
            mouseLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_LBUTTONUP:
            mouseLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_LBUTTONDBLCLK:
            mouseDoubleClick(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_RBUTTONDOWN:
            mouseRButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_MOUSEMOVE:
            mouseMove(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_SETFOCUS:
            gameActive(hWnd);
            Jupiter::getJupiter()->setPause(false);
            break;

        case WM_KILLFOCUS:
            gamePause(hWnd);
            Jupiter::getJupiter()->setPause(true);
            break;

        case WM_CLOSE:
            if (gameWindowClose(hWnd))
                DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            gameEnd();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return false;
}

