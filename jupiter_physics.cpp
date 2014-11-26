/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__  ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/___/\ \_/\  __/ \ \_/
 *         /_\ \ \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \__\_\/_/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
 *                            \ \_\
 *                             \/_/
 *      @file:      jupiter_physics.cpp
 *      @author:    ChenYang :)
 *      @date:      2014/11/24 20:56
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
    setObject(rObject,rBound);
}

JupiterPhysics::JupiterPhysics(RECT rObject, RECT rBound, POINTF ptFocus,
                               POINTF ptVelo, POINTF ptAccelerate, 
                               POINTF ptDes, bool bMove)
{
    setObject(rObject,rBound,ptFocus,ptVelo,ptAccelerate,ptDes,bMove);
}

JupiterPhysics::~JupiterPhysics()
{

}

void JupiterPhysics::setObject(RECT rObject,RECT rBound)
{
    POINTF ptPos={(float)rObject.left,(float)rObject.top};
    POINTF ptFocus={0,0};
    POINTF ptVelo={0,0};
    POINTF ptAccelerate={0,0};
    POINTF ptDes=ptPos;
    setObject(rObject,rBound,ptFocus,ptVelo,ptAccelerate,ptDes,false);
    jupiPhyPathArrive=true;
}

void JupiterPhysics::setObject(RECT rObject, RECT rBound, POINTF ptFocus,
                               POINTF ptVelo,POINTF ptAccelerate,
                               POINTF ptDes, bool bMove)
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

bool  JupiterPhysics::isPOINTFInBound(POINTF pt, RECT r)
{
    if(pt.x>=r.left && pt.x<=r.right
        && pt.y>=r.top && pt.y<=r.bottom)
        return true;
    else
        return false;
}

void JupiterPhysics::uniformMove()
{
    POINTF ptAccelerate={0,0};	

    setAccelerate(ptAccelerate);

    shiftMove();
}

void	JupiterPhysics::shiftMove()
{
    if(! getMoveState())
        return ;

    POINTF pt;
    pt=jupiPhyPtPos+jupiPhyPtVelo;

    if(!isPOINTFInBound(pt,jupiPhyRectFocusBound)) {
        moveToDes();
        setMoveState(false);
        jupiPhyPtVelo.x=0;
        jupiPhyPtVelo.y=0;
        jupiPhyPtAccelerate.x=0;
        jupiPhyPtAccelerate.y=0;
    } else {
        setPos(pt);
        POINTF ptv=jupiPhyPtVelo;
        ptv=ptv+jupiPhyPtAccelerate;
        setVelo(ptv);
    }
}

void JupiterPhysics::moveToDes()
{
    if(!getMoveState())
        return ;

    float sx = jupiPhyPtDes.x - jupiPhyPtPos.x;
    float sy = jupiPhyPtDes.y - jupiPhyPtPos.y;

    if(hypot(sx,sy)<jupiPhyStep) {
        setPos(jupiPhyPtDes);
        setMoveState(false);
        return ;
    }

    float x = (float)(jupiPhyStep*sx / sqrt(sx*sx+sy*sy));
    float y = (float)(jupiPhyStep*sy / sqrt(sx*sx+sy*sy));

    POINTF pt={x,y};
    setVelo(pt);

    POINTF pt1 = {jupiPhyPtPos.x+x, jupiPhyPtPos.y+y};
    setPos(pt1);
}

void JupiterPhysics::moveAlongPath(POINTF * ptDesPath, int nPtCount, bool bCycle)
{
    if(!jupiPhyInitIndex) {
        jupiPhynPathIndex = 0;
        jupiPhyInitIndex = true;
    }

    if(jupiPhynPathIndex <nPtCount) {
        setMoveState(true);
        setDes(ptDesPath[jupiPhynPathIndex]);
        moveToDes();
        if(!getMoveState())
            jupiPhynPathIndex++;
        return;
    } else {
        jupiPhyInitIndex=false;
        if(bCycle) {
            jupiPhyPathArrive=false;		
        } else {
            jupiPhyPathArrive=true;
        }
    }
}

void JupiterPhysics::moveDirect(DIRECTION Direct)
{
    POINTF ptDes;
    switch(Direct) {
        case DI_LEFT:
            ptDes.x = jupiPhyPtPos.x-jupiPhyStep;
            ptDes.y = jupiPhyPtPos.y;
            break;

        case DI_RIGHT:
            ptDes.x = jupiPhyPtPos.x+jupiPhyStep;
            ptDes.y = jupiPhyPtPos.y;
            break;

        case DI_UP:
            ptDes.x = jupiPhyPtPos.x;
            ptDes.y = jupiPhyPtPos.y-jupiPhyStep;
            break;

        case DI_DOWN:
            ptDes.x = jupiPhyPtPos.x;
            ptDes.y = jupiPhyPtPos.y+jupiPhyStep;
            break;	

        default:
            ptDes = jupiPhyPtPos;
            break;
    }

    setPos(ptDes);
};

