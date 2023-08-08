#ifndef GRASS_INCLUDE
#define GRASS_INCLUDE

#include "Vector2.hpp"
#include "OBject.hpp"

class Grass : public Object
{
public:
    Grass(Vector2 pos) : Object("grass")
    {
        init(pos, Vector2(1, 1), false);
    }
};

#endif