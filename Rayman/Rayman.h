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
 *      @file:      Rayman.h
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/12/07 12:25
 *      @history:   2014/11/29 10:30 2014/11/24 21:00
 *      @brief:     Exciting moment, the final game. Hope you enjoy it! :-)
 */

#include "jupiter.h"
#include "jupiter_sprite.h"
#include "jupiter_sprite_manage.h"
#include "jupiter_music.h"
#include "jupiter_physics.h"

#include "resource.h"

Jupiter*	kAmbiousPtr;		// Global Jupiter pointer.	

JupiterSprite* kSprBackGround;		// Blue background
JupiterSprite* kSprBackGroundSpace;

JupiterSprite* kSprCloud[8];		// Cloud image.
JupiterSprite* kSprThor;		// Used to draw plane image.
JupiterSprite* kSprThorPic[3];	        // Store plane images.

Point kCloudPt[8];			// Clouds left-top position.

JupiterSpriteManage kSprManage;		// SpriteManage object.

int kSceneNo;				// Scene number.
bool kIsInitScene;			// Whether scene is initialized.

void sceneSwitch();
bool initScene1(HWND hWnd);	// Init scene 1
bool playScene1(HWND hWnd);	// Play scene 1

JupiterMusic* kMscBGM;			// Background music.
JupiterMusic* kMscPlaneFly;		// Plane flying music.

JupiterSprite* kSprEnemyLv1[10];	        // Enemy images.

JupiterPhysics* kPhyFight;		// Plane physical actions.
JupiterPhysics* kPhyEnemyLv1[10];	// Enemy plane physical actions.


// Bind sprite and physics
void setEnemyInfo(JupiterSprite* spr, JupiterPhysics* phy, int nType);

// Used to make blast effect.
class JupiterBlast {
/*protected:*/
public:
	long jupiBlastEndTime;	// End time.
    bool jupiBlastFree;		// Free?
	JupiterSprite* jupiBlastSprite;	// Blast animation.

public:
	JupiterBlast()	
	{
		jupiBlastSprite = NULL;
		jupiBlastEndTime = 0;
		jupiBlastFree = true;	
	};

	~JupiterBlast(){};

        // Set animation, spr is blas.gif, rectJupiterBlast - where to display.
        // nDelay -- display time.
	void set(JupiterSprite* spr, RECT rectJupiterBlast, long nDelay)
	{
		jupiBlastSprite = spr;
		jupiBlastEndTime = timeGetTime() + nDelay;
		jupiBlastSprite->setDrawCxImageInfo(rectJupiterBlast.left-50,
                    rectJupiterBlast.top-50, 100, 100);
		jupiBlastSprite->setVisible(true);
		jupiBlastFree = false;
	};

	bool play()	// Play animation.
	{
        if (!jupiBlastFree && (jupiBlastEndTime <= timeGetTime())) {
		    jupiBlastSprite->setVisible(false);
		    jupiBlastSprite = NULL;
		    jupiBlastEndTime = 0;
		    jupiBlastFree = true;
		    return true;
	    } else {
		    return false;
	    }
	}

	bool isFree() { return jupiBlastFree; }    // Animation is free?
};

JupiterBlast kJupiterBlast;			// Blast animation object.
JupiterSprite* kSprJupiterBlast;		// Blast sprite.

JupiterPhysics* kPhyShot;	// Bullet movement physics object.
JupiterSprite* kSprShot;	// Bullet sprite.

JupiterSprite* kSprShots[8];
JupiterPhysics* kPhyShots[8];

bool kIsShotEnemy;		// Shot enemy;
bool kShotFighter;		// Crashed by enemy?

int kResult;			// Score.
JupiterSprite* kSprGameOver;	// Game over.
JupiterBlast kBlastLoadScene;
JupiterSprite* kSprLoadScene;		// Loading scene.
