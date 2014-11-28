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
 *      @file:      jupiter_physics.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/28 10:24
 *      @history:   2014/11/24 20:40
 *      @brief:     Implementation of JupiterPhysics. It's a little bit hard to
 *                  get these down. Come on! Have fun! :-)
 */

#include "jupiter_physics.h"

JupiterPhysics::JupiterPhysics()
{
    RECT rObject = {0, 0, 0, 0};
    RECT rBound = {0, 0, 0, 0};
    setObject(rObject, rBound);
}

JupiterPhysics::JupiterPhysics(RECT rObject, RECT rBound)
{
    setObject(rObject, rBound);
}

JupiterPhysics::JupiterPhysics(RECT rObject, RECT rBound, Point ptFocus,
                               Point ptVelo, Point ptAccelerate, 
                               Point ptDes, bool bMove)
{
    setObject(rObject, rBound, ptFocus, ptVelo, ptAccelerate, ptDes, bMove);
}

JupiterPhysics::~JupiterPhysics()
{

}

void JupiterPhysics::setObjectRect(RECT r)
{
    CopyRect(&jupiPhyRectObject, &r);
    setCheckBox(jupiPhyRectObject);     // Collision detection rectangle.
};

void JupiterPhysics::setObjectRect(int left, int top, int right, int bottom)
{
    jupiPhyRectObject.left = left;
    jupiPhyRectObject.top = top;
    jupiPhyRectObject.right = right;
    jupiPhyRectObject.bottom = bottom;
};

//         Object motion range.
//          /
//         ---------------------------
//         |  |-------------------|  |
//         | -|-                  |  |
//         || 0 |\                |\ |
//         |/-|-  Object          | \|
//         /  |-------------------|  |\
//        /--------------------------- \
//       /                            Focus motion range.
//      Object focus.
void JupiterPhysics::setFocus(Point ptFocus)
{	
    jupiPhyPtFocus = ptFocus;
    jupiPhyRectFocusBound.left = (int)jupiPhyPtFocus.x + jupiPhyRectBound.left;	
    jupiPhyRectFocusBound.top = (int)jupiPhyPtFocus.y + jupiPhyRectBound.top;
    jupiPhyRectFocusBound.right = jupiPhyRectBound.right - 
        (getWidth() - (int)jupiPhyPtFocus.x);
    jupiPhyRectFocusBound.bottom = jupiPhyRectBound.bottom - 
        (getHeight() - (int)jupiPhyPtFocus.y);
};

void JupiterPhysics::setFocus(float x, float y)
{	
    Point pt = {x, y};
    setFocus(pt);
};

void JupiterPhysics::setPos(Point ptPos)
{	
    jupiPhyPtPos = ptPos;

    Point pt = ptPos - jupiPhyPtFocus;

    int w = jupiPhyRectObject.right - jupiPhyRectObject.left;
    int h = jupiPhyRectObject.bottom - jupiPhyRectObject.top;

    jupiPhyRectObject.left = (int)pt.x;
    jupiPhyRectObject.top = (int)pt.y;
    jupiPhyRectObject.right = (int)pt.x + w;
    jupiPhyRectObject.bottom = (int)pt.y + h;
    setCheckBox(jupiPhyRectObject);
};

void JupiterPhysics::setPos(float x,float y)
{	
    Point pt = {x, y};
    setPos(pt);
};

void JupiterPhysics::setLeftTop(Point pt)
{
    int width = jupiPhyRectObject.right - jupiPhyRectObject.left;
    int height = jupiPhyRectObject.bottom - jupiPhyRectObject.top;

    jupiPhyRectObject.left = (int)pt.x;
    jupiPhyRectObject.top = (int)pt.y;
    jupiPhyRectObject.right = (int)pt.x + width;
    jupiPhyRectObject.bottom = (int)pt.y + height;

    setCheckBox(jupiPhyRectObject);
};

