#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/WorldManager.hpp"

Registry::Registry()
{
}

Registry::~Registry()
{
}

void Registry::update(float time)
{
    for (auto transform : getComponents<TransformComponent>())
    {
        transform->update(time);
    }
}

void Registry::draw(SDL_Renderer *pRenderer)
{
    // y value render order
    for(float y = (WorldManager::WORLD_SIZE / 2) - 1; y > -(WorldManager::WORLD_SIZE / 2); y -= 0.5f)
    {
        for (auto renderer : TypeVector<RendererComponent>::getVector())
        {
            if((float)((int)(renderer->pTransform->pos.y*4))/2.f == y)
                renderer->draw(pRenderer);
        }
    }
}