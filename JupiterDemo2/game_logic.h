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
 *      @file:      game_logic.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 20:06
 *      @brief:     A demo test JupiterSprite.
 *                  Have fun! :-)
 */

#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

//#include "jupiter.h"
#include <Windows.h>

//Jupiter* kDemoPtr;
//long kPreTime;
//int kFrequency;

bool gameInit(HINSTANCE hInstance);
void gameStart(HWND hWnd);
void gameEnd();
void gameAction(HWND hWnd);
void mouseClick(HWND hWnd, int x, int y, WPARAM wParam);
void gamePaint(HDC hDC);
void gameActive(HWND hWnd);
void gamePause(HWND hWnd);
bool gameWindowClose(HWND hWnd);

#endif // __GAME_LOGIC_H__