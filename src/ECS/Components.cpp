#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"

/* COMPONENT */
Component::Component() : game(*Game::getInstance()), registry(*game.pRegistry)
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

RendererComponent *RendererComponent::init(string objType, shared_ptr<TransformComponent> pTransform, int renderOrder)
{
    Component::init();
    pTexture = contentManager.getTextureFromType(objType);
    this->pTransform = pTransform;
    this->renderOrder = renderOrder;

    return this;
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

TransformComponent *TransformComponent::init(Vector2 pos, Vector2 dims)
{
    Component::init();
    this->pos = pos;
    this->dims = dims;
    pxDims = (Vector2Int)(dims * game.ppm);
    pxPos = game.worldToPixel(pos) - Vector2Int(0, pxDims.y);

    return this;
}

void TransformComponent::update(float time)
{
    if (!enabled)
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
ColliderComponent::ColliderComponent() : Component()
{
}

ColliderComponent *ColliderComponent::init(Vector2 start, Vector2 end, shared_ptr<TransformComponent> pTransform, shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions)
{
    Component::init();
    this->start = start;
    this->end = end;
    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;

    this->doCollisions = doCollisions;

    return this;
}

void ColliderComponent::update(float time)
{
    if (!enabled)
        return;

    leftX = pTransform->pos.x + (start.x * pTransform->dims.x);
    rightX = pTransform->pos.x + (end.x * pTransform->dims.x);
    bottomY = pTransform->pos.y + (start.y * pTransform->dims.y);
    topY = pTransform->pos.y + (end.y * pTransform->dims.y);
}

/* RIGIDBODY COMPONENT */
RigidbodyComponent::RigidbodyComponent() : Component()
{
}

RigidbodyComponent *RigidbodyComponent::init(shared_ptr<TransformComponent> pTransform, shared_ptr<ColliderComponent> pCollider, bool isStatic)
{
    Component::init();
    this->pTransform = pTransform;
    this->pCollider = pCollider;

    this->isStatic = isStatic;

    return this;
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

/* HEALTH COMPONENT */
HealthComponent::HealthComponent() : Component()
{
}

HealthComponent *HealthComponent::init(float baseHealth)
{
    this->baseHealth = baseHealth;
    health = baseHealth;

    auto pRedTransform = registry.newComponent<TransformComponent>();
    pRedTransform->init(Vector2::zero);

    auto pGreenTransform = registry.newComponent<TransformComponent>();
    pGreenTransform->init(Vector2::zero);

    pRedRenderer = registry.newComponent<RendererComponent>();
    pRedRenderer->init("red_bar", pRedTransform, -1);

    pGreenRenderer = registry.newComponent<RendererComponent>();
    pGreenRenderer->init("green_bar", pGreenTransform, -2);

    return this;
}

void HealthComponent::heal(float healAmount)
{
    health += healAmount;
    if (health > baseHealth)
        health = baseHealth;

    pGreenRenderer->pTransform->dims.x = health / baseHealth;
}

bool HealthComponent::damage(float dmgAmount)
{
    health -= dmgAmount;
    if (health <= 0)
    {
        health = 0;
        pGreenRenderer->pTransform->dims.x = 0;
        return true;
    }

    pGreenRenderer->pTransform->dims.x = health / baseHealth;

    return false;
}