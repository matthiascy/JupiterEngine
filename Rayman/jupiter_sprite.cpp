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
 *      @file:      jupiter_sprite.cpp
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/12/02 23:08
 *      @history    2014/11/25 10:09 2014/11/24 15:36
 *      @brief:     Implementation of JupiterSprite. You need to add additional
 *                  dependencies msimg32.lib, gdi32.lib, winmm.lib. 
 *                  Add animation process. Have fun! :-)
 */

#include "jupiter_sprite.h"
#include "cximagelib\\ximage.h"
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"cximagelib\\cximage.lib")
#pragma comment(lib,"cximagelib\\cximage.lib")  
#pragma comment(lib,"cximagelib\\png.lib")  
#pragma comment(lib,"cximagelib\\jasper.lib")  
#pragma comment(lib,"cximagelib\\jbig.lib")  
#pragma comment(lib,"cximagelib\\Jpeg.lib")   
#pragma comment(lib,"cximagelib\\libdcr.lib")  
#pragma comment(lib,"cximagelib\\mng.lib")  
#pragma comment(lib,"cximagelib\\Tiff.lib")  
#pragma comment(lib,"cximagelib\\zlib.lib")

int JupiterSprite::jupiterSpriteID = 1;

JupiterSprite::JupiterSprite()
{
    spriteBmp = NULL;
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;
    spriteZOrder = 0;
    spriteVisible = true;
}

JupiterSprite::JupiterSprite(HINSTANCE hInstance, LPTSTR szFileName)
{
    load(hInstance, szFileName);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;	
    spriteZOrder = 0;
    spriteVisible = true;
}

JupiterSprite::JupiterSprite(HINSTANCE hInstance, UINT uRcID)
{
    load(hInstance, uRcID);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;
    spriteZOrder = 0;
    spriteVisible = true;
}

JupiterSprite::JupiterSprite(HDC hDC, int nWidth, int nHeight, COLORREF crColor)
{
    create(hDC, nWidth, nHeight, crColor);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;
    spriteZOrder = 0;
    spriteVisible = true;
}

JupiterSprite::~JupiterSprite()
{
    release();
}

void JupiterSprite::release()
{
    if (spriteBmp != NULL) {
        DeleteObject(spriteBmp);
        spriteBmp = NULL;
    }
}


void JupiterSprite::setBitmap(HBITMAP hbm)
{
    if(spriteBmp != NULL)
        release();

    spriteBmp = hbm;

    BITMAP  bm;
    GetObject(spriteBmp, sizeof(BITMAP), &bm);
    spriteWidth = bm.bmWidth;
    spriteHeight = bm.bmHeight;
}

bool JupiterSprite::create(HDC hDC, int nWidth, int nHeight,
                           COLORREF crColor/* = RGB(0,0,0)*/)
{
    release();

    // Create compatible bitmap
    spriteBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    if(spriteBmp == NULL)
        return false;

    // Set width and height.
    spriteWidth = nWidth;	
    spriteHeight = nHeight;

    // Set bitmap by fill rectangle region of bitmap
    HDC hdcMem = CreateCompatibleDC(hDC);	// Create compatible DC.
    HBRUSH hBrush = CreateSolidBrush(crColor);	// Define signal color brush.
    // Select bitmap into DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, spriteBmp);
    RECT rc = {0, 0, nWidth, nHeight };	        // Define rectangle size.
    FillRect(hdcMem, &rc, hBrush);		// Draw filled rectangle

    // Replace out the bitmap, thus could be saved.
    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(hBrush);

    return true;
}

bool JupiterSprite::load(HINSTANCE hInstance, LPTSTR szFileName)
{
    release();
    spriteBmp = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, 
        LR_LOADFROMFILE);
    BITMAP  bm;
    GetObject(spriteBmp, sizeof(BITMAP), &bm);
    spriteWidth = bm.bmWidth;
    spriteHeight = bm.bmHeight;
    return true;
}

bool JupiterSprite::load(HINSTANCE hInstance, UINT uRcid)
{
    release();
    spriteBmp = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(uRcid), 
        IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);	
    BITMAP  bm;
    GetObject(spriteBmp, sizeof(BITMAP), &bm);
    spriteWidth = bm.bmWidth;
    spriteHeight = bm.bmHeight;
    return true;
}

