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
 *      @file:      jupiter_physics.h
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 20:40
 *      @brief:     Interesting JupiterPhysics. Make you game object behave 
 *                  much more real. Have fun! :-)
 */

#ifndef __JUPITER_PHYSICS_H__
#define __JUPITER_PHYSICS_H__

#include "windows.h"
#include <stdio.h>
#include <math.h>

// Actions after collision
enum BOUNDACTION {
    BA_stop = 0, BA_LOOP = 1, BA_REBOUND = 2, BA_CLEAR = 3
};

// Direction
enum DIRECTION {
    DI_STOP = 0, DI_LEFT = 1, DI_RIGHT = 2, DI_UP = 3, DI_DOWN = 4
};

typedef struct POINTFtag {
public:
    float x;
    float y;

    const POINTFtag& operator  =(const POINTFtag &pt)   
    {   
        x=pt.x;
        y=pt.y;
        return * this ;
    }
    const POINTFtag operator+(const POINTFtag &pt)
    {	
        POINTFtag ptTmp;
        ptTmp.x=x+pt.x;
        ptTmp.y=y+pt.y;
        return ptTmp;
    }
    const POINTFtag operator-(const POINTFtag &pt)
    {
        POINTFtag ptTmp;
        ptTmp.x=x-pt.x;
        ptTmp.y=y-pt.y;
        return ptTmp;
    }
} POINTF;

/*
class POINTF {
public:
float x, y;
POINTF();
~POINTF();
POINTF& operator= (const POINTF& rhs);
POINTF operator+ (const POINTF& p);
POINTF operator- (const POINTF& p);
};
*/

class JupiterPhysics
{
protected:
    POINTF	jupiPhyPtFocus;
    POINTF	jupiPhyPtPos;
    RECT	jupiPhyRectObject;
    RECT	jupiPhyRectFocusBound;
    RECT	jupiPhyRectBound;


    POINTF	jupiPhyPtVelo;
    POINTF	jupiPhyPtAccelerate;
    float	jupiPhyStep;	
    POINTF	jupiPhyPtDes;

    DIRECTION	jupiPhyDirect;

    bool	jupiPhyMove;

    bool	jupiPhyPathArrive;
    int		jupiPhynPathIndex;
    bool	jupiPhyInitIndex;

    bool	jupiPhyVisible;
    BOUNDACTION		jupiPhyBoundAction;
    RECT	jupiPhyRectCheckBox;

public:
    JupiterPhysics();	
    JupiterPhysics(RECT rObject,RECT rBound);
    JupiterPhysics(RECT rObject,RECT rBound,POINTF ptFocus,POINTF ptVelo,POINTF ptAccelerate,POINTF ptDes,bool bMove);
    ~JupiterPhysics();

    void setObjectRect(RECT r)
    {
        CopyRect(&jupiPhyRectObject,&r);
        setCheckBox(jupiPhyRectObject);
    };
    void setObjectRect(int left, int top, int right, int bottom)
    {
        jupiPhyRectObject.left = left;
        jupiPhyRectObject.top = top;
        jupiPhyRectObject.right = right;
        jupiPhyRectObject.bottom = bottom;
    };
    RECT getObjectRect() { return jupiPhyRectObject; };
    int getWidth() { return jupiPhyRectObject.right-jupiPhyRectObject.left; };
    int getHeight() { return jupiPhyRectObject.bottom-jupiPhyRectObject.top; };
    void setFocus(POINTF ptFocus)
    {	
        jupiPhyPtFocus = ptFocus;
        jupiPhyRectFocusBound.left = (int)jupiPhyPtFocus.x + jupiPhyRectBound.left;	
        jupiPhyRectFocusBound.top = (int)jupiPhyPtFocus.y + jupiPhyRectBound.top;
        jupiPhyRectFocusBound.right = jupiPhyRectBound.right - 
            (getWidth() - (int)jupiPhyPtFocus.x);
        jupiPhyRectFocusBound.bottom = jupiPhyRectBound.bottom - 
            (getHeight() - (int)jupiPhyPtFocus.y);
    };
    void setFocus(float x,float y)
    {	
        POINTF pt={x,y};
        setFocus(pt);
    };
    POINTF getFocus() { return jupiPhyPtFocus; };
    RECT getFocusBound() { return jupiPhyRectFocusBound; };
    void setPos(POINTF ptPos)
    {	
        jupiPhyPtPos = ptPos;

        POINTF pt = ptPos - jupiPhyPtFocus;

        int w = jupiPhyRectObject.right - jupiPhyRectObject.left;
        int h = jupiPhyRectObject.bottom - jupiPhyRectObject.top;

        jupiPhyRectObject.left = (int)pt.x;
        jupiPhyRectObject.top = (int)pt.y;
        jupiPhyRectObject.right = (int)pt.x + w;
        jupiPhyRectObject.bottom = (int)pt.y + h;
        setCheckBox(jupiPhyRectObject);
    };
    void setPos(float x,float y)
    {	
        POINTF pt={x,y};
        setPos(pt);
    };
    POINTF getPos() { return jupiPhyPtPos; };
    void setLeftTop(POINTF pt)
    {
        int nWidth = jupiPhyRectObject.right - jupiPhyRectObject.left;
        int nHeight = jupiPhyRectObject.bottom - jupiPhyRectObject.top;

        jupiPhyRectObject.left = (int)pt.x;
        jupiPhyRectObject.top = (int)pt.y;
        jupiPhyRectObject.right = (int)pt.x + nWidth;
        jupiPhyRectObject.bottom = (int)pt.y + nHeight;

        setCheckBox(jupiPhyRectObject);
    };
    void setLeftTop(int x,int y)
    {
        int nWidth=jupiPhyRectObject.right-jupiPhyRectObject.left;
        int nHeight=jupiPhyRectObject.bottom-jupiPhyRectObject.top;

        jupiPhyRectObject.left=x;
        jupiPhyRectObject.top=y;
        jupiPhyRectObject.right=x+nWidth;
        jupiPhyRectObject.bottom=y+nHeight;

        setCheckBox(jupiPhyRectObject);

    };		
    POINTF getLeftTop()
    {
        POINTF pt={(float)jupiPhyRectObject.left, (float)jupiPhyRectObject.top};
        return pt;
    };

