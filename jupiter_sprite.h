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
 *      @file:      jupiter_sprite.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 18:13
 *      @brief:     This is a Sprite class, cool! I can't wait to use
 *                  it. Have fun! :-)
 */

#ifndef __JUPITER_SPRITE_H__
#define __JUPITER_SPRITE_H__

#include <windows.h>
#include <wingdi.h>

class JupiterSprite {
protected:
    static int jupiterSpriteID;
    int spriteID;
    HBITMAP spriteBmp;
    int spriteWidth, spriteHeight;
    int spriteZOrder;

public:
    JupiterSprite();
    JupiterSprite(HINSTANCE hInstance, LPTSTR szFileName);
    Sprite(HINSTANCE hInstance, UINT uRcID);
    Sprite(HDC hDC, int nWidth, int nHeight, 
        COLORREF crColor = RGB(255, 255, 255));
    virtual ~JupiterSprite();

    int getID() { return spriteID; };
    HBITMAP getBitmap() { return spriteBmp; };
    void setBitmap(HBITMAP hbm)
    {
        if (spriteBmp != NULL)
            release();
        spriteBmp = hbm;
    }

    int getHeight() { return spriteHeight; };
    int getWidth() { return spriteWidth; };
    void setZOrder(int nZOrder) { spriteZOrder = nZOrder; };
    int getZOrder() { return spriteZOrder; };

    void release();
    bool create(HDC, int nWidth, int nHeight, 
        COLORREF crColor = RGB(0, 0, 0));
    bool load(HINSTANCE hInstance, LPTSTR szFileName);
    bool load(HINSTANCE hInstance, UINT uRcID);
    void draw(HDC hDC, int x, int y, bool bTrans = false,
        COLORREF crTransparent = RGB(0, 0, 0));
    void drawRect(HDC hDC, int x, int y, int nRectX, int nRectY,
        int nRectWidth, int nRectHeight, bool bTrans = false,
        COLORREF crTrans = RGB(0, 0, 0));
    void drawAlpha(HDC hDC, int x, int y, int nAlpha);
    void drawGray(HDC hDC, int x, int y, bool bTrans = false,
        COLORREF crTransparent = RGB(0, 0, 0));
    void drawTransAlpha(HDC hdcBack, int nXBack, int nYBack,
        int nWidthBack, int nHeightBack, int nXFore, int nYFore,
        int nAlpha = 100, COLORREF crTrans = RGB(0, 0, 0));

private:
    void TransBitmap(HDC hdcDest, int nXDest, int nYDest, 
        int nWidthDest, int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc,
        int nWidthSrc, int nHeightSrc, COLORREF crTrans = RGB(255, 255, 255));
};

#endif // __JUPITER_SPRITE_H__