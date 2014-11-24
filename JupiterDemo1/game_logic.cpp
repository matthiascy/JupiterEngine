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
 *      @file:      demo1.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 17:10
 *      @brief:     A demo shows how to use Jupiter to make a simple Windows
 *                  game. Have fun! :-)
 */

#include "game_logic.h"
#include "resource.h"
#include "jupiter.h"

Jupiter* kDemoPtr;
long kPreTime;
int kFrequency;

bool gameInit(HINSTANCE hInstance)
{
    kDemoPtr = new Jupiter(hInstance, TEXT("Demo1"), TEXT("Demo1"),
        IDI_BIGICON, IDI_SMALLICON, FALSE, 32, 800, 600);
    if (kDemoPtr == NULL)
        return false;

    kDemoPtr->setFrameDelay(15);
    kDemoPtr->setPause(false);
    kDemoPtr->createJupiterWindow();

    return true;
}

void gameAction(HWND hWnd)
{
    long tick = timeGetTime();
    kFrequency = 1000 / (tick - kPreTime);
    kPreTime = tick;

    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
}

void gamePaint(HDC hDC)
{

}

void gameStart(HWND hWnd)
{
    long cursor = (long)LoadCursor(kDemoPtr->getInstance(), 
        MAKEINTRESOURCE(IDC_CURSOR));
    SetClassLong(hWnd, GCL_HCURSOR, cursor);
}

void gameEnd()
{
    delete kDemoPtr;
}

void gameActive(HWND hWnd)
{
    kDemoPtr->setPause(false);
}

void gamePause(HWND hWnd)
{
    kDemoPtr->setPause(true);
}

bool gameWindowClose(HWND hWnd)
{
    if (MessageBox(kDemoPtr->getWindow(), "Sure to quit? T_T", "Quit", 
        MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES)
        return true;
    else
        return false;
}

void mouseClick(HWND hWnd, int x, int y, WPARAM wParam)
{

}