#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/ECS/Registry.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

using std::vector;

Object::Object(string objType) : game(*Game::getInstance()), objectManager(*game.pObjectManager), contentManager(*game.pContentManager), registry(*game.pRegistry)
{
    type = objType;

    pRenderer = registry.newComponent<RendererComponent>();
    pTransform = registry.newComponent<TransformComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
}

bool Object::init(Vector2 pos)
{
    pRenderer->init(type, pTransform);
    pTransform->init(pos);
    pRenderer->refreshDimensions();
    pRigidbody->init(pTransform, pCollider);

    vector<Vector2> colliderEndpoints = objectManager.getCollider(type);
    pCollider->init(colliderEndpoints[0], colliderEndpoints[1], pTransform, pRigidbody);

    return true;
}

void Object::update(float time)
{
    pRenderer->isFlipped = pRigidbody->velocity.x < 0;
}