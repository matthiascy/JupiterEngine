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
 *      @date:      2014/11/24 16:40
 *      @brief:     A demo shows how to use Jupiter to make a simple Windows
 *                  game. Have fun! :-)
 */

#include "jupiter.h"
#include "game_logic.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPTSTR lpCmdline, int nCmdShow)
{
    MSG msg;
    if (!gameInit(hInstance))
        return false;

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if (!Jupiter::getJupiter()->getPause()) {
                long tick = timeGetTime();
                long nextTick = Jupiter::getJupiter()->getNextTick();

                if (tick > nextTick) {
                    long nextGameTick = tick + Jupiter::getJupiter()->
                        getFrameDelay();
                    Jupiter::getJupiter()->setNextTick(nextGameTick);

                    gameAction(Jupiter::getJupiter()->getWindow());
                }
            }
        }
    }
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, 
                         LPARAM lParam)
{
    switch (message) {
        case WM_CREATE:
            Jupiter::getJupiter()->setWindow(hWnd);
            gameStart(hWnd);
            break;

        case WM_PAINT:
            HDC hDC;
            PAINTSTRUCT ps;
            hDC = BeginPaint(hWnd, &ps);
            gamePaint(hDC);
            EndPaint(hWnd, &ps);
            break;

        case WM_LBUTTONDOWN:
            mouseClick(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
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