void JupiterSprite::draw(HDC hDC, int x, int y, bool bTrans/* = false*/, 
                         COLORREF crTrans/* = RGB(0,0,0)*/)
{
    if(!spriteVisible)
        return;
    if (spriteBmp != NULL)
    {
        HDC hdcMem= CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        if (bTrans)
            TransparentBlt(hDC, x, y, spriteWidth, spriteHeight, hdcMem, 0 ,0,
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
                             COLORREF crTrans/* = RGB(0,0,0)*/)
{
    if(!spriteVisible)
        return;
    if (spriteBmp != NULL) {
        HDC hdcMem = CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        if (bTrans)
            TransparentBlt(hDC, x, y, nRectWidth, nRectHeight, hdcMem, 
                nRectX, nRectY, nRectWidth, nRectHeight, crTrans);
        else
            BitBlt(hDC, x, y, nRectWidth, nRectHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem,hOldBmp);	
        DeleteDC(hdcMem);				
    }
}

void JupiterSprite::drawAlpha(HDC hDC, int x, int y, int nAlpha)
{
    if(!spriteVisible)
        return;
    if (spriteBmp != NULL)
    {
        HDC hdcMem = CreateCompatibleDC(hDC);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, spriteBmp);

        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = nAlpha;
        bf.AlphaFormat = 0x00;
        AlphaBlend(hDC, x, y, spriteWidth, spriteHeight, hdcMem, 0, 0, 
            getWidth(), getHeight(), bf);

        SelectObject(hdcMem,hOldBmp);	
        DeleteDC(hdcMem);			
    }
}

void JupiterSprite::drawTransAlpha(HDC hdcBack, int nXBack, int nBackY, 
                                   int nBackWidth, int nBackHeight, 
                                   int nXFore, int nYFore, 
                                   int nAlpha/* = 100*/, 
                                   COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if(!spriteVisible)
        return;
    if (spriteBmp != NULL && nAlpha >0) {
        HDC hdcMem= CreateCompatibleDC(hdcBack);

        // Create a bitmap that has same size with background
        RECT rBackRect={nXBack,nBackY, nBackWidth, nBackHeight};
        HBITMAP	hbmTmp=CreateCompatibleBitmap(hdcBack ,nBackWidth, nBackHeight); 

        // Replace out background.
        HBITMAP hbmBk=(HBITMAP)SelectObject(hdcBack,hbmTmp);

        RECT rForeRect={nXFore, nYFore, nXFore+spriteWidth, nYFore+spriteHeight};
        RECT rTransRect, rTransRect2;
 
        // Get for/background rectangle, if doesn't intersect, don't calculate color
        if(IntersectRect(&rTransRect,&rBackRect, &rForeRect)) {
            BITMAP  bm, bmBk;	
            GetObject(spriteBmp, sizeof(BITMAP),&bm);
            BYTE * px = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
            GetBitmapBits(spriteBmp, bm.bmHeight*bm.bmWidthBytes, px);

            GetObject(hbmBk, sizeof(BITMAP),&bmBk);
            BYTE * pxBk = new unsigned char[bmBk.bmHeight*bmBk.bmWidthBytes];

            GetBitmapBits(hbmBk, bmBk.bmHeight*bmBk.bmWidthBytes, pxBk);

            rTransRect2=rTransRect;
            OffsetRect (&rTransRect2, -rForeRect.left, -rForeRect.top );

            int pb = bm.bmBitsPixel / 8;	// Get pixel size
            int pbBk = bmBk.bmBitsPixel / 8;

            //dy1 - background pixel column, dy2 - foreground pixel row
            //dx1 - background pixel row, dx2 - foreground pixel column
            int dx1,dx2,dy1,dy2;	
            for(dy1 = rTransRect.top, dy2 = rTransRect2.top; 
                    dy1 < rTransRect.bottom; dy1++, dy2++) {
                for(dx1 = rTransRect.left, dx2 = rTransRect2.left; 
                    dx1 < rTransRect.right; dx1++, dx2++) {
                        // Calculate fore/background init pixel position
                        int nBackRGB_B = dy1*bmBk.bmWidthBytes + dx1*pbBk;
                        int nForeRGB_B = dy2*bm.bmWidthBytes + dx2*pb;

                        if( RGB(px[nForeRGB_B+2], px[nForeRGB_B+1], 
                            px[nForeRGB_B])!=crTrans) {
                            // RGB_B * nAlpha + px
                            pxBk[nBackRGB_B] = (px[nForeRGB_B]*nAlpha+pxBk[nBackRGB_B]*(255-nAlpha))/255;
                            // RGB_G * nAlpha + px
                            pxBk[nBackRGB_B+1] =(px[nForeRGB_B+1]*nAlpha+pxBk[nBackRGB_B+1]*(255-nAlpha))/255;
                            // RGB_R * nAlpha + px
                            pxBk[nBackRGB_B+2] = (px[nForeRGB_B+2]*nAlpha+pxBk[nBackRGB_B+2]*(255-nAlpha))/255;
                        }
                }
            }
                // Set background values.
                SetBitmapBits(hbmTmp,bmBk.bmHeight*bmBk.bmWidthBytes,pxBk);
                HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem,hbmTmp); 
                BitBlt(hdcBack,0,0,rBackRect.right,rBackRect.bottom,hdcMem,0,0,SRCCOPY);

                delete[] px;
                delete[] pxBk;
                SelectObject(hdcMem,hbmOld);
        }

        DeleteObject(hbmTmp);
        DeleteDC(hdcMem);
    }
}

