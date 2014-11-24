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
 *      @file:      jupiter_sprite.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 15:36
 *      @brief:     Implementation of JupiterSprite. You need to add additional
 *                  dependencies msimg32.lib, gdi32.lib
 *                  Have fun! :-)
 */

#include "jupiter_sprite.h"

#pragma   comment(lib,"gdi32.lib")
#pragma   comment(lib,"msimg32.lib")

int JupiterSprite::jupiterSpriteID = 1;

JupiterSprite::JupiterSprite()
{
    spriteBmp = NULL;
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;
    spriteZOrder = 0;
}

JupiterSprite::JupiterSprite(HINSTANCE hInstance, LPTSTR szFileName)
{
    load(hInstance, szFileName);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++; 
    spriteZOrder = 0;
}

JupiterSprite::JupiterSprite(HINSTANCE hInstance, UINT uRcID)
{
    load(hInstance, uRcID);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++; 
    spriteZOrder = 0;
}

JupiterSprite::JupiterSprite(HDC hDC, int nWidth, int nHeight, COLORREF crColor)
{
    create(hDC, nWidth, nHeight, crColor);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++; 
    spriteZOrder = 0;
}

JupiterSprite::~JupiterSprite()
{
    release();
}

void JupiterSprite::release()
{
  if (spriteBmp != NULL)
  {
    DeleteObject(spriteBmp);
    spriteBmp = NULL;
  }
}

bool JupiterSprite::create(HDC hDC, int nWidth, int nHeight,
                    COLORREF crColor/* = RGB(0, 0, 0)*/)
{
    release();

    spriteBmp=CreateCompatibleBitmap(hDC, nWidth, nHeight);
    if(spriteBmp == NULL)
        return false;

    spriteWidth = nWidth;  
    spriteHeight = nHeight;

    HDC hdcMem = CreateCompatibleDC(hDC);
    HBRUSH hBrush = CreateSolidBrush(crColor);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, spriteBmp);
    RECT rc = {0, 0, nWidth, nHeight };
    FillRect(hdcMem, &rc, hBrush);

    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(hBrush);

    return TRUE;
}

bool JupiterSprite::load(HINSTANCE hInstance, LPTSTR szFileName)
{
    release();
    spriteBmp = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP,
        0, 0, LR_LOADFROMFILE);
    BITMAP  bm;
    GetObject(spriteBmp, sizeof(BITMAP), &bm);
    spriteWidth = bm.bmWidth;
    spriteHeight = bm.bmHeight;
    return true;
}

// problem in loading file from a resource id
bool JupiterSprite::load(HINSTANCE hInstance, UINT uRcid)
{
    release(); 
    spriteBmp = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(uRcid),IMAGE_BITMAP,
        0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE); 
    BITMAP bm;
    GetObject(spriteBmp, sizeof(BITMAP), &bm);
    spriteWidth = bm.bmWidth;
    spriteHeight = bm.bmHeight;
    return true;
}

void JupiterSprite::draw(HDC hDC, int x, int y, bool bTrans/* = false*/, 
                  COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if (spriteBmp != NULL) {
        HDC hdcMem = CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        if (bTrans)
             TransparentBlt(hDC, x, y, spriteWidth, spriteHeight, hdcMem, 0, 0,
             spriteWidth, spriteHeight, crTrans);
        else
            BitBlt(hDC, x, y, spriteWidth, spriteHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hOldBmp);   
        DeleteObject(hOldBmp);
        DeleteDC(hdcMem);       
    }
}

void JupiterSprite::drawRect(HDC hDC, int x, int y, int nRectX, int nRectY,
                      int nRectWidth, int nRectHeight, 
                      bool bTrans/* = false*/,
                      COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if (spriteBmp != NULL) {
        HDC hdcMem = CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        if (bTrans)
            TransparentBlt(hDC, x, y, nRectWidth, nRectHeight, hdcMem, nRectX,nRectY,nRectWidth, nRectHeight, crTrans);
        else
            BitBlt(hDC, x, y,nRectWidth, nRectHeight, hdcMem, 0,0, SRCCOPY);

        SelectObject(hdcMem, hOldBmp);   
        DeleteDC(hdcMem);               
    }
}

void JupiterSprite::drawAlpha(HDC hDC,int x, int y, int nAlpha)
{
    if (spriteBmp != NULL) {
        HDC hdcMem = CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        BLENDFUNCTION bf;
        bf.BlendOp=AC_SRC_OVER;
        bf.BlendFlags=0;
        bf.SourceConstantAlpha=nAlpha;
        bf.AlphaFormat=0x00;
        AlphaBlend(hDC, x, y, spriteWidth, spriteHeight, hdcMem, 0, 0, getWidth(),
            getHeight(), bf);
        
        SelectObject(hdcMem, hOldBmp);   
        DeleteDC(hdcMem);           
    }
}

void JupiterSprite::drawGray(HDC hDC, int x, int y, bool bTrans, COLORREF crTrans)
{
    if (spriteBmp != NULL) {
        HDC hdcMem= CreateCompatibleDC(hDC);

    
        BITMAP  bm; 
        GetObject(spriteBmp, sizeof(BITMAP), &bm);
        BYTE * px = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
        GetBitmapBits(spriteBmp, bm.bmHeight*bm.bmWidthBytes, px);

    
        int pb = bm.bmBitsPixel/8;
        for (int dy = 0;dy < bm.bmHeight; dy++) {
            for(int dx = 0;dx < bm.bmWidth; dx++) {
                int rgb_b = dy*bm.bmWidthBytes + dx*pb; 
                px[rgb_b] = (px[rgb_b]*299 + px[rgb_b+1]*587 +px[rgb_b+2]*114)
                    / 1000;
                px[rgb_b+1] = px[rgb_b];
                px[rgb_b+2] = px[rgb_b];
            }
        }
        HBITMAP hbmTmp = CreateCompatibleBitmap(hDC, bm.bmWidth, 
            bm.bmHeight);  
        SetBitmapBits(hbmTmp, bm.bmHeight*bm.bmWidthBytes, px);

        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hbmTmp); 

        if (bTrans)
             TransparentBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0,
                bm.bmWidth, bm.bmHeight, crTrans);
        else
            BitBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    
        
        delete[] px;

        SelectObject(hdcMem, hOldBmp);   
        DeleteObject(hbmTmp);
        DeleteDC(hdcMem);
    }
}