void JupiterPhysics::setLeftTop(int x, int y)
{
    int width = jupiPhyRectObject.right - jupiPhyRectObject.left;
    int height = jupiPhyRectObject.bottom - jupiPhyRectObject.top;

    jupiPhyRectObject.left = x;
    jupiPhyRectObject.top = y;
    jupiPhyRectObject.right = x + width;
    jupiPhyRectObject.bottom = y + height;

    setCheckBox(jupiPhyRectObject);

};

Point JupiterPhysics::getLeftTop()
{
    Point pt = {
        (float)jupiPhyRectObject.left,
        (float)jupiPhyRectObject.top
    };
    return pt;
};

void JupiterPhysics::setRectBound(int left, int top, int right, int bottom)
{
    jupiPhyRectBound.left = left;
    jupiPhyRectBound.top = top;
    jupiPhyRectBound.right = right;
    jupiPhyRectBound.bottom = bottom;
};

void JupiterPhysics::setDes(Point pt)
{
    // If object's destination is outside of the motion rectangle, clamp it
    // to rectangle size.
    if (pt.x < jupiPhyRectFocusBound.left)
        pt.x = (float)jupiPhyRectFocusBound.left;
    else if (pt.x > jupiPhyRectFocusBound.right)
        pt.x = (float)jupiPhyRectFocusBound.right;
    if (pt.y < jupiPhyRectFocusBound.top)
        pt.y = (float)jupiPhyRectFocusBound.top;
    else if (pt.y > jupiPhyRectFocusBound.bottom)
        pt.y = (float)jupiPhyRectFocusBound.bottom;

    jupiPhyPtDes = pt;
    jupiPhyPathArrive = false;
};

void JupiterPhysics::setDes(float x, float y)
{
    Point pt = {x, y};
    setDes(pt);
};

void JupiterPhysics::setVelo(Point ptVelo)
{
    jupiPhyPtVelo = ptVelo;
    jupiPhyStep = (float)hypot(ptVelo.x, ptVelo.y);

    if (fabs(jupiPhyPtVelo.x)<0.001 && fabs(jupiPhyPtVelo.y)<0.001)
        jupiPhyDirect = DI_STOP;
    else if ((jupiPhyPtVelo.x + jupiPhyPtVelo.y) > 0) {
        if (jupiPhyPtVelo.x >= jupiPhyPtVelo.y)
            jupiPhyDirect = DI_RIGHT;
        else
            jupiPhyDirect = DI_DOWN;
    } else {
        if (jupiPhyPtVelo.x > jupiPhyPtVelo.y)
            jupiPhyDirect = DI_UP;
        else
            jupiPhyDirect = DI_LEFT;
    }
};

void JupiterPhysics::setVelo(float x, float y)
{
    Point pt = {x, y};
    setVelo(pt);
};

void JupiterPhysics::setAccelerate(float x, float y)
{
    jupiPhyPtAccelerate.x = x;
    jupiPhyPtAccelerate.y = y;
};

void JupiterPhysics::setObject(RECT rObject, RECT rBound)
{
    Point ptPos = {
        (float)rObject.left, 
        (float)rObject.top
    };
    Point ptFocus = {0, 0};
    Point ptVelo = {0, 0};
    Point ptAccelerate = {0, 0};
    Point ptDes = ptPos;
    setObject(rObject, rBound, ptFocus, ptVelo, ptAccelerate, ptDes, false);
    jupiPhyPathArrive = true;
}

void JupiterPhysics::setObject(RECT rObject, RECT rBound, Point ptFocus,
                               Point ptVelo, Point ptAccelerate,
                               Point ptDes, bool bMove)
{
    setObjectRect(rObject);
    setRectBound(rBound);
    setFocus(ptFocus);
    setPos((float)rObject.left, (float)rObject.top);
    setMoveState(bMove);
    jupiPhyInitIndex = false;
    setVisible(true);
    setDes(ptDes);
    setVelo(ptVelo);
    setAccelerate(ptAccelerate);
}

