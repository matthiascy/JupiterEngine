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
    
};

#endif // __JUPITER_SPRITE_H__