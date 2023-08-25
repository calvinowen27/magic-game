#ifndef GRASS_INCLUDE
#define GRASS_INCLUDE

#include "Vector2.hpp"
#include "Object.hpp"

class Grass : public Object
{
public:
    Grass() : Object()
    {
    }

    bool init(EntityType entityType, Vector2 pos) override
    {
        Object::init(entityType, pos);
        pCollider->doCollisions = false;
        return true;
    }
};

#endif