void JupiterSprite::drawGray(HDC hDC, int x, int y, bool bTrans/* = false*/, 
                             COLORREF crTrans/* = RGB(0,0,0)*/)
{
    if(!spriteVisible)
        return;
    if (spriteBmp != NULL) {
        HDC hdcMem= CreateCompatibleDC(hDC);

        BITMAP  bm;	
        GetObject(spriteBmp,sizeof(BITMAP),&bm);
        // Temporary store array.
        BYTE* px = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
        // Get source bitmap each pixel's RGB values and stores in px;
        GetBitmapBits(spriteBmp,bm.bmHeight*bm.bmWidthBytes,px);

        int pb = bm.bmBitsPixel / 8;
        // Calculate gray value.
        for(int dy = 0; dy < bm.bmHeight; dy++) {
            for(int dx = 0; dx < bm.bmWidth; dx++) {
                int rgb_b = dy*bm.bmWidthBytes + dx*pb;	
                px[rgb_b] = (px[rgb_b]*299 + px[rgb_b+1]*587 + px[rgb_b+2]*114)/1000;
                px[rgb_b + 1] = px[rgb_b] ;
                px[rgb_b + 2] = px[rgb_b] ;
            };
        }
        // Create temp bitmap to store gray bitmap
        HBITMAP	hbmTmp = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight); 	
        SetBitmapBits(hbmTmp, bm.bmHeight*bm.bmWidthBytes, px);

        HBITMAP hOldBmp =( HBITMAP)SelectObject(hdcMem, hbmTmp); 

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

void JupiterSprite::transBitmap( HDC hdcDest, int nXDest, int nYDest,
                                int nWidthDest, int nHeightDest,
                                HDC hdcSrc, int nXSrc, int nYSrc,
                                int nWidthSrc, int nHeightSrc,
                                COLORREF crTrans/* = RGB(255,255,255)*/)
{
    // Create compatible bitmap.
    HBITMAP hbmTmp = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	
    // Create single color bitmap.
    HBITMAP hbmMask = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);	
    // Create compatible DC.
    HDC		hdcMem = CreateCompatibleDC(hdcDest);
    HDC		hdcMask = CreateCompatibleDC(hdcDest);

    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmTmp);
    HBITMAP hbmMaskOld = (HBITMAP)SelectObject(hdcMask, hbmMask);

    // Copy source DC into temp DC.
    if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
        BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc, SRCCOPY);
    else
        StretchBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, 
        hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);

    // Set transparent color.
    SetBkColor(hdcMem, crTrans);

    // Get mask bitmap.
    BitBlt(hdcMask, 0, 0, nWidthDest, nHeightDest, hdcMem, 0, 0, SRCCOPY);

    // Get black background bitmap.
    SetBkColor(hdcMem, RGB(0,0,0));
    SetTextColor(hdcMem, RGB(255,255,255));
    BitBlt(hdcMem, 0, 0, nWidthDest, nHeightDest, hdcMask, 0, 0, SRCAND);

    // Background correspondent to transparent part unchanged, other is black.
    SetBkColor(hdcDest,RGB(255,255,255));
    SetTextColor(hdcDest,RGB(0,0,0));
    BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMask, 0, 0, SRCAND);

    // OR 
    BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMem, 0, 0, SRCPAINT);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    SelectObject(hdcMask, hbmMaskOld);
    DeleteDC(hdcMask);
    DeleteObject(hbmTmp);
    DeleteObject(hbmMask);
}