void	JupiterPhysics::uniformMove2(BOUNDACTION action)
{
    if(! getMoveState())
        return;

    POINTF pt;				
    pt=jupiPhyPtPos+jupiPhyPtVelo;

    if(isPOINTFInBound(pt,jupiPhyRectFocusBound)) {
        setPos(pt);
        return;
    }

    switch(action) {
        case BA_stop:
            moveToDes();
            setMoveState(false);
            break;

        case BA_LOOP:
            if(pt.x > jupiPhyRectFocusBound.right)
                pt.x = (float)jupiPhyRectFocusBound.left;
            else if(pt.x < jupiPhyRectFocusBound.left)
                pt.x = (float)jupiPhyRectFocusBound.right;
            else if(pt.y > jupiPhyRectFocusBound.bottom)
                pt.y = (float)jupiPhyRectFocusBound.top;
            else if(pt.y < jupiPhyRectFocusBound.top)
                pt.y = (float)jupiPhyRectFocusBound.bottom;
            setPos(pt);
            break;


        case BA_REBOUND:
            if(pt.x>=jupiPhyRectFocusBound.right || pt.x<=jupiPhyRectFocusBound.left)
                jupiPhyPtVelo.x=-jupiPhyPtVelo.x;
            else if(pt.y>=jupiPhyRectFocusBound.bottom || pt.y<=jupiPhyRectFocusBound.top)
                jupiPhyPtVelo.y=-jupiPhyPtVelo.y;
            setVelo(jupiPhyPtVelo);
            break;

        case BA_CLEAR:
            setVisible(false);
            break;
    }
}

bool	JupiterPhysics::collision(	JupiterPhysics * ph, BOUNDACTION action,RECT * prcollision)
{
    if(!(getVisible()&&ph->getVisible()))
        return false;

    if(IntersectRect(prcollision, &getCheckBox(), &(ph->getCheckBox())))
    {
        POINTF pt1,pt2;

        switch(action) {
            case BA_stop:
                setMoveState(false);
                ph->setMoveState(false);
                break;

            case BA_CLEAR:
                setVisible(false);
                ph->setVisible(false);
                break;

            case BA_REBOUND:
                if(getVelo().x * ph->getVelo().x>=0) {
                    pt1.x=getVelo().x;
                    pt2.x=ph->getVelo().x;
                } else {
                    pt1.x=-getVelo().x;
                    pt2.x=-ph->getVelo().x;
                }
                
                if(getVelo().y * ph->getVelo().y>=0) {
                    pt1.y=getVelo().y;
                    pt2.y=ph->getVelo().y;
                } else {
                    pt1.y=-getVelo().y;
                    pt2.y=-ph->getVelo().y;
                }

                if(getVelo().x * ph->getVelo().x>=0 &&
                    getVelo().y * ph->getVelo().y>=0) {
                    pt2=getVelo();
                    pt1=ph->getVelo();
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
    if(isPOINTFInBound(jupiPhyPtPos,jupiPhyRectFocusBound))
        return false;
    if(bRectify) {
        POINTF pt = jupiPhyPtPos;

        if(jupiPhyRectFocusBound.left > jupiPhyPtPos.x)
            pt.x = (float)jupiPhyRectFocusBound.left;
        else if(jupiPhyRectFocusBound.right < jupiPhyPtPos.x)
            pt.x = (float)jupiPhyRectFocusBound.right;

        if(jupiPhyRectFocusBound.top > jupiPhyPtPos.y)
            pt.y = (float)jupiPhyRectFocusBound.top;
        else if(jupiPhyRectFocusBound.bottom < jupiPhyPtPos.y)
            pt.y = (float)jupiPhyRectFocusBound.bottom;

        setPos(pt);
        setMoveState(false);
    }
    return true;
}
/*
POINTF::POINTF(): x(0), y(0)
{

}

POINTF::~POINTF()
{

}

POINTF& POINTF::operator=(const POINTF& rhs)
{
if (this == &rhs)
return *this;
else {
this->x = rhs.x;
this->y = rhs.y;
return *this;
}
}

POINTF POINTF::operator+(const POINTF& p)
{
this->x += p.x; this->y += p.y;
return *this;
}

POINTF POINTF::operator-(const POINTF& p)
{
this->x -= p.x; this->y -= p.y;
return *this;
}
*/