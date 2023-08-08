#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/ECS/ColliderHandler.hpp"
#include "../include/game/ECS/Registry.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object(std::string objType)
{
    pGame = Game::getInstance();
    pObjectManager = ObjectManager::getInstance();
    pContentManager = pGame->pContentManager;
    pRegistry = pGame->pRegistry;

    pRenderer = static_cast<RendererComponent *>(pRegistry->newComponent(ComponentType::Renderer));
    pTransform = static_cast<TransformComponent *>(pRegistry->newComponent(ComponentType::Transform));
    pRigidbody = static_cast<RigidbodyComponent *>(pRegistry->newComponent(ComponentType::Rigidbody));
    pCollider = static_cast<ColliderComponent *>(pRegistry->newComponent(ComponentType::Collider));

    type = objType;

    pObjectManager->addObject(this);
}

Object::~Object()
{
    delete pRenderer;
    delete pTransform;
    delete pCollider;
    delete pRigidbody;
}

void Object::init(Vector2 pos, Vector2 dims, bool doCollisions)
{
    pRenderer->init(type, pTransform);
    pTransform->init(pos, dims);
    pRigidbody->init(pTransform, pCollider);

    std::vector<Vector2> colliderEndpoints = pGame->pColliderHandler->getEndsOfType(type);
    pCollider->init(colliderEndpoints[0], colliderEndpoints[1], pTransform, pRigidbody);
    pCollider->doCollisions = doCollisions;
}

void Object::update(float time)
{
    pRenderer->isFlipped = pRigidbody->velocity.x < 0;
}