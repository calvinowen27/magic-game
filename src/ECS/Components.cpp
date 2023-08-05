#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ECS/Registry.hpp"

/* COMPONENT */
Component::Component()
{
    pGame = Game::getInstance();
    pRegistry = pGame->pRegistry;
}

/* TRANSFORM COMPONENT */
TransformComponent::TransformComponent(Vector2 pos, Vector2 dims) : Component()
{
    this->pos = pos;
    this->dims = dims;
    pxDims = (Vector2Int)(dims * pGame->ppm);
    pxPos = pGame->worldToPixel(pos) - Vector2Int(0, pxDims.y);

    pRegistry->addTransform(this);
}

void TransformComponent::update(float time)
{
    pxDims = (Vector2Int)(dims * pGame->ppm);
    pxPos = pGame->worldToPixel(pos) - Vector2Int(0, pxDims.y);
}

/* COLLIDER COMPONENT */
ColliderComponent::ColliderComponent(Vector2 start, Vector2 end, TransformComponent *pTransform, RigidbodyComponent *pRigidbody, bool doCollisions)
{
    this->start = start;
    this->end = end;
    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;

    this->doCollisions = doCollisions;

    pRegistry->addCollider(this);
}

void ColliderComponent::update(float time)
{
    leftX = pTransform->pos.x + (start.x * pTransform->dims.x);
    rightX = pTransform->pos.x + (end.x * pTransform->dims.x);
    bottomY = pTransform->pos.y + (start.y * pTransform->dims.y);
    topY = pTransform->pos.y + (end.y * pTransform->dims.y);
}

/* RIGIDBODY COMPONENT */
RigidbodyComponent::RigidbodyComponent(TransformComponent *pTransform, ColliderComponent *pCollider, bool isStatic)
{
    this->pTransform = pTransform;
    this->pCollider = pCollider;

    this->isStatic = isStatic;
}

void RigidbodyComponent::update(float time)
{
    if(isStatic)
        return;

    if (SDL_fabsf(velocity.x) < 0.0625)
        velocity.x = 0;
    if (SDL_fabsf(velocity.y) < 0.0625)
        velocity.y = 0;

    pTransform->pos += velocity * time;
}