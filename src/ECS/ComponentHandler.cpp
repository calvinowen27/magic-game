#include "../../include/game/ECS/ComponentHandler.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/Game.hpp"
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
    auto renderers = std::vector<std::shared_ptr<RendererComponent>>(rendererSet.begin(), rendererSet.end());
    std::sort(renderers.begin(), renderers.end(), rendererComparator);

    for (auto renderer : renderers)
    {
        if (renderer && renderer->isEnabled())
            renderer->draw(pRenderer);
    }
}

void ComponentHandler::update(float time)
{
    updateColliders(time);
    updateAnimators(time);
}

void ComponentHandler::updateColliders(float time)
{
    auto colliders = _registry.getComponents<ColliderComponent>();
    for (auto pCol : colliders)
    {
        if (!pCol->isEnabled())
            continue;

        pCol->update(time);

        Vector2 deltaPos = pCol->pRigidbody->velocity * time;
        Vector2 nextPos = pCol->pTransform->pos + deltaPos;

        for (auto pOther : colliders)
        {
            if (!pOther->isEnabled() || !pCol->isEnabled())
                continue;

            if (pCol == pOther)
                continue;

            if (!pCol->doCollisions || !pOther->doCollisions)
                continue;

            // Box v. Box
            if (pCol->colliderType == ColliderType::Box && pOther->colliderType == ColliderType::Box)
            {
                // skip if not touching
                if (pCol->bottomLeft.x + deltaPos.x > pOther->topRight.x || pCol->topRight.x + deltaPos.x < pOther->bottomLeft.x ||
                    pCol->bottomLeft.y + deltaPos.y > pOther->topRight.y || pCol->topRight.y + deltaPos.y < pOther->bottomLeft.y)
                {
                    if (pCol->isTouching(pOther) || pOther->isTouching(pCol))
                    {
                        pCol->onCollisionExit(pOther);
                        pOther->onCollisionExit(pCol);
                    }
                    continue;
                }
                else
                {
                    nextPos = handleCollisionBoxBox(pCol, pOther, deltaPos);
                }
            }

            // Circle v. Box
            if (pCol->colliderType == ColliderType::Circle && pOther->colliderType == ColliderType::Box)
            {
                // skip if not touching
                if (pCol->bottomLeft.x + deltaPos.x > pOther->topRight.x || pCol->topRight.x + deltaPos.x < pOther->bottomLeft.x ||
                    pCol->bottomLeft.y + deltaPos.y > pOther->topRight.y || pCol->topRight.y + deltaPos.y < pOther->bottomLeft.y)
                {
                    if (pCol->isTouching(pOther) || pOther->isTouching(pCol))
                    {
                        pCol->onCollisionExit(pOther);
                        pOther->onCollisionExit(pCol);
                    }
                    continue;
                }
                else
                {
                    nextPos = handleCollisionCircleBox(pCol, pOther, deltaPos);
                }
            }

            // Circle v. Circle
            if (pCol->colliderType == ColliderType::Circle && pOther->colliderType == ColliderType::Circle)
            {
                // skip if not touching
                if (Vector2::distance(pCol->center, pOther->center) > pCol->end.x + pOther->end.x)
                {
                    if (pCol->isTouching(pOther) || pOther->isTouching(pCol))
                    {
                        pCol->onCollisionExit(pOther);
                        pOther->onCollisionExit(pCol);
                    }
                    continue;
                }
                else
                {
                    nextPos = handleCollisionCircleCircle(pCol, pOther, deltaPos);
                }
            }
        }

        pCol->pTransform->pos = nextPos;
    }
}

Vector2 ComponentHandler::handleCollisionBoxBox(std::shared_ptr<ColliderComponent> box1, std::shared_ptr<ColliderComponent> box2, Vector2 dPos)
{
    Vector2 nextPos = box1->pTransform->pos + dPos;

    // this left and other right
    if (box1->borderEnabled[0] && box2->borderEnabled[1])
    {
        if (box1->bottomLeft.x >= box2->topRight.x && box1->bottomLeft.x + dPos.x < box2->topRight.x)
        {
            if (!(box1->isTrigger || box2->isTrigger))
                nextPos.x = box2->topRight.x - (box1->start.x * box1->pTransform->dims.x);

            box1->onCollisionEnter(box2);
            box2->onCollisionEnter(box1);
        }
    }

    // this right and other left
    if (box1->borderEnabled[1] && box2->borderEnabled[0])
    {
        if (box1->topRight.x <= box2->bottomLeft.x && box1->topRight.x + dPos.x > box2->bottomLeft.x)
        {
            if (!(box1->isTrigger || box2->isTrigger))
                nextPos.x = box2->bottomLeft.x - (box1->end.x * box1->pTransform->dims.x);

            box1->onCollisionEnter(box2);
            box2->onCollisionEnter(box1);
        }
    }

    // this bottom and other top
    if (box1->borderEnabled[2] && box2->borderEnabled[3])
    {
        if (box1->bottomLeft.y >= box2->topRight.y && box1->bottomLeft.y + dPos.y < box2->topRight.y)
        {
            if (!(box1->isTrigger || box2->isTrigger))
                nextPos.y = box2->topRight.y - (box1->start.y * box1->pTransform->dims.y);

            box1->onCollisionEnter(box2);
            box2->onCollisionEnter(box1);
        }
    }

    // this top and other bottom
    if (box1->borderEnabled[3] && box2->borderEnabled[2])
    {
        if (box1->topRight.y <= box2->bottomLeft.y && box1->topRight.y + dPos.y > box2->bottomLeft.y)
        {
            if (!(box1->isTrigger || box2->isTrigger))
                nextPos.y = box2->bottomLeft.y - (box1->end.y * box1->pTransform->dims.y);

            box1->onCollisionEnter(box2);
            box2->onCollisionEnter(box1);
        }
    }

    return nextPos;
}

