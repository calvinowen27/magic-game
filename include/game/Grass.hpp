#ifndef GRASS_INCLUDE
#define GRASS_INCLUDE

#include "Vector2.hpp"
#include "Object.hpp"

class Grass : public Object
{
public:
    Grass() : Object("Grass")
    {
    }

    bool init(Vector2 pos) override
    {
        Object::init(pos);
        pCollider->doCollisions = false;
    }
};

#endif