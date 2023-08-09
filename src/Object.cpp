#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/ECS/Registry.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object() : game(*Game::getInstance()), objectManager(*game.pObjectManager), contentManager(*game.pContentManager), registry(*game.pRegistry)
{
    pRenderer = registry.newComponent<RendererComponent>();
    pTransform = registry.newComponent<TransformComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
}

void Object::init(string objType, Vector2 pos, Vector2 dims, bool doCollisions)
{
    type = objType;
    
    pRenderer->init(objType, pTransform);
    pTransform->init(pos, dims);
    pRigidbody->init(pTransform, pCollider);

    // std::vector<Vector2> colliderEndpoints = game.pColliderHandler->getEndsOfType(type);
    pCollider->init(Vector2::zero, Vector2(1, 1), pTransform, pRigidbody);
    pCollider->doCollisions = doCollisions;
}

void Object::update(float time)
{
    pRenderer->isFlipped = pRigidbody->velocity.x < 0;
}