void JupiterSprite::initAni(int nRowCount, int nColCount, int nFrameDelay)
{
    if (spriteBmp == NULL )
        return;
    spriteRowCount = nRowCount; // Row number.
    spriteColCount = nColCount;	// Column number.

    spriteFrameWidth = spriteWidth / nColCount;		// Frame width.
    spriteFrameHeight = spriteHeight / nRowCount;		// Frame height.
    sprNextFrameTime = nFrameDelay + timeGetTime();	// Time to draw next frame.
    sprFrameDelay = nFrameDelay;

    spriteCol = 0;
    spriteRow = 0;

    isInitAni=true; // Initialized.
}

void JupiterSprite::drawAni(HDC hDC, int x, int y, bool bTrans/* = false*/,
                            COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if(!spriteVisible)
        return;
    if(spriteBmp == NULL || !isInitAni)
        return;
    if(timeGetTime()>sprNextFrameTime)	// Next frame time?
    {	// Yes, reset next frame time.
        sprNextFrameTime = timeGetTime()+sprFrameDelay;	

        spriteCol++;						
        if(spriteCol >= spriteColCount)	
            spriteCol = 0;
    }
    drawRowCol(hDC, x, y, spriteRow, spriteCol, bTrans, crTrans);
};

void JupiterSprite::drawRowCol(HDC hDC, int x, int y, int nRow, int nCol,
                               bool bTrans/* = false*/,
                               COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    if(!spriteVisible)
        return;
    if (spriteBmp == NULL || !isInitAni)
        return;
    HDC hdcMem= CreateCompatibleDC(hDC);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, spriteBmp);

    if (bTrans)
        TransparentBlt(hDC, x, y,spriteFrameWidth, spriteFrameHeight, hdcMem,
            spriteFrameWidth*nCol, spriteFrameHeight*nRow, spriteFrameWidth,
            spriteFrameHeight, crTrans);
    else		
        BitBlt(hDC, x, y,spriteFrameWidth, spriteFrameHeight, hdcMem,
            spriteFrameWidth*nCol, spriteFrameHeight*nRow, SRCCOPY);

    SelectObject(hdcMem,hbmOld);
    DeleteObject(hbmOld);
    DeleteDC(hdcMem);		
}

void JupiterSprite::setDrawAlphaInfo(int x, int y, int nAlpha/* = 255*/)
{
    spriteType = DT_ALPHA;
    spriteX = x;
    spriteY = y;
    spriteAlpha = nAlpha;
}

void JupiterSprite::setDrawInfo(int x, int y, bool bTrans/* = false*/,
                                COLORREF crTrans/* = RGB(0, 0, 0)*/)
{
    spriteType = DT_draw;
    spriteX = x;
    spriteY = y;
    spriteTrans = bTrans;
    spriteTransCr = crTrans;
}

void JupiterSprite::setdrawAniInfo(int x, int y, int nRow, bool bTrans, 
                                   COLORREF crTrans/* = RGB(0, 0, 0)*/)                  
{
    spriteType = DT_ANIMA;
    spriteX = x;
    spriteY = y;
    spriteRow = nRow;
    spriteTrans = bTrans;
    spriteTransCr = crTrans;
}

void JupiterSprite::setDrawGrayInfo(int x, int y, 
                                    COLORREF crTrans/* = RGB(0,0,0)*/)                  
{
    spriteType = DT_GRAY;
    spriteX = x;
    spriteY = y;
    spriteTrans = true;
    spriteTransCr = crTrans;
}

void JupiterSprite::setDrawRectInfo(int x, int y, int nRectX, int nRectY, 
                                    int nRectWidth, int nRectHeight, 
                                    bool bTrans, 
                                    COLORREF crTrans/* = RGB(0,0,0)*/)
{
    spriteType = DT_RECT;
    spriteX = x;
    spriteY = y;
    spriteRectX = nRectX;
    spriteRectY = nRectY;
    spriteRectWidth = nRectWidth;
    spriteRectHeight = nRectHeight;
    spriteTrans = bTrans;
    spriteTransCr = crTrans;
}	

void JupiterSprite::setDrawTransAlphaInfo(int nXBack, int nBackY,
                                          int nBackWidth, int nBackHeight,
                                          int nXFore, int nYFore,
                                          int nAlpha,
                                          COLORREF crTrans/* = RGB(0,0,0)*/)
{
    spriteType = DT_TRANSALPHA;
    spriteX = nXFore;
    spriteY = nYFore;
    spriteBackX = nXBack;
    spriteBackY = nBackY;
    spriteBackWidth = nBackWidth;
    spriteBackHeight = nBackHeight;
    spriteAlpha = nAlpha;
    spriteTransCr = crTrans;
}

