#ifndef WALL_INCLUDE
#define WALL_INCLUDE

#include "Vector2.hpp"
#include "Object.hpp"

class Wall : public Object
{
public:
    Wall(Vector2 pos) : Object("wall")
    {
        init(pos, Vector2(1, 1));
    }
};

#endif