/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__  ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/___/\ \_/\  __/ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \/______/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
 *                            \ \_\
 *                             \/_/
 *      @file:      Rayman.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 21:00
 *      @brief:     Exciting moment, the final game. Hope you enjoy it! :-)
 */

#include "jupiter.h"
#include "jupiter_sprite.h"
#include "jupiter_sprite_manage.h"
#include "jupiter_music.h"
#include "jupiter_physics.h"

#include "resource.h"

Jupiter*	kAmbiousPtr;		// Global Jupiter pointer.	

JupiterSprite* kSprLoadScene;		// Loading scene.
JupiterSprite* kSprBackGround;		// Blue background	

JupiterSprite* kSprCloud[8];		// Cloud image.
JupiterSprite* kSprThor;		// Used to draw plane image.
JupiterSprite* kSprThorPic[3];	        // Store plane images.

POINTF kCloudPt[8];			// Clouds left-top position.

JupiterSpriteManage kSprManage;		// SpriteManage object.

int kSceneNo;				// Scene number.
bool kIsInitScene;			// Whether scene is initialized.

bool initScene1(HWND hWnd);	// Init scene 1
bool playScene1(HWND hWnd);	// Play scene 1

JupiterMusic* kMscBGM;			// Background music.
JupiterMusic* kMscPlaneFly;		// Plane flying music.

JupiterSprite* kSprEnemy[4];	        // Enemy images.

JupiterPhysics* kPhyFight;		// Plane physical actions.
JupiterPhysics* kPhyEnemy[4];	// Enemy plane physical actions.

// Bind sprite and physics
void setEnemyInfo(JupiterSprite* spr, JupiterPhysics* phy, int nType);

// Used to make blast effect.
class JupiterBlast
{
protected:
	long jupiBlastEndTime;	// End time.
	JupiterSprite* jupiBlastSprite;	// Blast animation.
	bool	jupiBlastFree;		// Free?

public:
	JupiterBlast()	
	{
		jupiBlastSprite = NULL;
		jupiBlastEndTime = 0;
		jupiBlastFree = true;	
	};
	~JupiterBlast(){};

        // Set animation, spr is blas.gif, rJupiterBlast - where to display.
        // nDelay -- display time.
	void set(JupiterSprite* spr, RECT rJupiterBlast, long nDelay)
	{
		jupiBlastSprite = spr;
		jupiBlastEndTime = timeGetTime() + nDelay;
		jupiBlastSprite->setDrawCxImageInfo(rJupiterBlast.left-50,
                    rJupiterBlast.top-50, 100, 100);
		jupiBlastSprite->setVisible(true);
		jupiBlastFree = false;
	};

	bool play()	// Play animation.
	{
            if(!jupiBlastFree && jupiBlastEndTime <= timeGetTime()) {
			jupiBlastSprite->setVisible(false);
			jupiBlastSprite = NULL;
			jupiBlastEndTime = 0;
			jupiBlastFree = true;
			return true;
	    } else {
			return false;
	    }
	}

	bool IsFree()		// Animation is free?
	{ return jupiBlastFree; }
};

JupiterBlast kJupiterBlast;			// Blast animation object.
JupiterSprite* kSprJupiterBlast;		// Blast sprite.

JupiterPhysics* kPhyShot;	// Bullet movement physics object.
JupiterSprite* kSprShot;	// Bullet sprite.

bool kIsShotEnemy;		// Shot enemy;
bool kShotFighter;		// Crashed by enemy?

int kResult;			// Score.
JupiterSprite* g_pSprGameOver;	// Game over.