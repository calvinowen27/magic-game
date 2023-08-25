#include "../include/game/Object.hpp"
#include "../include/game/ObjectManager.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object() : Entity()
{
}

bool Object::init(EntityType entityType, Vector2 pos)
{
    Entity::init(entityType, pos);

    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();

    pRigidbody->init(pTransform, pCollider);

    std::vector<Vector2> colliderEndpoints = objectManager.getCollider(entityType);
    pCollider->init(colliderEndpoints[0], colliderEndpoints[1], pTransform, pRigidbody);
    pCollider->setEntity(this);

    return true;
}

void Object::update(float time)
{
    pRenderer->isFlipped = pRigidbody->velocity.x < 0;

    Entity::update(time);
}

void Object::kill()
{
    Entity::kill();

    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
}