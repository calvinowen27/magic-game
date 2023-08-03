#include "../include/game/CollisionBox.hpp"

CollisionBox::CollisionBox()
{
    _start = Vector2::zero;
    _end = Vector2(1, 1);
}

CollisionBox::CollisionBox(Vector2 relPos, Vector2 relDims)
{
    _start = relPos;
    _end = relDims;
}