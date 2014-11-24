/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__  ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/___/\ \_/\  __/ \ \_/
 *         /_\ \ \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \__\_\/_/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
 *                            \ \_\
 *                             \/_/
 *      @file:      game_logic.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 20:06
 *      @brief:     A demo shows how to use Jupiter to make a simple Windows
 *                  game. Have fun! :-)
 */

#include "resource.h"
#include "jupiter.h"
#include "jupiter_sprite.h"
#include "game_logic.h"

Jupiter* kDemoPtr;
long kPreTime;
int kFrequency;

JupiterSprite* kSprBackgroundPtr;
JupiterSprite* kSprManPtr;
JupiterSprite* kSprGirlPtr;
JupiterSprite* kSprSceneTitle;

int kSceneTitleAlpha;
int kManX, kManY, kGirlX, kGirlY;

bool gameInit(HINSTANCE hInstance)
{
    kDemoPtr = new Jupiter(hInstance, TEXT("Demo2"), TEXT("Demo2"),
        NULL, NULL, FALSE, 32, 800, 600);
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
    if (kSceneTitleAlpha < 100) {
        kSceneTitleAlpha += 5;
    }
    kManX += 1;
    kGirlY -= 1;

    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
}

void gamePaint(HDC hDC)
{
    kSprBackgroundPtr->draw(hDC, 0, 0);
    kSprManPtr->draw(hDC, kManX, kManY, true, RGB(0, 0, 0));
    kSprGirlPtr->draw(hDC, kGirlX, kGirlY, true, RGB(0, 0, 0));

    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 255, 0));

    kSprSceneTitle->drawTransAlpha(hDC, 0, 0, 800, 600, 140, 30,
        kSceneTitleAlpha, RGB(0, 0, 0));
}

void gameStart(HWND hWnd)
{
    long cursor = (long)LoadCursor(kDemoPtr->getInstance(), 
        MAKEINTRESOURCE(IDC_CURSOR));
    SetClassLong(hWnd, GCL_HCURSOR, cursor);

    kSprBackgroundPtr = new JupiterSprite(NULL, "background.bmp");
    kSprGirlPtr = new JupiterSprite(NULL, "girl.bmp");
    kSprManPtr = new JupiterSprite(NULL, "foeman.bmp");
    kSprSceneTitle = new JupiterSprite(NULL, "SceneTitle.bmp");

    kSceneTitleAlpha = 0;
    kManX = 600;
    kManY = 200;
    kGirlX = 20;
    kGirlY = 200;
}

void gameEnd()
{
    delete kSprSceneTitle;
    delete kSprManPtr;
    delete kSprGirlPtr;
    delete kSprBackgroundPtr;
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