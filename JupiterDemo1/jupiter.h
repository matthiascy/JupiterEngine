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
 *      @file:      jupiter.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 15:12
 *      @brief:     This file encapsulate basic functions of Win32 API that
 *                  used to build basic Windows applications. You can use it
 *                  to create your own Windows game. Have fun! :-)
 */

#ifndef __JUPITER_H__
#define __JUPITER_H__
#include <Windows.h>

class Jupiter {
protected:
    static Jupiter* ptrJupiter;
    HINSTANCE jupiInst;
    HWND jupiWnd;
    TCHAR jupiWndCls[50];
    TCHAR jupiTitle[50];
    WORD jupiIcon;
    WORD jupiSmIcon;
    int jupiWndWidth, jupiWndHeight;

    bool jupiFullScreen;
    int jupiColorbit;

    int jupiFrameDelay;
    long jupiNextTick;
    long jupiStartTime;
    bool jupiPause;

public:
    Jupiter(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
            WORD wIcon, WORD wSmallIcon, bool bFullScreen = false,
            int nColorbit = 32, int nWidth = 800, int nHeight = 600);
    virtual ~Jupiter();

    bool createJupiterWindow();

    static Jupiter* getJupiter() { return ptrJupiter; };
    HINSTANCE getInstance() { return jupiInst; };
    HWND getWindow() { return jupiWnd; };
    void setWindow(HWND hWnd) { jupiWnd = hWnd; };
    bool getPause() { return jupiPause; };
    void setPause(bool bPause) { jupiPause = bPause; };
    int getFrameDelay() { return jupiFrameDelay; };
    void setFrameDelay(int nFrameDelay) { jupiFrameDelay = nFrameDelay; };
    int getWidth() { return jupiWndWidth; };
    int getHeight() { return jupiWndHeight; };
    long getStartTime() { return jupiStartTime; };
    int getNextTick() { return jupiNextTick; };
    void setNextTick(int nNextGameTick) { jupiNextTick = nNextGameTick; };
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, 
                         LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPTSTR lpCmdline, int nCmdShow);

#endif // __JUPITER_H__