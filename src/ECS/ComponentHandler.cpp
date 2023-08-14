#include "../../include/game/ECS/ComponentHandler.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/WorldManager.hpp"
#include "../../include/game/ECS/Components.hpp"

#include <algorithm>

ComponentHandler::ComponentHandler() : _game(*Game::getInstance()), _registry(*_game.pRegistry)
{
}

void ComponentHandler::draw(SDL_Renderer *pRenderer)
{
    auto rendererSet = _registry.getComponents<RendererComponent>();
    auto renderers = std::vector<shared_ptr<RendererComponent>>(rendererSet.begin(), rendererSet.end());
    std::sort(renderers.begin(), renderers.end(), rendererComparator);

    for(auto renderer : renderers)
        renderer->draw(pRenderer);
}

void ComponentHandler::update(float time)
{
    updateTransforms(time);
    updateColliders(time);
}

void ComponentHandler::updateTransforms(float time)
{
    for (auto transform : _registry.getComponents<TransformComponent>())
    {
        transform->update(time);
    }
}

void ComponentHandler::updateColliders(float time)
{
    auto colliders = _registry.getComponents<ColliderComponent>();
    for (auto pCol : colliders)
    {
        pCol->update(time);

        Vector2 deltaPos = pCol->pRigidbody->velocity * time;
        Vector2 nextPos = pCol->pTransform->pos + deltaPos;

        for (auto pOther : colliders)
        {
            if (pCol == pOther)
                continue;

            if (!pCol->doCollisions || !pOther->doCollisions)
                continue;

            // skip if not touching
            if (pCol->leftX + deltaPos.x >= pOther->rightX || pCol->rightX + deltaPos.x <= pOther->leftX ||
                pCol->bottomY + deltaPos.y >= pOther->topY || pCol->topY + deltaPos.y <= pOther->bottomY)
                continue;

            // this left and other right
            if (pCol->borderEnabled[0] && pOther->borderEnabled[1])
            {
                if (pCol->leftX >= pOther->rightX && pCol->leftX + deltaPos.x < pOther->rightX)
                    nextPos.x = pOther->rightX - (pCol->start.x * pCol->pTransform->dims.x);
            }

            // this right and other left
            if (pCol->borderEnabled[1] && pOther->borderEnabled[0])
            {
                if (pCol->rightX <= pOther->leftX && pCol->rightX + deltaPos.x > pOther->leftX)
                    nextPos.x = pOther->leftX - (pCol->end.x * pCol->pTransform->dims.x);
            }

            // this bottom and other top
            if (pCol->borderEnabled[2] && pOther->borderEnabled[3])
            {
                if (pCol->bottomY >= pOther->topY && pCol->bottomY + deltaPos.y < pOther->topY)
                    nextPos.y = pOther->topY - (pCol->start.y * pCol->pTransform->dims.y);
            }

            // this top and other bottom
            if (pCol->borderEnabled[3] && pOther->borderEnabled[2])
            {
                if (pCol->topY <= pOther->bottomY && pCol->topY + deltaPos.y > pOther->bottomY)
                    nextPos.y = pOther->bottomY - (pCol->end.y * pCol->pTransform->dims.y);
            }
        }

        pCol->pTransform->pos = nextPos;
    }
}

bool rendererComparator(shared_ptr<RendererComponent> a, shared_ptr<RendererComponent> b)
{
    return a->pTransform->pos.y > b->pTransform->pos.y;
}