Vector2 ComponentHandler::handleCollisionCircleBox(std::shared_ptr<ColliderComponent> circle, std::shared_ptr<ColliderComponent> box, Vector2 dPos)
{
    Vector2 nextPos = circle->pTransform->pos + dPos;

    // (Vector2::distance(pCol->center + deltaPos, pOther->bottomLeft) > pCol->end.x * pCol->pTransform->dims.x &&
    // Vector2::distance(pCol->center + deltaPos, pOther->bottomRight) > pCol->end.x * pCol->pTransform->dims.x &&
    // Vector2::distance(pCol->center + deltaPos, pOther->topLeft) > pCol->end.x * pCol->pTransform->dims.x &&
    // Vector2::distance(pCol->center + deltaPos, pOther->topRight) > pCol->end.x * pCol->pTransform->dims.x)

    if (box->borderEnabled[0] || box->borderEnabled[2])
    {
        if (Vector2::distance(circle->center + dPos, box->bottomLeft) <= circle->end.x)
        {
            std::cout << Vector2::distance(circle->center + dPos, box->bottomLeft) << "\n";
            if (!(circle->isTrigger || box->isTrigger))
                nextPos = box->bottomLeft - circle->end;

            circle->onCollisionEnter(box);
            box->onCollisionEnter(circle);

            return nextPos;
        }
    }

    // circle left and box right
    if (box->borderEnabled[1])
    {
        if (circle->bottomLeft.x >= box->topRight.x && circle->bottomLeft.x + dPos.x < box->topRight.x)
        {
            if (!(circle->isTrigger || box->isTrigger))
                nextPos.x = box->topRight.x - (circle->end.x * circle->pTransform->dims.x);

            circle->onCollisionEnter(box);
            box->onCollisionEnter(circle);
        }
    }

    // circle right and box left
    if (box->borderEnabled[0])
    {
        if (circle->topRight.x <= box->bottomLeft.x && circle->topRight.x + dPos.x > box->bottomLeft.x)
        {
            if (!(circle->isTrigger || box->isTrigger))
                nextPos.x = box->bottomLeft.x - (circle->end.x * circle->pTransform->dims.x * 2);

            circle->onCollisionEnter(box);
            box->onCollisionEnter(circle);
        }
    }

    // circle bottom and box top
    if (box->borderEnabled[3])
    {
        if (circle->bottomLeft.y >= box->topRight.y && circle->bottomLeft.y + dPos.y < box->topRight.y)
        {
            if (!(circle->isTrigger || box->isTrigger))
                nextPos.y = box->topRight.y - (circle->end.x * circle->pTransform->dims.y);

            circle->onCollisionEnter(box);
            box->onCollisionEnter(circle);
        }
    }

    // circle top and box bottom
    if (box->borderEnabled[2])
    {
        if (circle->topRight.y <= box->bottomLeft.y && circle->topRight.y + dPos.y > box->bottomLeft.y)
        {
            if (!(circle->isTrigger || box->isTrigger))
                nextPos.y = box->bottomLeft.y - (circle->end.x * circle->pTransform->dims.y);

            circle->onCollisionEnter(box);
            box->onCollisionEnter(circle);
        }
    }

    return nextPos;
}

Vector2 ComponentHandler::handleCollisionCircleCircle(std::shared_ptr<ColliderComponent> circle1, std::shared_ptr<ColliderComponent> circle2, Vector2 dPos)
{
    Vector2 nextPos = circle1->pTransform->pos + dPos;

    return nextPos;
}

void ComponentHandler::updateAnimators(float time)
{
    auto animators = _registry.getComponents<AnimatorComponent>();
    for (auto animator : animators)
    {
        animator->update(time);
    }
}

bool rendererComparator(std::shared_ptr<RendererComponent> a, std::shared_ptr<RendererComponent> b)
{
    if (!a->isEnabled())
        return false;

    if (!b->isEnabled())
        return false;

    if (a->renderOrder != b->renderOrder)
        return a->renderOrder > b->renderOrder;

    return a->pTransform->pos.y > b->pTransform->pos.y;
}