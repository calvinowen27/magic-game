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

    bool init(std::string type, Vector2 pos) override
    {
        Object::init(type, pos);
        pCollider->doCollisions = false;
        return true;
    }
};

#endif