/*
 *      Copyright(C) 2014, Jupiter-org.
 *      License Boilerplate: MIT
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
 *      @file:      jupiter_sprite_manage.cpp
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/11/25 10:47
 *      @brief:     Once you created lots of sprites, how to manage them? Aye,
 *                  you get it, use what this file provided for you.
 *                  Enjoy yourself! :-)
 */

#include "jupiter_sprite_manage.h"

JupiterSpriteManage::JupiterSpriteManage()
{

}

JupiterSpriteManage::~JupiterSpriteManage()
{
    release();
}

bool JupiterSpriteManage::addSprite(JupiterSprite * pSprite, int nZorder)
{
    if (pSprite != NULL) {
        pSprite->setZOrder(nZorder);
        if (!jupiSprites.empty()) {
            vector<JupiterSprite *>::iterator curSprite;
            for (curSprite = jupiSprites.begin(); curSprite != jupiSprites.end(); 
                curSprite++)
                if (nZorder < (*curSprite)->getZOrder()) {
                    jupiSprites.insert(curSprite, pSprite);
                    return true;
                }
        }		
        jupiSprites.push_back(pSprite);
        return true;
    }
    return false;
}

void JupiterSpriteManage::release(bool delSprite)
{
    if (!jupiSprites.empty()) {
        if(delSprite) {
            vector<JupiterSprite *>::iterator curSprite;
            for (curSprite = jupiSprites.begin(); curSprite != jupiSprites.end();
                curSprite++)
                delete *curSprite; // delete sprite object
        }
        jupiSprites.clear();        // delete pointer of container
        vector<JupiterSprite *>(jupiSprites).swap(jupiSprites);	// compress container
    }
}

void JupiterSpriteManage::delSprite(JupiterSprite* pSprite, bool delSprite,
                                    bool bCompress)
{
    if (!jupiSprites.empty()) {
        vector<JupiterSprite*>::iterator curSprite;
        for (curSprite = jupiSprites.begin(); curSprite != jupiSprites.end();
            curSprite++)
            if ((*curSprite)->getID() == pSprite->getID()) {
                if (delSprite) delete pSprite;			

                jupiSprites.erase(curSprite);
                break;
            }
            if (bCompress)
                vector<JupiterSprite *>(jupiSprites).swap(jupiSprites);
    }
}

bool JupiterSpriteManage::findSprite(JupiterSprite* pSprite)
{
    if (!jupiSprites.empty()) {
        vector<JupiterSprite*>::iterator curSprite;
        for (curSprite = jupiSprites.begin(); curSprite != jupiSprites.end();
            curSprite++)
            if ((*curSprite)->getID() == pSprite->getID())
                return true;
    }
    return false;	
}            

void JupiterSpriteManage::setSpriteVisible(JupiterSprite* pSprite, bool bVisible)
{
    pSprite->setVisible(bVisible);
}

void JupiterSpriteManage::draw(HDC hDC)
{
    if (!jupiSprites.empty()) {
        vector<JupiterSprite *>::iterator curSprite;
        for (curSprite = jupiSprites.begin(); curSprite != jupiSprites.end();
            curSprite++) {
            (*curSprite)->drawSprite(hDC);
        }
    }
}

void JupiterSpriteManage::setZOrder(JupiterSprite* pSprite, int nZorder)
{
    delSprite(pSprite);
    addSprite(pSprite, nZorder);
}