bool JupiterPhysics::isPointInBound(Point pt, RECT r)
{
    if (pt.x>=r.left && pt.x<=r.right
        && pt.y>=r.top && pt.y<=r.bottom)
        return true;
    else
        return false;
}

void JupiterPhysics::uniformMove()
{
    Point ptAccelerate = {0, 0};	
    setAccelerate(ptAccelerate);
    shiftMove();
}

void JupiterPhysics::shiftMove()
{
    if (!getMoveState())
        return ;

    Point pt;
    pt = jupiPhyPtPos + jupiPhyPtVelo;

    if (!isPointInBound(pt, jupiPhyRectFocusBound)) {
        moveToDes();
        setMoveState(false);
        jupiPhyPtVelo.x = 0;
        jupiPhyPtVelo.y = 0;
        jupiPhyPtAccelerate.x = 0;
        jupiPhyPtAccelerate.y = 0;
    } else {
        setPos(pt);
        Point ptv = jupiPhyPtVelo;
        ptv = ptv + jupiPhyPtAccelerate;
        setVelo(ptv);
    }
}

void JupiterPhysics::moveToDes()
{
    if (!getMoveState())
        return ;

    float sx = jupiPhyPtDes.x - jupiPhyPtPos.x;
    float sy = jupiPhyPtDes.y - jupiPhyPtPos.y;

    if (hypot(sx,sy)<jupiPhyStep) {
        setPos(jupiPhyPtDes);
        setMoveState(false);
        return ;
    }

    float x = (float)(jupiPhyStep*sx / sqrt(sx*sx+sy*sy));
    float y = (float)(jupiPhyStep*sy / sqrt(sx*sx+sy*sy));

    Point pt={x,y};
    setVelo(pt);

    Point pt1 = {jupiPhyPtPos.x+x, jupiPhyPtPos.y+y};
    setPos(pt1);
}

void JupiterPhysics::moveAlongPath(Point * ptDesPath, int nPtCount,
                                   bool bCycle/* = false*/)
{
    if (!jupiPhyInitIndex) {
        jupiPhyPathIndex = 0;
        jupiPhyInitIndex = true;
    }

    if (jupiPhyPathIndex < nPtCount) {
        setMoveState(true);
        setDes(ptDesPath[jupiPhyPathIndex]);
        moveToDes();
        if(!getMoveState())
            jupiPhyPathIndex++;
        return;
    } else {
        jupiPhyInitIndex = false;
        if (bCycle) {
            jupiPhyPathArrive = false;		
        } else {
            jupiPhyPathArrive = true;
        }
    }
}

void JupiterPhysics::moveDirect(DIRECTION Direct)
{
    Point ptDes;
    switch(Direct) {
        case DI_LEFT:
            ptDes.x = jupiPhyPtPos.x - jupiPhyStep;
            ptDes.y = jupiPhyPtPos.y;
            break;

        case DI_RIGHT:
            ptDes.x = jupiPhyPtPos.x + jupiPhyStep;
            ptDes.y = jupiPhyPtPos.y;
            break;

        case DI_UP:
            ptDes.x = jupiPhyPtPos.x;
            ptDes.y = jupiPhyPtPos.y - jupiPhyStep;
            break;

        case DI_DOWN:
            ptDes.x = jupiPhyPtPos.x;
            ptDes.y = jupiPhyPtPos.y + jupiPhyStep;
            break;	

        default:
            ptDes = jupiPhyPtPos;
            break;
    }

    setPos(ptDes);
};

