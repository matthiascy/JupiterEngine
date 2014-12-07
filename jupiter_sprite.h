/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
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
 *      @file:      jupiter_sprite.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/26 17:52
 *      @history:   2014/11/25 10:06 2014/11/24 23:08, 2014/11/24 18:13 
 *      @brief:     You this powerful class to create your own object in game,
 *                  of course, the animation is included. Cool! isn't it? ;-)
 *                  There are lots of variables, perhaps you need a paper to
 *                  write it down, in case of you can't remember it. After all,
 *                  Have fun! :-)
 */

#ifndef __JUPITER_SPRITE_H__
#define __JUPITER_SPRITE_H__


#include <windows.h>
#include "wingdi.h"
#include "cximagelib\\ximage.h" // CxImage is a powerful image library.

#include "jupiter_physics.h"

// Decides how to draw sprite, simply draw? with alpha? gray type? transparent?
// or even the animation, or just a rectangle, or perhaps through CxImage.
enum DRAWTYPE {
    DT_NULL = 0, DT_draw = 1, DT_ALPHA = 2, DT_GRAY = 3, DT_TRANSALPHA = 4, 
    DT_ANIMA = 5, DT_RECT = 6, DT_CXIMAGE = 7	
};

class JupiterSprite
{
protected:
    // Statistics every sprite you added, signs each one a unique id.
    static int jupiterSpriteID;
    // Current sprite id.
    int spriteID;
    // Handle of the bitmap that sprite holds.
    HBITMAP spriteBmp;
    // Sprite's bitmap width, height, and the Z order.
    int	spriteWidth, spriteHeight;
    // Decides which layer the sprite in.
    int	spriteZOrder;
    // Frame width and height that consists animation in a sprite bitmap.
    int	spriteFrameWidth, spriteFrameHeight;
    // Frame count.
    int	spriteRowCount, spriteColCount;
    // Current sprite frame row and col, same function with i, j in a for loop.
    int spriteRow, spriteCol;
    // Each animation frame delay.
    int	sprFrameDelay;
    // Time to draw next frame.
    unsigned long sprNextFrameTime;
    // Indicates whether the animation is initialized or not.
    bool isInitAni;
    // Indicates whether the animation is finished.
    bool isFiniAni;

    
    // Attributes used to manage sprites.
    DRAWTYPE spriteType;    // Sprite's drawing type.
    int	spriteX, spriteY;   // Position to draw sprite.
    bool spriteTrans;       // Transparent or not.
    COLORREF spriteTransCr; // Color used to transparent sprite.
    int	spriteAlpha;        // Sprite opacity.

    // Variables used by drawTransAlpha() method, background's left-top 
    // position and it's width, height
    int	spriteBackX, spriteBackY, spriteBackWidth, spriteBackHeight;

    // Variables used by drawRect() method, rectangle rigion's left-top 
    // position and it's width, height
    int	spriteRectX, spriteRectY, spriteRectWidth, spriteRectHeight;
    bool spriteVisible; //Sprite is visible or not.


    CxImage* spriteCXImage;    // Point to CxImage object.

public:
    JupiterSprite();
    // According bitmap file name to create sprite.
    JupiterSprite(HINSTANCE hInstance,LPTSTR szFileName);
    // Create sprite according to resource ID.
    JupiterSprite(HINSTANCE hInstance,UINT uRcID);
    // Create sprite according to specified size and color.
    JupiterSprite(HDC hDC, int nWidth, int nHeight, 
        COLORREF crColor = RGB(255,255,255));
    virtual ~JupiterSprite();

    // Return sprite's id number.
    int getID() { return spriteID; 	};
    // Return sprite's bitmap handle
    HBITMAP getBitmp() { return spriteBmp; };
    //  Set sprite's bitmap handle
    void setBitmap(HBITMAP hbm);
    // Get sprite bitmap's width and height.
    int getHeight() { return spriteHeight; }
    int getWidth() { return spriteWidth; }
    // Lay sprite on a proper layer.
    void setZOrder(int nZOrder)	 { spriteZOrder=nZOrder; }
    // Return sprite's z order.
    int getZOrder() { return spriteZOrder; }

    // Release all source that sprite holds.
    void release();

    // Create a bitmap according to assigned width, height and color.
    bool create(HDC hDC, int nWidth, int nHeight, 
                COLORREF crColor = RGB(0,0,0));

    // Load a bitmap from a file.
    // by filename or resource id.
    bool load(HINSTANCE hInstance,LPTSTR szFileName);
    bool load(HINSTANCE hInstance,UINT uRcid);

    // Draw sprite bitmap.
    // @params: x, y -- where to draw sprite bitmap.
    //          bTrans -- indicates whether draw a background transparent bitmap.
    //          crTransparent -- used to transparent background, usually is the
    //          background color.
    void draw(HDC hDC, int x, int y, bool bTrans = false, 
              COLORREF crTransparent = RGB(0,0,0));

