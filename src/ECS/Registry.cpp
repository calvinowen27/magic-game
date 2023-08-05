#include "../../include/game/ECS/Registry.hpp"

Registry::Registry()
{
}

Registry::~Registry()
{
    for (auto transform : _transforms)
    {
        delete transform;
    }
}

void Registry::update(float time)
{
    for (auto transform : _transforms)
    {
        transform->update(time);
    }
}

void Registry::draw(SDL_Renderer *pRenderer)
{
}