void JupiterSprite::drawTransAlpha(HDC hdcBack, int nXBack, int nYBack, 
                            int nWidthBack, int nHeightBack,
                            int nXFore, int nYFore, int nAlpha/* = 100*/,
                            COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if (spriteBmp != NULL) {
        HDC hdcMem = CreateCompatibleDC(hdcBack);

        RECT rBackRect = { nXBack, nYBack, nWidthBack, nHeightBack };
        HBITMAP hbmTmp = CreateCompatibleBitmap(hdcBack, nWidthBack,
            nHeightBack); 

        HBITMAP hbmBk = (HBITMAP)SelectObject(hdcBack, hbmTmp);

        RECT rForeRect = { nXFore, nYFore, nXFore+spriteWidth, nYFore+spriteHeight };
        RECT rTransRect, rTransRect2;

        if (IntersectRect(&rTransRect, &rBackRect, &rForeRect)) {
            BITMAP  bm, bmBk;    
            GetObject(spriteBmp, sizeof(BITMAP),&bm);
            BYTE * px = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
            GetBitmapBits(spriteBmp, bm.bmHeight*bm.bmWidthBytes, px);

            GetObject(hbmBk, sizeof(BITMAP),&bmBk);
            BYTE * pxBk = new unsigned char[bmBk.bmHeight*bmBk.bmWidthBytes]; 

            GetBitmapBits(hbmBk, bmBk.bmHeight*bmBk.bmWidthBytes, pxBk);

            rTransRect2 = rTransRect;
            OffsetRect (&rTransRect2, -rForeRect.left, -rForeRect.top ) ;
            
            int pb = bm.bmBitsPixel / 8;
            int pbBk = bmBk.bmBitsPixel / 8;

            int dx1, dx2, dy1, dy2;
            for(dy1 = rTransRect.top, dy2 = rTransRect2.top; 
                dy1<rTransRect.bottom; dy1++, dy2++)
                for(dx1 = rTransRect.left, dx2 = rTransRect2.left; 
                    dx1 < rTransRect.right; dx1++, dx2++) {
                    int nBackRGB_B = dy1*bmBk.bmWidthBytes+dx1*pbBk;
                    int nForeRGB_B = dy2*bm.bmWidthBytes+dx2*pb;
                    
                    if( RGB(px[nForeRGB_B+2], px[nForeRGB_B+1], 
                        px[nForeRGB_B]) != crTrans) {
                    pxBk[nBackRGB_B] = (px[nForeRGB_B]*nAlpha + 
                        pxBk[nBackRGB_B]*(255-nAlpha)) / 255;
                    pxBk[nBackRGB_B+1] = (px[nForeRGB_B+1]*nAlpha + 
                        pxBk[nBackRGB_B+1]*(255-nAlpha)) / 255;
                    pxBk[nBackRGB_B+2] = (px[nForeRGB_B+2]*nAlpha +
                        pxBk[nBackRGB_B+2]*(255-nAlpha)) / 255;
                    }
                }
            
            SetBitmapBits(hbmTmp, bmBk.bmHeight*bmBk.bmWidthBytes, pxBk);
            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmTmp); 
            BitBlt(hdcBack, 0, 0, rBackRect.right, rBackRect.bottom, hdcMem,
                0, 0, SRCCOPY);
    
            delete[] px;
            delete[] pxBk;
            SelectObject(hdcMem,hbmOld);
        }
    
        DeleteObject(hbmTmp);
        DeleteDC(hdcMem);
    }
}

void JupiterSprite::transBitmap(HDC hdcDest, int nXDest, int nYDest, int nWidthDest,
                         int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc,
                         int nWidthSrc, int nHeightSrc, 
                         COLORREF crTrans/* = RGB(255, 255, 255)*/)
{
    HBITMAP hbmTmp = CreateCompatibleBitmap(hdcDest, nWidthDest,
        nHeightDest);  
    HBITMAP hbmMask = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);    
    HDC     hdcMem = CreateCompatibleDC(hdcDest);
    HDC     hdcMask = CreateCompatibleDC(hdcDest);

    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmTmp);
    HBITMAP hbmMaskOld = (HBITMAP)SelectObject(hdcMask, hbmMask);

    if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
        BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc,
            SRCCOPY);
    else
        StretchBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc,
            nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);

    SetBkColor(hdcMem, crTrans);

    BitBlt(hdcMask, 0, 0, nWidthDest, nHeightDest, hdcMem, 0, 0, SRCCOPY);

    SetBkColor(hdcMem, RGB(0,0,0));
    SetTextColor(hdcMem, RGB(255,255,255));
    BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcMask, 0, 0, SRCAND);

    SetBkColor(hdcDest,RGB(255,255,255));
    SetTextColor(hdcDest,RGB(0,0,0));
    BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMask,
        0, 0, SRCAND);

    BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMem,
        0, 0, SRCPAINT);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    SelectObject(hdcMask, hbmMaskOld);
    DeleteDC(hdcMask);
    DeleteObject(hbmTmp);
    DeleteObject(hbmMask);
}