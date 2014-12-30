/*
 *      Copyright(C) 2014, Jupiter-org.
 *      License Boilerplate: MIT
 *
 *        _______
 *       /\      \                 __     __
 *       \ /-\ \-/                /\ \   /\ \
 *          \ \ \  __   __  _____ \/_/  _\_\ \__  ____  _____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/__/\ \_ \  __/\ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\  \ \_\\ \____\\ \_\
 *         \/______/\/____/_/\ \ \   \/_/   \/_/ \/____/ \/_/
 *                            \ \_\
 *                             \/_/
 *
 *      @file:      jupiter_sprite_manage.h
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/11/25 10:35
 *      @brief:     Once you created lots of sprites, how to manage them? Aye,
 *                  you get it, use what this file provided for you.
 *                  Enjoy yourself! :-)
 */

#ifndef __JUPITER_SPRITE_MANAGE_H__
#define __JUPITER_SPRITE_MANAGE_H__

#include "jupiter_sprite.h"
#include <vector>
using std::vector;

class JupiterSpriteManage {
public:

    JupiterSpriteManage();
    ~JupiterSpriteManage();
    bool addSprite(JupiterSprite* pSprite, int nZorder=0);
    void release(bool delSprite = false);
    void delSprite(JupiterSprite* pSprite, bool delSprite = false, bool bCompress = false);
    void setSpriteVisible(JupiterSprite* pSprite, bool bVisible);
    bool findSprite(JupiterSprite* pSprite);
    void draw(HDC hDC);
    void setZOrder(JupiterSprite* pSprite, int nZorder);

private:
    vector<JupiterSprite*> jupiSprites;
};

#endif // __JUPITER_SPRITE_MANAGE_H__