#include "../../include/game/ECS/ColliderHandler.hpp"
#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/Game.hpp"

ColliderHandler::ColliderHandler(Registry *pRegistry)
{
    _pRegistry = pRegistry;

    _colliderEndpoints = {
        {"wall", std::vector<Vector2>{ Vector2::zero, Vector2(1, 0.35f) }},
        {"player", std::vector<Vector2>{ Vector2(0.25f, 0), Vector2(0.75f, 0.25f) }},
        {"grass", std::vector<Vector2>{ Vector2::zero, Vector2(1, 1) }}
    };
}

void ColliderHandler::update(float time)
{
    std::vector<ColliderComponent *> colliders = _pRegistry->getColliders();
    for(auto pCol : colliders)
    {
        pCol->update(time);

        Vector2 deltaPos = pCol->pRigidbody->velocity * time;
        Vector2 nextPos = pCol->pTransform->pos + deltaPos;

        for(auto pOther : colliders)
        {
            if(pCol == pOther)
                continue;

            if(!pCol->doCollisions || !pOther->doCollisions)
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

std::vector<Vector2> ColliderHandler::getEndsOfType(std::string type)
{
    return _colliderEndpoints[type];
}