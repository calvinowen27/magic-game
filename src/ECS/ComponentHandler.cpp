#include "../../include/game/ECS/ComponentHandler.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/WorldManager.hpp"
#include "../../include/game/ECS/Components.hpp"

#include <algorithm>

ComponentHandler::ComponentHandler() : _game(*Game::getInstance()), _registry(*_game.pRegistry)
{
}

ComponentHandler::~ComponentHandler()
{
}

void ComponentHandler::draw(SDL_Renderer *pRenderer)
{
    auto rendererSet = _registry.getComponents<RendererComponent>();
    auto renderers = std::vector<shared_ptr<RendererComponent>>(rendererSet.begin(), rendererSet.end());
    std::sort(renderers.begin(), renderers.end(), rendererComparator);

    for (auto renderer : renderers)
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
        if(!pCol->isEnabled())
            continue;

        pCol->update(time);

        Vector2 deltaPos = pCol->pRigidbody->velocity * time;
        Vector2 nextPos = pCol->pTransform->pos + deltaPos;

        for (auto pOther : colliders)
        {
            if(!pOther->isEnabled())
                continue;

            if (pCol == pOther)
                continue;

            if (!pCol->doCollisions || !pOther->doCollisions)
                continue;

            // skip if not touching
            if (pCol->leftX + deltaPos.x > pOther->rightX || pCol->rightX + deltaPos.x < pOther->leftX ||
                pCol->bottomY + deltaPos.y > pOther->topY || pCol->topY + deltaPos.y < pOther->bottomY ||
                !pCol->isEnabled() || !pOther->isEnabled())
            {
                if (pCol->isTouching(pOther) || pOther->isTouching(pCol))
                {
                    pCol->onCollisionExit(pOther);
                    pOther->onCollisionExit(pCol);
                }
                continue;
            }

            // this left and other right
            if (pCol->borderEnabled[0] && pOther->borderEnabled[1])
            {
                if (pCol->leftX >= pOther->rightX && pCol->leftX + deltaPos.x < pOther->rightX)
                {
                    if (!(pCol->isTrigger || pOther->isTrigger))
                        nextPos.x = pOther->rightX - (pCol->start.x * pCol->pTransform->dims.x);

                    pCol->onCollisionEnter(pOther);
                    pOther->onCollisionEnter(pCol);
                }
            }

            // this right and other left
            if (pCol->borderEnabled[1] && pOther->borderEnabled[0])
            {
                if (pCol->rightX <= pOther->leftX && pCol->rightX + deltaPos.x > pOther->leftX)
                {
                    if (!(pCol->isTrigger || pOther->isTrigger))
                        nextPos.x = pOther->leftX - (pCol->end.x * pCol->pTransform->dims.x);

                    pCol->onCollisionEnter(pOther);
                    pOther->onCollisionEnter(pCol);
                }
            }

            // this bottom and other top
            if (pCol->borderEnabled[2] && pOther->borderEnabled[3])
            {
                if (pCol->bottomY >= pOther->topY && pCol->bottomY + deltaPos.y < pOther->topY)
                {
                    if (!(pCol->isTrigger || pOther->isTrigger))
                        nextPos.y = pOther->topY - (pCol->start.y * pCol->pTransform->dims.y);

                    pCol->onCollisionEnter(pOther);
                    pOther->onCollisionEnter(pCol);
                }
            }
            
            // this top and other bottom
            if (pCol->borderEnabled[3] && pOther->borderEnabled[2])
            {
                if (pCol->topY <= pOther->bottomY && pCol->topY + deltaPos.y > pOther->bottomY)
                {
                    if (!(pCol->isTrigger || pOther->isTrigger))
                        nextPos.y = pOther->bottomY - (pCol->end.y * pCol->pTransform->dims.y);

                    pCol->onCollisionEnter(pOther);
                    pOther->onCollisionEnter(pCol);
                }
            }
        }

        pCol->pTransform->pos = nextPos;
    }
}

bool rendererComparator(shared_ptr<RendererComponent> a, shared_ptr<RendererComponent> b)
{
    if (a->renderOrder != b->renderOrder)
        return a->renderOrder > b->renderOrder;

    return a->pTransform->pos.y > b->pTransform->pos.y;
}