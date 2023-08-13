#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"

/* COMPONENT */
Component::Component() : game(*Game::getInstance())
{
}

bool Component::init()
{
    enable();
    return true;
}

/* RENDERER COMPONENT */
RendererComponent::RendererComponent() : Component(), contentManager(*game.pContentManager)
{
}

RendererComponent::~RendererComponent()
{
    SDL_DestroyTexture(pTexture);
}

bool RendererComponent::init(string objType, shared_ptr<TransformComponent> pTransform)
{
    Component::init();
    pTexture = contentManager.getTextureFromType(objType);
    this->pTransform = pTransform;

    return true;
}

void RendererComponent::update(float time)
{
    enabled = game.isTransformOnScreen(*pTransform);
}

void RendererComponent::draw(SDL_Renderer *pRenderer)
{
    if (!enabled)
        return;

    spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    SDL_RenderCopyEx(pRenderer, pTexture, NULL, &spriteRect, spriteAngle, NULL, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

bool RendererComponent::setTexture(std::string textureName)
{
    pTexture = contentManager.getTexture(textureName);

    return true;
}

void RendererComponent::refreshDimensions()
{
    // change dimensions of 
    Vector2Int newPxDims;
    SDL_QueryTexture(pTexture, NULL, NULL, &newPxDims.x, &newPxDims.y);
    newPxDims = newPxDims * (float)(game.ppm / game.TEXTURE_PPM);
    pTransform->setPxDims(newPxDims);
}

/* TRANSFORM COMPONENT */
TransformComponent::TransformComponent() : Component()
{
}

bool TransformComponent::init(Vector2 pos, Vector2 dims)
{
    Component::init();
    this->pos = pos;
    this->dims = dims;
    pxDims = (Vector2Int)(dims * game.ppm);
    pxPos = game.worldToPixel(pos) - Vector2Int(0, pxDims.y);

    return true;
}

void TransformComponent::update(float time)
{
    if(!enabled)
        return;
    
    pxDims = (Vector2Int)(dims * game.ppm);
    pxPos = game.worldToPixel(pos) - Vector2Int(0, pxDims.y);
}

void TransformComponent::setDims(Vector2 newDims)
{
    dims = newDims;
    pxDims = (Vector2Int)(dims * game.ppm);
}

void TransformComponent::setPxDims(Vector2Int newPxDims)
{
    pxDims = newPxDims;
    dims = (Vector2)pxDims / game.ppm;
}

/* COLLIDER COMPONENT */
ColliderComponent::ColliderComponent()
{
}

bool ColliderComponent::init(Vector2 start, Vector2 end, shared_ptr<TransformComponent> pTransform, shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions)
{
    Component::init();
    this->start = start;
    this->end = end;
    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;

    this->doCollisions = doCollisions;

    return true;
}

void ColliderComponent::update(float time)
{
    if(!enabled)
        return;
    
    leftX = pTransform->pos.x + (start.x * pTransform->dims.x);
    rightX = pTransform->pos.x + (end.x * pTransform->dims.x);
    bottomY = pTransform->pos.y + (start.y * pTransform->dims.y);
    topY = pTransform->pos.y + (end.y * pTransform->dims.y);
}

/* RIGIDBODY COMPONENT */
RigidbodyComponent::RigidbodyComponent()
{
}

bool RigidbodyComponent::init(shared_ptr<TransformComponent> pTransform, shared_ptr<ColliderComponent> pCollider, bool isStatic)
{
    Component::init();
    this->pTransform = pTransform;
    this->pCollider = pCollider;

    this->isStatic = isStatic;

    return true;
}

void RigidbodyComponent::update(float time)
{
    if (isStatic || !enabled)
        return;

    if (SDL_fabsf(velocity.x) < 0.0625)
        velocity.x = 0;
    if (SDL_fabsf(velocity.y) < 0.0625)
        velocity.y = 0;

    pTransform->pos += velocity * time;
}