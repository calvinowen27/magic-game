#ifndef GRASS_INCLUDE
#define GRASS_INCLUDE

#include "../Util/Vector2.hpp"
#include "../Objects/Object.hpp"

class Grass : public Object
{
public:
    Grass() : Object()
    {
    }

    bool init(EntityType entityType, Vector2 pos)
    {
        Object::init(entityType, pos, Vector2(1, 0.5));
        pCollider->doCollisions = false;
        return true;
    }
};

#endif