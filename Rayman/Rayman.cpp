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
 *      @file:      Rayman.cpp
 *      @author:    ChenYang :) GuoYan
 *      @date:      2014/12/7 12:14
 *      @history:   2014/11/29 10:30 2014/11/24 21:00
 *      @brief:     Exciting moment, the final game. Hope you enjoy it! :-)
 */
#include "Rayman.h"

#include <time.h>

bool gameInitialize(HINSTANCE hInstance)
{
    kAmbiousPtr = new Jupiter(hInstance, TEXT("Rayman"), TEXT("Rayman"),
        IDI_ICON_BIG, IDI_ICON_SMALL, 
        false, 32, 450, 600);
    if(kAmbiousPtr == NULL)
        return false;

    kAmbiousPtr->setFrameDelay(30) ;
    kAmbiousPtr->setPause(false);

    if(!kAmbiousPtr->createGameWindow()) {
        MessageBox(NULL,"Failed to change display mode!\r\nExit!",
            "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

void gameStart(HWND hWnd)
{
    // Set mouse cursor
    long lCur = (long)LoadCursor(kAmbiousPtr->getInstance(), MAKEINTRESOURCE(IDC_CURSOR));
    SetClassLong(hWnd, GCL_HCURSOR, lCur);

    kSprLoadScene = new JupiterSprite("res\\loading.gif");

    kSceneNo = 1;	
    kIsInitScene = false;	

    initScene1(hWnd);
}

void gameAction(HWND hWnd)
{
    if(kIsInitScene) {
        playScene1(hWnd);
    }
    InvalidateRect(hWnd, NULL, false);
    UpdateWindow(hWnd);
}

bool initScene1(HWND hWnd)
{
    kSprCloud[0] = new JupiterSprite("res\\cloud_1.png");
    kSprCloud[1] = new JupiterSprite("res\\cloud_2.png");
    kSprCloud[2] = new JupiterSprite("res\\cloud_3.png");
    kSprCloud[3] = new JupiterSprite("res\\cloud_4.png");
    kSprCloud[4] = new JupiterSprite("res\\cloud_5.png");
    kSprCloud[5] = new JupiterSprite("res\\cloud_6.png");
    kSprCloud[6] = new JupiterSprite("res\\cloud_7.png");
    kSprCloud[7] = new JupiterSprite("res\\cloud_8.png");

    kSprThorPic[0] = new JupiterSprite("res\\FighterPlane.png");
    kSprThorPic[1] = new JupiterSprite("res\\FighterPlaneL.png");
    kSprThorPic[2] = new JupiterSprite("res\\FighterPlaneR.png");

    kSprThor = new JupiterSprite();
    kSprThor->setCxImage(kSprThorPic[0]->load());

    // Use DC to create blue background.
    HDC hDCTmp = GetDC(hWnd);
    kSprBackGround = new JupiterSprite(hDCTmp, kAmbiousPtr->getWidth(),
        kAmbiousPtr->getHeight(), RGB(40, 80, 160));
    ReleaseDC(hWnd, hDCTmp);

    kSprBackGroundSpace = new JupiterSprite("res\\space_1.png");
    kSprBackGroundSpace->setDrawCxImageInfo(0, 0, 450, 600);

    kSprBackGround->setDrawInfo(0, 0);
    kSprManage.addSprite(kSprBackGround, 0);

    for (int i = 0; i < 8; i++) {
        kSprManage.addSprite(kSprCloud[i], 1);
    }

    kSprManage.addSprite(kSprThor, 3);

    srand((unsigned)time(NULL));
    for(int i = 0; i < 8; i++) {
        kCloudPt[i].x = rand() % kAmbiousPtr->getWidth();
        kCloudPt[i].y = rand() % kAmbiousPtr->getHeight();
    }

    // Create music object and open it.
    kMscPlaneFly = new JupiterMusic("res\\music\\fly.wav");
//    kMscBGM = new JupiterMusic("res\\bgm.mp3");
    kMscBGM = new JupiterMusic("res\\music\\bgm4.mp3");
    kMscBGM->play();

    // Enemy plane.
    kSprEnemyLv1[0] = new JupiterSprite("res\\enemy1.png");
    kSprEnemyLv1[1] = new JupiterSprite("res\\enemy2.png");
    kSprEnemyLv1[2] = new JupiterSprite("res\\enemy3.png");
    kSprEnemyLv1[3] = new JupiterSprite("res\\enemy4.png");
    
    kSprEnemyLv1[4] = new JupiterSprite("res\\omni_1.png");
    
    kSprEnemyLv1[5] = new JupiterSprite("res\\omni_2.png");
    kSprEnemyLv1[6] = new JupiterSprite("res\\omni_3.png");
    kSprEnemyLv1[7] = new JupiterSprite("res\\omni_4.png");
    kSprEnemyLv1[8] = new JupiterSprite("res\\omni_5.png");
    kSprEnemyLv1[9] = new JupiterSprite("res\\omni_6.png");
    
    // Create physics object.
    for (int i = 0; i < 10; i++) {
        kSprManage.addSprite(kSprEnemyLv1[i], 3);
        kPhyEnemyLv1[i] = new JupiterPhysics();
    }

    // Bound plane with physics object.
    RECT rObject = {200, 400, 300, 500};
    RECT rBound = {0, 0, kAmbiousPtr->getWidth(), kAmbiousPtr->getHeight()};
    Point ptFocus = {0, 0};
    Point ptVelo = {0, 5};
    Point ptAccelerate = {0, 0};
    Point ptDes = {300, 300};
    kPhyFight = new JupiterPhysics(rObject, rBound, ptFocus, ptVelo,
        ptAccelerate, ptDes, false);

    kSprThor->setVisible(true);
    kPhyFight->setVisible(true);

    // Set bullet physics object.
    kSprShot = new JupiterSprite("res\\Shot.png");
    RECT rObjectShot = {0, 0, 20, 100};
    RECT rBoundShot = {0, -100, 400, 600};
    Point ptFocusShot = {10, 0};
    Point ptVeloShot = {0, 10};
    Point ptAccelerateShot = {0, 0};
    Point ptDesShot = {0, 0};
    kPhyShot = new JupiterPhysics(rObjectShot, rBoundShot, ptFocusShot,
        ptVeloShot, ptAccelerateShot, ptDesShot, false);

    kPhyShot->setVisible(false);
    kSprManage.addSprite(kSprShot, 4);

    for (int i = 0; i < 8; i++) {
        kSprShots[i] = new JupiterSprite("res\\Shot.png");
        kPhyShots[i] = new JupiterPhysics(rObjectShot, rBoundShot, ptFocusShot,
            ptVeloShot, ptAccelerateShot, ptDesShot, false);
        kPhyShots[i]->setVisible(false);
        kSprManage.addSprite(kSprShots[i], 4);
    }

    // Blast and game end scene.
    kSprJupiterBlast = new JupiterSprite("res\\bom.gif");
    kSprGameOver = new JupiterSprite("res\\GameOver.png");
    kSprManage.addSprite(kSprJupiterBlast, 4);
    kSprManage.addSprite(kSprGameOver, 4);
    kSprManage.addSprite(kSprBackGroundSpace, 0);

    kSprGameOver->setDrawCxImageInfo(50, 300);
    kSprGameOver->setVisible(false);

    kIsShotEnemy = false;	
    kShotFighter = false;	

    kResult = 0;

    kIsInitScene = true;

   return true;
}

void setEnemyInfo(JupiterSprite* spr, JupiterPhysics* phy, int nType)
{
    srand((unsigned)time(NULL));
    // horizontal position.
    int x = rand() % (kAmbiousPtr->getWidth() - spr->getWidth());
    
    //Point ptPath[4][8];
    // Plane's generation place. (plane_x, -plane_y)
    // plane_x is a random number between 0 ~ Thor's width - Enemy's width
    // plane_y is negative of plane's height.
    // Simply, suppose initialized position is (200, -plane_height)
    // path is set as following figure.
    //         (x, -plane_height)
    //    |          o------>o            |----> off screen
    //    |                  |            |
    //    -------------------|-------------
    //    |                 \|/           |    
    //    |                  o------>o    |-----> gaming area
    // 100|             same point---|    |
    //    |   o<---------------------o    |
    //    |   |                           |
    //    |  \|/                          |
    // 200|   o------------->o            |
    //    |                  |            |
    //    |                 \|/           |
    // 300|                  o            |
    //    |                  |            |
    //    |                 \|/           |
    // 400|                  o            | (Ambitious' height = 400)
    //    ---------------------------------
    //       100     200    300    400
    Point ptPath[8] = {
        {x+100, -spr->getHeight()},
        {x+100, 100},
        {x+200, 100},
        {x-100, 100}, 
        {x-100, 200},
        {x+100, 200},
        {x+100, 300},
        {x, kAmbiousPtr->getHeight()}
    };
    
    Point ptPath_1[8] = {
        {x+100, -spr->getHeight()},
        {x+100, 100}, 
        {x+100, 100 + rand()%spr->getHeight()}, 
        {x+200, 100 + rand()%spr->getHeight()}, 
        {x-100, 100 + rand()%spr->getHeight()}, 
        {x-100, 200 + rand()%spr->getHeight()}, 
        {x+100, 200 - rand()%spr->getHeight()}, 
        {x, kAmbiousPtr->getHeight()}
    };

    Point ptPath_2[8] = {
        {x+100, -spr->getHeight()},
        {x+100, 100},
        {x+200, 100 + rand()%spr->getHeight()},
        {x-100, 100}, 
        {x + rand()%spr->getHeight(), 200},
        {x+100, 200},
        {x+100, 300},
        {x, kAmbiousPtr->getHeight()}
    };

    Point ptPath_3[8] = {
        {x+100, -spr->getHeight()},
        {x+150, 100},
        {x+250, 250},
        {x-100, 150}, 
        {x-150, 220},
        {x+100, 200},
        {x+100, 350},
        {x, kAmbiousPtr->getHeight()}
    };
    // If don't move, reset physics.
        if ((phy->getPathArrive() && nType == 2) || (!phy->getMoveState() && nType == 1)) {
            RECT rObject = {x, -spr->getHeight(), x+spr->getWidth(), 0};
            RECT rBound = {0, -spr->getHeight(), kAmbiousPtr->getWidth(), 
                kAmbiousPtr->getHeight() + spr->getHeight()};
            Point ptFocus = {0, 0};
            Point ptVelo = {0, rand()%9 + 1};
            Point ptAccelerate = {0, 0};
            Point ptDes = {x, kAmbiousPtr->getHeight()};
            phy->setObject(rObject, rBound, ptFocus, ptVelo, ptAccelerate,
                ptDes, true);
    }

    // Move as type indicated.
    switch(nType) {
        case 1:
            srand(time(NULL));
            switch((rand()%100)%3) {
                case 0:
                    phy->moveAlongPath(ptPath_1, 8);
                    break;
                case 1:
                    phy->moveToDes();
                    break;
                case 2:
                    phy->moveAlongPath(ptPath_3, 8);
                    break;
            }
            break;

        case 2:
            phy->moveAlongPath(ptPath_2, 8);
            break;
    }

    spr->setVisible(phy->getVisible());

    if(spr->getVisible())
        spr->setDrawCxImageInfo(phy->getLeftTop().x, phy->getLeftTop().y);
}

bool playScene1(HWND hWnd)
{	
    for (int i = 0; i < 8; i++) {	// Clouds position and z order.
        // Reach bottom.
        if (kCloudPt[i].y > kAmbiousPtr->getHeight()) {
            // Reach bottom, set y to top, x is random.
            kCloudPt[i].x = rand() % kAmbiousPtr->getWidth() - 
                kAmbiousPtr->getWidth() / 2;
            kCloudPt[i].y =- (kSprCloud[i]->getHeight());

            // Set clouds layer.
            if (rand() % 2)
                kSprManage.setZOrder(kSprCloud[i], 1);
            else
                kSprManage.setZOrder(kSprCloud[i], 4);				
        } else {
            // Clouds layer.
            if(kSprCloud[i]->getZOrder() < 4)	
                kCloudPt[i].y += 2;	// Back, move down 2 pixels.
            else
                kCloudPt[i].y += 3;	// Fore, move down 3 pixels.
        }
        kSprCloud[i]->setDrawCxImageInfo(kCloudPt[i].x, kCloudPt[i].y);
    }
    
    for (int i = 0; i < 10; i++) {
        setEnemyInfo(kSprEnemyLv1[i], kPhyEnemyLv1[i], i%2+1);
    }
    // Enemy crashed with Thor or shotted by bullet.
    RECT r1, r2;

    for (int i = 0; i < 10; i++) {
        // Crashed with Thor?
        if (kPhyEnemyLv1[i]->collision(kPhyFight, BA_CLEAR, &r1)) {
            kPhyEnemyLv1[i]->setVisible(false);
            kSprEnemyLv1[i]->setVisible(false);
            kPhyFight->setVisible(false);
            kSprThor->setVisible(false);
            kShotFighter = true;
            break;
        }

        // Whether been shotted.
        if (kPhyShot->getMoveState()) {
            if (kSprEnemyLv1[i]->getVisible() && kSprShot->getVisible()) {
                if (kPhyEnemyLv1[i]->collision(kPhyShot, BA_CLEAR, &r2)) {
                    kIsShotEnemy = true;
                    kSprShot->setVisible(false);
                    kPhyShot->setMoveState(false);
                    kSprEnemyLv1[i]->setVisible(false);
                    kJupiterBlast.set(kSprJupiterBlast, r2, 800); // Blast ani.

                    switch (i)  { // Score
                        case 0:
                            kResult += 5;
                            break;
                        case 1:
                            kResult += 10;
                            break;
                        case 2:
                        case 7:
                            kResult += 3;
                            break;
                        case 3:
                        case 8:
                        case 9:
                            kResult += 2;
                            break;
                        case 4:
                            kResult += 4;
                            break;
                        case 5:
                        case 6:
                            kResult += 7;
                            break;
                    }
                }		
            }
        }

    }
    // Bullet state, on move, set move settings and paint.
    if (kPhyShot->getMoveState()) {
        kPhyShot->moveToDes();
        kSprShot->setDrawCxImageInfo(kPhyShot->getLeftTop().x,
            kPhyShot->getLeftTop().y, 20, 100);
    } else {
        kSprShot->setVisible(false);
    }

    // Thor crashed, not game over, display game over.
    if (kShotFighter && !kSprGameOver->getVisible()) {
        kJupiterBlast.set(kSprJupiterBlast, r1, 3000);
        kSprGameOver->setVisible(true);
        //kSprLoadScene->setVisible(true);
        //kSprLoadScene->drawCxImage(GetDC(hWnd), 0, 0);
    }

    if (!kJupiterBlast.isFree()) {// Blast animation finished.
        // Thor blasted?
        if (kJupiterBlast.play() && kShotFighter)
            if (MessageBox(kAmbiousPtr->getWindow(), "Game Over!Try again?",
                "GameOver", MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES) {
                kJupiterBlast.set(kSprJupiterBlast, r1, 3000);
                kSprGameOver->setVisible(false);
                kSprManage.release(true);
                delete kPhyFight;
                delete kPhyShot;
                delete kMscBGM;
                delete kMscPlaneFly;
                gameStart(hWnd);
            }
            else
                DestroyWindow(hWnd);	
    }
    return false;
}

void gamePaint(HDC hDC)
{
    if (!kIsInitScene) {
        //kSprLoadScene->setDrawCxImageInfo(0, 0, 450, 600, 5);
        kSprLoadScene->drawCxImage(hDC, 0, 0);
    }
    else {
        kSprManage.draw(hDC);
    }
    // Draw point.
    SetBkMode(hDC, TRANSPARENT);
    char szResult[20];
    sprintf(szResult, "Score:%d", kResult);		

    HFONT hFont = CreateFont(40, 40, 0, 0, FW_REGULAR, false, false, false,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
        FIXED_PITCH | FF_MODERN, "Petala Pro");
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    SetTextColor(hDC, 0x00ee00);
    TextOut(hDC, 10, 550, szResult, (int)strlen(szResult));
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont);
}

void gameEnd()
{
    delete kMscBGM;
    delete kMscPlaneFly;
    delete[] (*kPhyEnemyLv1); 
    delete kPhyFight;
    kSprManage.release(true);
    delete kSprLoadScene;
    delete kAmbiousPtr;
}

void gameActive(HWND hWnd)
{
    kAmbiousPtr->setPause(false);	
}

void gamePause(HWND hWnd)
{
    kAmbiousPtr->setPause(true);
}

bool gameWindowClose(HWND hWnd)
{
    if (MessageBox(kAmbiousPtr->getWindow(), "Are you sure to quit? T_T", "Quit",
        MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES)
        return true;
    else
        return false;
}

void keyEvent(HWND hWnd)
{
    if (GetAsyncKeyState(VK_UP) < 0) {  // Press up?
        kPhyFight->moveDirect(DI_UP);	// Move up.
        kPhyFight->checkErr(true);
        kSprThor->setDrawCxImageInfo(kPhyFight->getLeftTop().x, 
            kPhyFight->getLeftTop().y, 100, 100);
        kSprThor->setCxImage(kSprThorPic[0]->load());
        kMscPlaneFly->play(300, false, false);
    } else if (GetAsyncKeyState(VK_DOWN) < 0) {
        kPhyFight->moveDirect(DI_DOWN);	
        kPhyFight->checkErr(true);		

        kSprThor->setDrawCxImageInfo(kPhyFight->getLeftTop().x, 
            kPhyFight->getLeftTop().y, 100, 100);
        kSprThor->setCxImage(kSprThorPic[0]->load());
    } else if (GetAsyncKeyState(VK_LEFT) < 0) {
        kPhyFight->moveDirect(DI_LEFT);
        kPhyFight->checkErr(true);	
        kSprThor->setDrawCxImageInfo(kPhyFight->getLeftTop().x + 10,
            kPhyFight->getLeftTop().y, 80, 100);
        kSprThor->setCxImage(kSprThorPic[1]->load());
        kMscPlaneFly->play(300, false, false);
    } else if (GetAsyncKeyState(VK_RIGHT) < 0) {
        kPhyFight->moveDirect(DI_RIGHT);
        kPhyFight->checkErr(true);	
        kSprThor->setDrawCxImageInfo(kPhyFight->getLeftTop().x + 10,
            kPhyFight->getLeftTop().y, 80, 100);
        kSprThor->setCxImage(kSprThorPic[2]->load());
        kMscPlaneFly->play(300, false, false);	// Play fly music.
    } else if (GetAsyncKeyState(VK_PRIOR) < 0) {	// PageUp, volume up.
        kMscBGM->volumeUp();
    } else if (GetAsyncKeyState(VK_NEXT) < 0) {	// PageDown, volume down.
        kMscBGM->volumeDown();
    } else if (GetAsyncKeyState(VK_HOME ) < 0) {	// Home, replay.
        kMscBGM->play(300, true);
    } else if (GetAsyncKeyState(VK_END ) < 0) {	// End, stop music.
        kMscBGM->stop();
    } else if (GetAsyncKeyState(VK_ESCAPE) < 0) {	// Pressed ESCAPE?
        SendMessage(hWnd, WM_CLOSE, 0, 0);
    } else {			// No key is pressed.
        kSprThor->setCxImage(kSprThorPic[0]->load());
        kSprThor->setDrawCxImageInfo(kPhyFight->getLeftTop().x, 
            kPhyFight->getLeftTop().y, 100, 100);
    }

    if (GetAsyncKeyState('S') < 0) {// S pressed? if no bullet in scene, beam bullet.
        if (!kPhyShot->getMoveState() && kSprThor->getVisible()) {
            Point pt = {(kPhyFight->getPos().x + kPhyFight->getWidth() / 2), 
                (kPhyFight->getPos().y)};
            kPhyShot->setPos(pt);
            kPhyShot->setDes(pt.x, -100);
            kPhyShot->setMoveState(true);
            kPhyShot->setVisible(true);
            kSprShot->setVisible(true);
        }   
    }
}

void mouseLButtonDown(HWND hWnd, int x, int y, WPARAM wParam)
{

}

void mouseLButtonUp(HWND hWnd, int x, int y, WPARAM wParam)
{

}

void mouseDoubleClick(HWND hWnd, int x, int y, WPARAM wParam)
{

}

void mouseRButtonDown(HWND hWnd, int x, int y, WPARAM wParam)
{

}

void mouseMove(HWND hWnd, int x, int y, WPARAM wParam)
{

}