#include "../include/game/Registry.hpp"

Registry::Registry()
{
}

Registry::~Registry()
{
    for (auto transform : _transformComponents)
    {
        delete transform;
    }
}

void Registry::update(float time)
{
    for (auto transform : _transformComponents)
    {
        transform->update(time);
    }
}

void Registry::draw(SDL_Renderer *pRenderer)
{
}