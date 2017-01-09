#ifndef OBJ_H
#define OBJ_H

#include "MyVector.h"

class Obj
{
public:
    Obj::Obj() : pos(MyVector(0, 0))
    {
    }

    Obj::~Obj()
    {
    }
    MyVector getPos() { return pos; }
    void setPos(MyVector newVec) { pos = newVec; }

    enum OBJTYPE
    {
        TABLE,
        DOOR,
        MAX_TYPE
    };
    OBJTYPE objType;
private:
    MyVector pos;
};

#endif