void JupiterPhysics::uniformMove2(BOUNDACTION action)
{
    if (! getMoveState())
        return;

    Point pt;				
    pt=jupiPhyPtPos+jupiPhyPtVelo;

    if (isPointInBound(pt,jupiPhyRectFocusBound)) {
        setPos(pt);
        return;
    }

    switch(action) {
        case BA_STOP:
            moveToDes();
            setMoveState(false);
            break;

        case BA_LOOP:
            if (pt.x > jupiPhyRectFocusBound.right)
                pt.x = (float)jupiPhyRectFocusBound.left;
            else if (pt.x < jupiPhyRectFocusBound.left)
                pt.x = (float)jupiPhyRectFocusBound.right;
            else if (pt.y > jupiPhyRectFocusBound.bottom)
                pt.y = (float)jupiPhyRectFocusBound.top;
            else if (pt.y < jupiPhyRectFocusBound.top)
                pt.y = (float)jupiPhyRectFocusBound.bottom;
            setPos(pt);
            break;


        case BA_REBOUND:
            if (pt.x>=jupiPhyRectFocusBound.right || pt.x<=jupiPhyRectFocusBound.left)
                jupiPhyPtVelo.x =- jupiPhyPtVelo.x;
            else if (pt.y>=jupiPhyRectFocusBound.bottom || pt.y<=jupiPhyRectFocusBound.top)
                jupiPhyPtVelo.y =- jupiPhyPtVelo.y;
            setVelo(jupiPhyPtVelo);
            break;

        case BA_CLEAR:
            setVisible(false);
            break;
    }
}

bool JupiterPhysics::collision(JupiterPhysics* ph, BOUNDACTION action,
                               RECT * prcollision)
{
    if (!(getVisible()&&ph->getVisible()))
        return false;

    if (IntersectRect(prcollision, &getCheckBox(), &(ph->getCheckBox()))) {
        Point pt1, pt2;

        switch(action) {
            case BA_STOP:
                setMoveState(false);
                ph->setMoveState(false);
                break;

            case BA_CLEAR:
                setVisible(false);
                ph->setVisible(false);
                break;

            case BA_REBOUND:
                if (getVelo().x * ph->getVelo().x>=0) {
                    pt1.x = getVelo().x;
                    pt2.x = ph->getVelo().x;
                } else {
                    pt1.x =- getVelo().x;
                    pt2.x =- ph->getVelo().x;
                }
                
                if (getVelo().y * ph->getVelo().y>=0) {
                    pt1.y = getVelo().y;
                    pt2.y = ph->getVelo().y;
                } else {
                    pt1.y =- getVelo().y;
                    pt2.y =- ph->getVelo().y;
                }

                if (getVelo().x * ph->getVelo().x>=0 &&
                    getVelo().y * ph->getVelo().y>=0) {
                    pt2 = getVelo();
                    pt1 = ph->getVelo();
                }

                setVelo(pt1);
                ph->setVelo(pt2);
                break;
        }
        return true;
    }
    else
        return false;
}

bool JupiterPhysics::checkErr(bool bRectify)
{	
    if (isPointInBound(jupiPhyPtPos, jupiPhyRectFocusBound))
        return false;
    if (bRectify) {
        Point pt = jupiPhyPtPos;

        if (jupiPhyRectFocusBound.left > jupiPhyPtPos.x)
            pt.x = (float)jupiPhyRectFocusBound.left;
        else if (jupiPhyRectFocusBound.right < jupiPhyPtPos.x)
            pt.x = (float)jupiPhyRectFocusBound.right;

        if (jupiPhyRectFocusBound.top > jupiPhyPtPos.y)
            pt.y = (float)jupiPhyRectFocusBound.top;
        else if (jupiPhyRectFocusBound.bottom < jupiPhyPtPos.y)
            pt.y = (float)jupiPhyRectFocusBound.bottom;

        setPos(pt);
        setMoveState(false);
    }
    return true;
}

void JupiterPhysics::setCheckBox(RECT rObject)
{
    int w = (int)((rObject.right - rObject.left) * 0.1);
    int h = (int)((rObject.bottom - rObject.top) * 0.1);

    jupiPhyRectCheckBox.left = rObject.left + w;
    jupiPhyRectCheckBox.right = rObject.right - w;
    jupiPhyRectCheckBox.top = rObject.top + h;
    jupiPhyRectCheckBox.bottom = rObject.bottom - h;
}