#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/ECS/ColliderHandler.hpp"
#include "../include/game/ECS/Registry.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions)
{
    pGame = Game::getInstance();
    pObjectManager = ObjectManager::getInstance();
    pContentManager = pGame->pContentManager;
    pRegistry = pGame->pRegistry;

    type = objType;

    _pTexture = pContentManager->getTextureFromType(type);

    pTransform = static_cast<TransformComponent *>(pRegistry->newComponent(ComponentType::Transform));
    pTransform->init(pos, dims);

    pRigidbody = static_cast<RigidbodyComponent *>(pRegistry->newComponent(ComponentType::Rigidbody));
    pRigidbody->init(pTransform, pCollider);

    std::vector<Vector2> colliderEndpoints = pGame->pColliderHandler->getEndsOfType(type);
    pCollider = static_cast<ColliderComponent *>(pRegistry->newComponent(ComponentType::Collider));
    pCollider->init(colliderEndpoints[0], colliderEndpoints[1], pTransform, pRigidbody);

    pCollider->doCollisions = doCollisions;

    _spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    pObjectManager->addObject(this);
}

Object::~Object()
{
    SDL_DestroyTexture(_pTexture);
}

void Object::draw(SDL_Renderer *pRenderer)
{
    if (!pGame->objOnScreen(*this))
        return;

    _spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    SDL_Point axis{0, _spriteRect.h};
    if (_flipped)
        axis.x = _spriteRect.w;

    SDL_RenderCopyEx(pRenderer, _pTexture, NULL, &_spriteRect, _textureAngle, &axis, pRigidbody->velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Object::update(float time)
{
}