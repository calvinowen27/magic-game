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

    pRenderer->setCollider(pCollider);

    pRigidbody->init(pTransform, pCollider);

    pCollider->init(entityType, pTransform, pRigidbody);
    pCollider->setEntity(this);

    pRenderer->enable();

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