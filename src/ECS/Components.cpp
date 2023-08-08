#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"

/* COMPONENT */
Component::Component()
{
    pGame = Game::getInstance();
}

/* RENDERER COMPONENT */
RendererComponent::RendererComponent() : Component()
{
    pContentManager = pGame->pContentManager;
}

RendererComponent::~RendererComponent()
{
    SDL_DestroyTexture(pTexture);
}

bool RendererComponent::init(std::string objType, TransformComponent *pTransform)
{
    pTexture = pContentManager->getTextureFromType(objType);
    this->pTransform = pTransform;

    return true;
}

void RendererComponent::update(float time)
{
    isEnabled = pGame->isTransformOnScreen(*pTransform);
}

void RendererComponent::draw(SDL_Renderer *pRenderer)
{
    if (!isEnabled)
        return;

    spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    SDL_RenderCopyEx(pRenderer, pTexture, NULL, &spriteRect, spriteAngle, NULL, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

bool RendererComponent::setTexture(std::string textureName)
{
    pTexture = pContentManager->getTexture(textureName);

    return true;
}

/* TRANSFORM COMPONENT */
TransformComponent::TransformComponent() : Component()
{
}

bool TransformComponent::init(Vector2 pos, Vector2 dims)
{
    this->pos = pos;
    this->dims = dims;
    pxDims = (Vector2Int)(dims * pGame->ppm);
    pxPos = pGame->worldToPixel(pos) - Vector2Int(0, pxDims.y);

    return true;
}

void TransformComponent::update(float time)
{
    pxDims = (Vector2Int)(dims * pGame->ppm);
    pxPos = pGame->worldToPixel(pos) - Vector2Int(0, pxDims.y);
}

/* COLLIDER COMPONENT */
ColliderComponent::ColliderComponent()
{
}

bool ColliderComponent::init(Vector2 start, Vector2 end, TransformComponent *pTransform, RigidbodyComponent *pRigidbody, bool doCollisions)
{
    this->start = start;
    this->end = end;
    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;

    this->doCollisions = doCollisions;
}

void ColliderComponent::update(float time)
{
    leftX = pTransform->pos.x + (start.x * pTransform->dims.x);
    rightX = pTransform->pos.x + (end.x * pTransform->dims.x);
    bottomY = pTransform->pos.y + (start.y * pTransform->dims.y);
    topY = pTransform->pos.y + (end.y * pTransform->dims.y);
}

/* RIGIDBODY COMPONENT */
RigidbodyComponent::RigidbodyComponent()
{
}

bool RigidbodyComponent::init(TransformComponent *pTransform, ColliderComponent *pCollider, bool isStatic)
{
    this->pTransform = pTransform;
    this->pCollider = pCollider;

    this->isStatic = isStatic;

    return true;
}

void RigidbodyComponent::update(float time)
{
    if (isStatic)
        return;

    if (SDL_fabsf(velocity.x) < 0.0625)
        velocity.x = 0;
    if (SDL_fabsf(velocity.y) < 0.0625)
        velocity.y = 0;

    pTransform->pos += velocity * time;
}