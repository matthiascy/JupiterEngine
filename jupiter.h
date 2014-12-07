/*
 *      Copyright(C) 2014, Jupiter-org.
 *                                        
 *        _______
 *       /\      \                 __     __
 *       \ /-\ \-/                /\ \   /\ \
 *          \ \ \  __   __  _____ \/_/  _\_\ \__  ____  _____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/__/\ \_/\  __/\ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\  \ \_\\ \____\\ \_\
 *         \/______/\/____/_/\ \ \   \/_/   \/_/ \/____/ \/_/
 *                            \ \_\
 *                             \/_/
 *
 *      @file:      jupiter.h
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/11/25 10:32
 *      @history:   2014/11/24 15:12
 *      @brief:     This file encapsulate basic functions of Win32 API that
 *                  used to build basic Windows applications. You can use it
 *                  to create your own Windows game. Declare the game logic
 *                  functions here. Have fun! :-)
 */
#ifndef __JUPITER_H__
#define __JUPITER_H__

#include "windows.h"

class Jupiter {
protected:
    static Jupiter* ptrJupiter;	                                                                 
    HINSTANCE	jupiInst;
    HWND	jupiWnd;
    TCHAR	jupiWndCls[50];
    TCHAR	m_szTitle[50];
    WORD	jupiIcon;
    WORD	jupiSmIcon;
    int		jupiWndWidth, jupiWndHeight;

    bool	jupiFullScreen;
    int		jupiColorbit;

    int		jupiFrameDelay;
    long	jupiNextTick;
    long	jupiStartTime;
    bool	jupiPause;

public:
    Jupiter(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
            WORD wIcon, WORD wSmallIcon, bool bFullScreen = false,	
            int nColorbit = 32, int nWidth = 800, int nHeight = 600);
    virtual ~Jupiter();
    bool createGameWindow();
    static Jupiter * getJupiter() { return ptrJupiter; };
    HINSTANCE getInstance() { return jupiInst; };
    HWND getWindow() { return jupiWnd; };
    void setWindow(HWND hWnd) { jupiWnd=hWnd; };
    bool getPause() { return jupiPause; };
    void setPause(bool bPause) { jupiPause=bPause; };
    int	getFrameDelay() { return jupiFrameDelay; };
    void setFrameDelay(int nFrameDelay) { jupiFrameDelay=nFrameDelay; };
    int	getWidth() { return jupiWndWidth; };
    int	getHeight() { return jupiWndHeight; };
    long getStartTime() { return jupiStartTime; }
    void setNextGameTick(int nNextGameTick) { jupiNextTick=nNextGameTick; }
    int getNextGameTick() { return jupiNextTick; }
};

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,
                   LPSTR lpCmdline, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

bool gameInitialize(HINSTANCE hInstance);
void gameStart(HWND hWnd);
void gameEnd();
void gameAction(HWND hWnd);
void gamePaint(HDC hDC);
void gameActive(HWND hWnd);
void gamePause(HWND hWnd);
bool gameWindowClose(HWND hWnd);
void keyEvent(HWND hWnd);
void mouseLButtonDown(HWND hWnd, int x, int y, WPARAM wParam);
void mouseLButtonUp(HWND hWnd, int x, int y, WPARAM wParam);
void mouseDoubleClick(HWND hWnd, int x, int y, WPARAM wParam);
void mouseRButtonDown(HWND hWnd, int x, int y, WPARAM wParam);
void mouseMove(HWND hWnd, int x, int y, WPARAM wParam);

#endif // __JUPITER_H__
