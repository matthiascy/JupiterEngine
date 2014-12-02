/*
 *      Copyright(C) 2014, ChenYang, Jupiter-org.
 *
 *        _______
 *       /\      \                __      __
 *       \ /-\ \-/               /\ \    /\ \
 *          \ \ \  __   __  _____\/_/   _\_\ \__   ____  ____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\ /`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/___/\ \_ \  __/ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\   \ \_\\ \____\ \_\
 *         \/______/\/____/_/\ \ \   \/_/    \/_/ \/____/\/_/
 *                            \ \_\
 *                             \/_/
 *      @file:      jupiter_physics.h
 *      @author:    ChenYang :);GuoYan
 *      @date:      2014/11/28 10:24
 *      @history:   2014/11/24 20:40
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
    BA_STOP = 0, BA_LOOP = 1, BA_REBOUND = 2, BA_CLEAR = 3
};

// Direction
enum DIRECTION {
    DI_STOP = 0, DI_LEFT = 1, DI_RIGHT = 2, DI_UP = 3, DI_DOWN = 4
};

typedef struct Point {
public:
    float x;
    float y;

    const Point& operator=(const Point& pt)   
    {   
        x=pt.x;
        y=pt.y;
        return *this ;
    }
    const Point operator+(const Point& pt)
    {	
        Point ptTmp;
        ptTmp.x = x+pt.x;
        ptTmp.y = y+pt.y;
        return ptTmp;
    }
    const Point operator-(const Point& pt)
    {
        Point ptTmp;
        ptTmp.x = x - pt.x;
        ptTmp.y = y - pt.y;
        return ptTmp;
    }
} Point;

class JupiterPhysics
{
protected:
    Point	jupiPhyPtFocus;  // Object's focus coord, relative to left-top.
    Point	jupiPhyPtPos;    // Object's position in the world coord.
    RECT	jupiPhyRectObject;      // Object's rectangle region.
    RECT	jupiPhyRectFocusBound;  // Focus' range of motion.
    RECT	jupiPhyRectBound;       // Object's range of motion.


    Point	jupiPhyPtVelo;          // Velocity vector.
    Point	jupiPhyPtAccelerate;    // Acceleration.
    float	jupiPhyStep;  // Movement stride, same with velocity magnitude.
    Point	jupiPhyPtDes; // Destination position.

    DIRECTION	jupiPhyDirect;         // Movement direction.

    bool	jupiPhyMove;           // Motion state.

    bool	jupiPhyPathArrive;     // Reach breakpoint?
    int		jupiPhyPathIndex;      // Path index, ie. breakpoint.
    bool	jupiPhyInitIndex;      // Initialized path index?

    bool	jupiPhyVisible;        // Object's visibility.
    BOUNDACTION	jupiPhyBoundAction;    // Actions after collision
    RECT	jupiPhyRectCheckBox;   // Collision detection box.

public:
    JupiterPhysics();

    // Constructs objects with specified object rectangle and motion rectangle.
    JupiterPhysics(RECT rObject, RECT rBound);

    // Constructs objects with specified arguments.
    // @params: rObject:       Object rectangle.
    //          rBound:        Object motion rectangle.
    //          ptFocus:       Object focus position.
    //          ptVelo:        Object velocity.
    //          ptAccelerate:  Object acceleration.
    //          ptDes:         Object destination position.
    //          pMove:         Object move status.
    JupiterPhysics(RECT rObject, RECT rBound, Point ptFocus,
                   Point ptVelo, Point ptAccelerate, Point ptDes,
                   bool bMove);

    ~JupiterPhysics();
    
    // Set object rectangle.
    void setObjectRect(RECT r);
    // Overloaded setObjectRect with specified RECT params.
    void setObjectRect(int left, int top, int right, int bottom);

    // @return type: int
    // Return object rectangle.
    RECT getObjectRect() { return jupiPhyRectObject; };

    // @return type: int
    // Return object width.
    int getWidth() { return jupiPhyRectObject.right - jupiPhyRectObject.left;};

    // @return type: int
    // Return object height.
    int getHeight() { return jupiPhyRectObject.bottom - jupiPhyRectObject.top;};

    // Set object focus position(offset to left-top corner) and object
    // focus motion range.
    void setFocus(Point ptFocus);
    // Overloaded setFocus(...).
    void setFocus(float x, float y);

    // @return type: Point
    // Return object focus. 
    Point getFocus() { return jupiPhyPtFocus; };

    // @return type: RECT
    // Return focus motion range.
    RECT getFocusBound() { return jupiPhyRectFocusBound; };

    // Set object position in world coord.
    void setPos(Point ptPos);
    // Overloaded setPos(...).
    void setPos(float x, float y);
    
    // @return type: Point
    // Return object position.
    Point getPos() { return jupiPhyPtPos; };

    // Set object left-top position.
    void setLeftTop(Point pt);
    // Overloaded setLeftTop(...).
    void setLeftTop(int x,int y);
    // Get object left-top position.
    Point getLeftTop();
    
    // Set object motion rectangle.
    void setRectBound(RECT rc) { CopyRect(&jupiPhyRectBound, &rc); };
    // Overloaded setRectBound(...).
    void setRectBound(int left, int top, int right, int bottom);
    // @return type: RECT
    // Return object motion rectangle.
    RECT getRectBound() { return jupiPhyRectBound; };

    // Set object movement destination.
    void setDes(Point pt);
    // Overloaded setDes(...);
    void setDes(float x, float y);
    // @return type: Point
    // Return object destination.
    Point getDes() { return jupiPhyPtDes; };

    // Reach breakpoint?
    bool getPathArrive() { return jupiPhyPathArrive; }

    // Set object velocity, if velocity changed, move stride(step) also will be
    // changed.
    void setVelo(Point ptVelo);
    // Overloaded setVelo(...).
    void setVelo(float x, float y);
    // @return type: Point.
    // Return object velocity.
    Point getVelo() { return jupiPhyPtVelo; };
    
    // Return object move direction.
    DIRECTION getDirect() { return jupiPhyDirect; };

    // Set object acceleration.
    void setAccelerate(Point ptAccelerate) { jupiPhyPtAccelerate = 
        ptAccelerate; };
    // Overloaded setAccelerate(...).
    void setAccelerate(float x, float y);

    // Set object move state.
    void setMoveState(bool bMove) { jupiPhyMove = bMove; };
    // Is moving?
    bool getMoveState() { return jupiPhyMove; };

    // Set object visibility.
    void setVisible(bool bVisible) { jupiPhyVisible = bVisible; };
    // Visible or not.
    bool getVisible() {return jupiPhyVisible; };

    void setObject(RECT rObject, RECT rBound);
    void setObject(RECT rObject, RECT rBound, Point ptFocus, Point ptVelo, 
                    Point ptAccelerate, Point ptDes, bool bMove);

    // Uniform motion.
    void uniformMove();
    // Shift motion.
    void shiftMove();

    // Move to destination.
    void moveToDes();

    // Move along path which specified by ptDesPath.
    // @params: ptDesPath -- Point vector that stores checkpoint of path.
    //          nPtCount -- Checkpoint counts.
    //          bCycle -- Repeat or not, false by default.
    void moveAlongPath(Point* ptDesPath, int nPtCount,
                       bool bCycle = false);

    // Set object move direction.
    void moveDirect(DIRECTION Direct);

    // Decides whether the object is moving in the bounded rectangle.
    bool isPointInBound(Point pt, RECT r);
    
    // Test object's position, inside or outside of bounding box.
    // inside -- return false.
    // outside -- return true.
    // if bRectify if true, set object at the edge of bounding box.
    bool checkErr(bool bRectify);

    // Set object collision rectangle.
    void setCheckBox(RECT rObject);
    // Return object collision rectangle.
    RECT getCheckBox() { return jupiPhyRectCheckBox; }
    
    // Uniform motion with velocity = juptPhyPtVelo, and detect collision with
    // border, if collision happened, move as action specified.
    void uniformMove2(BOUNDACTION action);

    // Two objects move in themselves' speed, detect collision between them.
    bool collision(JupiterPhysics* ph, BOUNDACTION action, RECT* rcollision);

};

#endif // __JUPITER_PHYSICS_H__