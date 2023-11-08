#include "../../include/game/Objects/Wall.hpp"

Wall::Wall() : Object()
{
}

bool Wall::init(WallState state, Vector2 pos)
{
    Object::init(EntityType::Wall, pos);

    this->state = state;

    return true;
}