    // Draw the rectangle region of a bitmap.
    // @params: x, y -- where to draw sprite bitmap.
    //          nRectX, nRectY -- rectangle position on bitmap.
    //          nRectWidth, nRectHeight -- rectangle size.
    //          bTrans, crTrans -- same with before.
    void drawRect(HDC hDC, int x, int y, int nRectX, int nRectY,
                   int nRectWidth, int nRectHeight, bool bTrans = false, 
                   COLORREF crTrans = RGB(0,0,0));

    // Draw half-transparent bitmap, nAlpha = 0~255.
    // @params: x, y -- where to draw sprite bitmap.
    //          nAlpha -- bitmap transparency.
    void drawAlpha(HDC hDC, int x, int y, int nAlpha);

    // Draw gray bitmap.
    // @params: x, y -- where to draw sprite bitmap.
    //          bTrans, crTrans -- same with before.
    void drawGray(HDC hDC, int x, int y, bool bTrans = false, 
                  COLORREF crTransparent = RGB(0,0,0));	

    // Draw half-transparent, background transparent bitmap.
    void drawTransAlpha( HDC hdcBack,	// background HDC
        int nBackX,			// background left-top corner x
        int nBackY,			// background left-top corner y
        int nBackWidth,                 // background width
        int nBackHeight,                // background height
        int nXFore,			// foreground left-top corner x
        int nYFore,			// foreground left-top corner y
        int nAlpha = 100,		        // opacity
        COLORREF crTrans = RGB(0,0,0));	// color used to transparent background

    // Methods used by animation.
    // Init animation drawing info.
    void initAni(int nRowCount, int nColCount, int nFrameDelay);

    // Draw the (nRow, nCol) animation frame at (x, y).
    void drawRowCol(HDC hDC, int x, int y, int nRow, int nCol,
                    bool bTrans = false, COLORREF crTrans = RGB(0,0,0));

    // Draw animation at (x, y).
    void drawAni(HDC hDC,int x, int y, bool bTrans = false, 
                 COLORREF crTrans = RGB(0,0,0));

    // Methods used by SpriteManage.
    // Set drawAlpha() parameters.
    void setDrawAlphaInfo(int x, int y, int nAlpha = 255);

    // Set draw() parameters.
    void setDrawInfo(int x, int y, bool bTrans = false,
                     COLORREF crTrans = RGB(0,0,0));

    // Set drawAni() parameters.
    void setdrawAniInfo(int x, int y, int nRow, bool bTrans,
                        COLORREF crTrans = RGB(0,0,0));

    // Set drawGray() parameters.
    void setDrawGrayInfo(int x, int y, COLORREF crTrans = RGB(0,0,0));

    // Set drawRect() parameters.
    void setDrawRectInfo(int x, int y, int nRectX, int nRectY, int nRectWidth,
                         int nRectHeight, bool bTrans,
                         COLORREF crTrans = RGB(0,0,0));

    // Set drawTransAlpha() parameters.
    void setDrawTransAlphaInfo(int nBackX, int nBackY, 
                                int nBackWidth, int nBackHeight,
                                int nXFore, int nYFore, int nAlpha = 255,
                                COLORREF crTrans = RGB(0,0,0));

    // Unified drawing interface.
    void drawSprite(HDC hDC);

    // Sprite is visible or not.
    void setVisible(bool bVisible) { spriteVisible=bVisible; }

    // Get sprite is visible or not.	
    bool getVisible() { return spriteVisible; }

    // Methods use CxImage.
    // Create sprite, load image 'szFileName'.
    // @params:     szFileName -- file loaded.
    //              bConvertBmp -- indicates whether the image could be converted.
    JupiterSprite(LPTSTR szFileName, bool bConvertBmp = false);

    // Load image 'szFileName'.
    // @params:     szFileName -- file loaded.
    //              bConvertBmp -- indicates whether the image could be converted.
    bool loadCxImage(LPTSTR szFileName, bool bConvertBmp = false);

    // Get CxImage attributes.
    CxImage * load() const { return spriteCXImage; }
    // Set CxImage attributes.
    void setCxImage(CxImage * image) { spriteCXImage=image; }

    // Draw image at (x, y), and set animation frame to FrameDelay
    void drawCxImage(HDC hDC, int x, int y, int nWidth = 0, int nHeight = 0,
                     int nFrameDelay = 100);

    // Set drawCxImage() parameters.
    void setDrawCxImageInfo(int x, int y, int nWidth = 0, int nHeight = 0, 
                            int nFrameDelay = 100);

    bool getIsFiniAni();
    void setIsFiniAni();
    void setNextFrameTime(int n) {sprNextFrameTime += n;};

private:
    // Method to draw transparent bitmap, DIY, :)
    void transBitmap( HDC hdcDest,      // Destination DC
        int nXDest,			// Destination left-top corner
        int nYDest,	
        int nWidthDest,                 // Destination width.
        int nHeightDest,                // Destination height.
        HDC hdcSrc,                     // Source DC.
        int nXSrc,			// Source origin point.
        int nYSrc,
        int nWidthSrc,                  // Source width.
        int nHeightSrc,                 // Source height.
        COLORREF crTrans = RGB(255,255,255)
        );

};

#endif // __JUPITER_SPRITE_H__