void JupiterSprite::drawSprite(HDC hDC)
{
    if(!spriteVisible)
        return;
    switch(spriteType) {
        case DT_draw:
            draw(hDC, spriteX, spriteY, spriteTrans, spriteTransCr);
            break;

        case DT_ALPHA:
            drawAlpha(hDC, spriteX, spriteY, spriteAlpha);
            break;

        case DT_GRAY:
            drawGray(hDC, spriteX, spriteY, spriteTrans, spriteTransCr);
            break;

        case DT_TRANSALPHA:
            drawTransAlpha(hDC, spriteBackX, spriteBackY, spriteBackWidth,
                spriteBackHeight, spriteX, spriteY, spriteAlpha, spriteTransCr);
            break;

        case DT_ANIMA:
            drawAni(hDC,spriteX, spriteY, spriteTrans, spriteTransCr);
            break;

        case DT_RECT:
            drawRect(hDC, spriteX, spriteY, spriteRectX, spriteRectY,
                spriteRectWidth, spriteRectHeight, spriteTrans, spriteTransCr);
            break;

        case DT_CXIMAGE:
            drawCxImage(hDC, spriteX, spriteY, spriteWidth, spriteHeight,
                sprFrameDelay);

        default:
            break;
    }
}

JupiterSprite::JupiterSprite(LPTSTR szFileName, bool bConvertBmp/* = false*/)
{
    loadCxImage(szFileName, bConvertBmp);
    spriteID = jupiterSpriteID;
    jupiterSpriteID++;
    spriteZOrder = 0;
}

bool JupiterSprite::loadCxImage(LPTSTR szFileName, bool bConvertBmp/* = false*/)
{
    release();

    spriteCXImage = new CxImage();	// Create CxImage object.
    spriteCXImage->Load(szFileName);	// Load image.

    if (!spriteCXImage->IsValid())
        return FALSE;

    if (spriteCXImage->GetType() == CXIMAGE_FORMAT_GIF) {	// GIF ?
        spriteCXImage->SetRetreiveAllFrames(true);// Set all frame in one image.
        // Frame to load.
        spriteCXImage->SetFrame(spriteCXImage->GetNumFrames()-1);
        spriteCXImage->Load(szFileName);    // Load file.
        sprNextFrameTime=0;		    // Set next frame time.
    }

    if (bConvertBmp)		
        spriteBmp = spriteCXImage->MakeBitmap();	// Convert to bitmap

    spriteWidth = spriteCXImage->GetWidth();	// Set width.
    spriteHeight = spriteCXImage->GetHeight();	// Set height.	
    spriteVisible = true;			        // Visible or not.

    return true;
}

void JupiterSprite::drawCxImage(HDC hDC, int x, int y,
                                int nWidth/* = 0*/, int nHeight/* = 0*/,
                                int nFrameDelay/* = 100*/)
{
    if(spriteCXImage == NULL)
        return;

    int ImgType = spriteCXImage->GetType();   // Get image type.
    // Steps to draw gif animation.
<<<<<<< HEAD
=======
    /*下面是绘制GIF动画所需步骤*/
>>>>>>> brmer
    if(ImgType == CXIMAGE_FORMAT_GIF) {  // GIF?
        if (timeGetTime() >= sprNextFrameTime) {	// Next frame?
            sprNextFrameTime = timeGetTime() + nFrameDelay; // Set next frame time.

            long m = spriteCXImage->GetNumFrames();	// Get frames count.
            long n = spriteCXImage->GetFrame() + 1; // Calc next frame index.
            if (n >= m)				// Whether is last frame index.
                n = 0;

            if (spriteCXImage->GetFrame(n))	// Next frame available?
                spriteCXImage->Copy(*spriteCXImage->GetFrame(n)); // Next frame.
            spriteCXImage->SetFrame(n);		// Set current index.
        }
    }

    spriteCXImage->Draw(hDC, x, y, nWidth, nHeight);    // Draw image.
}

void JupiterSprite::setDrawCxImageInfo(int x, int y,
                                       int nWidth/* = 0*/, int nHeight/* = 0*/,
                                       int nFrameDelay/* = 100*/)
{
    spriteType = DT_CXIMAGE;
    spriteX = x;
    spriteY = y;
    if(nWidth != 0)
        spriteWidth = nWidth;
    if(nHeight != 0)
        spriteHeight = nHeight;
    sprFrameDelay = nFrameDelay;
}
