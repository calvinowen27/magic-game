#include "../include/game/Object.hpp"
#include "../include/game/ObjectManager.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object(std::string objType) : Entity(objType)
{
    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
}

bool Object::init(Vector2 pos)
{
    Entity::init(pos);

    pRigidbody->init(pTransform, pCollider);

    std::vector<Vector2> colliderEndpoints = objectManager.getCollider(type);
    pCollider->init(colliderEndpoints[0], colliderEndpoints[1], pTransform, pRigidbody, this);

    return true;
}

void Object::update(float time)
{
    pRenderer->isFlipped = pRigidbody->velocity.x < 0;
}