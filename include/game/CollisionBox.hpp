#ifndef COLLISION_BOX_INCLUDE
#define COLLISION_BOX_INCLUDE

#include "Vector2.hpp"

struct CollisionBox
{
private:
    Vector2 _start;
    Vector2 _end;

public:
    CollisionBox();
    CollisionBox(Vector2 relPos, Vector2 relDims);
    inline Vector2 start() { return _start; }
    inline Vector2 end() { return _end; }
};

#endif