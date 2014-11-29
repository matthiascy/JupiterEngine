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
 *      @file:      Rayman.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/29 10:30
 *      @history:   2014/11/24 21:00
 *      @brief:     Exciting moment, the final game. Hope you enjoy it! :-)
 */
#include "Rayman.h"

#include <time.h>

bool gameInitialize(HINSTANCE hInstance)
{
    kAmbiousPtr = new Jupiter(hInstance, TEXT("Rayman"), TEXT("Rayman"),
        /*IDI_BIGICON*/NULL, /*IDI_SMALLICON*/ NULL, 
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

    kSprLoadScene = new JupiterSprite(NULL, "res\\LoadScene.bmp");


    kSceneNo=1;	
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

    kSprBackGround->setDrawInfo(0,0);
    kSprManage.addSprite(kSprBackGround, 0);

    kSprManage.addSprite(kSprCloud[0], 1);
    kSprManage.addSprite(kSprCloud[1], 1);
    kSprManage.addSprite(kSprCloud[2], 1);
    kSprManage.addSprite(kSprCloud[3], 1);
    kSprManage.addSprite(kSprCloud[4], 1);
    kSprManage.addSprite(kSprCloud[5], 1);
    kSprManage.addSprite(kSprCloud[6], 1);
    kSprManage.addSprite(kSprCloud[7], 1);

    kSprManage.addSprite(kSprThor, 3);

    srand((unsigned)time(NULL));
    for(int i = 0; i < 8; i++) {
        kCloudPt[i].x = rand() % kAmbiousPtr->getWidth();
        kCloudPt[i].y = rand() % kAmbiousPtr->getHeight();
    }

    // Create music object and open it.
    kMscPlaneFly = new JupiterMusic("res\\fly.wav");
    kMscBGM = new JupiterMusic("res\\bgm.mp3");
    kMscBGM->play();

    // Enemy plane.
    kSprEnemy[0] = new JupiterSprite("res\\enemy1.png");
    kSprEnemy[1] = new JupiterSprite("res\\enemy2.png");
    kSprEnemy[2] = new JupiterSprite("res\\enemy3.png");
    kSprEnemy[3] = new JupiterSprite("res\\enemy4.png");

    kSprManage.addSprite(kSprEnemy[0], 3);
    kSprManage.addSprite(kSprEnemy[1], 3);
    kSprManage.addSprite(kSprEnemy[2], 3);
    kSprManage.addSprite(kSprEnemy[3], 3);

    // Create physics object.
    kPhyEnemy[0] = new JupiterPhysics();
    kPhyEnemy[1] = new JupiterPhysics();
    kPhyEnemy[2] = new JupiterPhysics();
    kPhyEnemy[3] = new JupiterPhysics();


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
    kPhyShot=new JupiterPhysics(rObjectShot, rBoundShot, ptFocusShot,
        ptVeloShot, ptAccelerateShot, ptDesShot, false);

    kPhyShot->setVisible(false);
    kSprManage.addSprite(kSprShot, 4);	



    // Blast and game end scene.
    kSprJupiterBlast = new JupiterSprite("res\\bom.gif");
    g_pSprGameOver = new JupiterSprite("res\\GameOver.png");
    kSprManage.addSprite(kSprJupiterBlast, 4);
    kSprManage.addSprite(g_pSprGameOver, 4);

    g_pSprGameOver->setDrawCxImageInfo(30, 300);
    g_pSprGameOver->setVisible(false);

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

    // If don't move, reset physics.
        if((phy->getPathArrive() && nType == 2) || (!phy->getMoveState() && nType == 1)) {
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
            phy->moveToDes();
            break;
        case 2:
            phy->moveAlongPath(ptPath, 8);
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

    setEnemyInfo(kSprEnemy[0], kPhyEnemy[0], 2);
    setEnemyInfo(kSprEnemy[1], kPhyEnemy[1], 2);
    setEnemyInfo(kSprEnemy[2], kPhyEnemy[2], 1);
    setEnemyInfo(kSprEnemy[3], kPhyEnemy[3], 1);

    // Enemy crashed with Thor or shotted by bullet.
    RECT r1, r2;

    for (int i = 0; i < 4; i++) {
        // Crashed with Thor?
        if (kPhyEnemy[i]->collision(kPhyFight, BA_CLEAR, &r1)) {
            kPhyEnemy[i]->setVisible(false);
            kSprEnemy[i]->setVisible(false);
            kPhyFight->setVisible(false);
            kSprThor->setVisible(false);
            kShotFighter = true;
            break;
        }

        // Whether been shotted.
        if (kPhyShot->getMoveState()) {
            if (kSprEnemy[i]->getVisible() && kSprShot->getVisible()) {
                if (kPhyEnemy[i]->collision(kPhyShot, BA_CLEAR, &r2)) {
                    kIsShotEnemy = true;
                    kSprShot->setVisible(false);
                    kPhyShot->setMoveState(false);
                    kSprEnemy[i]->setVisible(false);

                    kJupiterBlast.set(kSprJupiterBlast, r2, 800); // Blast ani.

                    switch (i)  { // Score
                        case 0:
                            kResult += 5;
                            break;
                        case 1:
                            kResult += 10;
                            break;
                        case 2:
                            kResult += 3;
                            break;
                        case 3:
                            kResult += 2;
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
    if (kShotFighter && !g_pSprGameOver->getVisible()) {
        kJupiterBlast.set(kSprJupiterBlast, r1, 3000);
        g_pSprGameOver->setVisible(true);
    }

    if (!kJupiterBlast.isFree())	 {// Blast animation finished.
        // Thor blasted?
        if (kJupiterBlast.play() && kShotFighter)
            if (MessageBox(kAmbiousPtr->getWindow(), "Game Over!Try again?",
                "GameOver", MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES) {
                kJupiterBlast.set(kSprJupiterBlast, r1, 3000);
                g_pSprGameOver->setVisible(false);
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
    if (!kIsInitScene)
        kSprLoadScene->draw(hDC, 0, 0);
    else
        kSprManage.draw(hDC);
    // Draw point.
    SetBkMode(hDC, TRANSPARENT);   	// Background is transparent.
    SetTextColor(hDC, RGB(255, 255, 0));	// Text color is yellow.
    char szResult[20];
    sprintf(szResult, "result:%d", kResult);

    RECT rResult = { 300, 10, 400, 40 };
    DrawText(hDC, szResult, -1, &rResult, DT_LEFT);		
}

void gameEnd()
{
    delete kMscBGM;
    delete kMscPlaneFly;
    delete[] (*kPhyEnemy); 
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
    if(MessageBox(kAmbiousPtr->getWindow(), "Are you sure to quit? T_T", "Quit",
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
        kMscPlaneFly->play(300,false,false);
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