    void setRectBound(RECT rc) { CopyRect(&jupiPhyRectBound,&rc); };
    void setRectBound(int left,int top,int right,int bottom)
    {
        jupiPhyRectBound.left=left;
        jupiPhyRectBound.top=top;
        jupiPhyRectBound.right=right;
        jupiPhyRectBound.bottom=bottom;
    };
    RECT getRectBound() { return jupiPhyRectBound; };
    void setDes(POINTF pt)
    {
        if(pt.x<jupiPhyRectFocusBound.left)
            pt.x=(float)jupiPhyRectFocusBound.left;
        else if(pt.x>jupiPhyRectFocusBound.right)
            pt.x=(float)jupiPhyRectFocusBound.right;

        if(pt.y<jupiPhyRectFocusBound.top)
            pt.y=(float)jupiPhyRectFocusBound.top;
        else if(pt.y>jupiPhyRectFocusBound.bottom)
            pt.y=(float)jupiPhyRectFocusBound.bottom;

        jupiPhyPtDes=pt;
        jupiPhyPathArrive=FALSE;
    };
    void setDes(float x,float y)
    {
        POINTF pt={x,y};
        setDes(pt);
    };
    POINTF getDes() { return jupiPhyPtDes; };

    bool getPathArrive() { return jupiPhyPathArrive; }
    void setVelo(POINTF ptVelo)
    {
        jupiPhyPtVelo = ptVelo;
        jupiPhyStep = (float)hypot(ptVelo.x,ptVelo.y);

        if(fabs(jupiPhyPtVelo.x)<0.001 && fabs(jupiPhyPtVelo.y)<0.001)
            jupiPhyDirect=DI_STOP;
        else if((jupiPhyPtVelo.x+jupiPhyPtVelo.y)>0)
        {
            if(jupiPhyPtVelo.x>=jupiPhyPtVelo.y)
                jupiPhyDirect=DI_RIGHT;
            else
                jupiPhyDirect=DI_DOWN;
        }
        else
        {
            if(jupiPhyPtVelo.x>jupiPhyPtVelo.y)
                jupiPhyDirect=DI_UP;
            else
                jupiPhyDirect=DI_LEFT;
        }
    };
    void setVelo(float x, float y)
    {
        POINTF pt={x,y};
        setVelo(pt);
    };
    POINTF getVelo() { return jupiPhyPtVelo; };
    DIRECTION getDirect() { return jupiPhyDirect; };
    void setAccelerate(POINTF ptAccelerate){ jupiPhyPtAccelerate=ptAccelerate; };
    void setAccelerate(float x,float y)
    {
        jupiPhyPtAccelerate.x=x;
        jupiPhyPtAccelerate.y=y;
    };
    void setMoveState(bool bMove) { jupiPhyMove=bMove; };
    bool getMoveState() { return jupiPhyMove; };
    void setVisible(bool bVisible) { jupiPhyVisible=bVisible; };
    bool getVisible() {return jupiPhyVisible; };
    void setObject(RECT rObject, RECT rBound);
    void setObject(RECT rObject, RECT rBound, POINTF ptFocus, POINTF ptVelo, 
                    POINTF ptAccelerate, POINTF ptDes, bool bMove);
    void uniformMove();
    void shiftMove();
    void moveToDes();
    void moveAlongPath(POINTF * ptDesPath,int nPtCount,bool bCycle=FALSE);
    void moveDirect(DIRECTION Direct);
    bool isPOINTFInBound(POINTF pt,RECT r);
    bool checkErr(bool bRectify);
    void setCheckBox(RECT rObject)
    {
        int w = (int)((rObject.right - rObject.left)*0.1);
        int h = (int)((rObject.bottom - rObject.top)*0.1);

        jupiPhyRectCheckBox.left = rObject.left + w;
        jupiPhyRectCheckBox.right = rObject.right - w;
        jupiPhyRectCheckBox.top = rObject.top + h;
        jupiPhyRectCheckBox.bottom = rObject.bottom - h;
    }
    RECT getCheckBox() { return jupiPhyRectCheckBox; }
    void uniformMove2(BOUNDACTION action);
    bool collision(JupiterPhysics* ph, BOUNDACTION action, RECT* rcollision);

};

#endif // __JUPITER_PHYSICS_H__