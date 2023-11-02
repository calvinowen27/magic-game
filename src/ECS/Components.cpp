#include "../../include/game/ECS/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/Object.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Animation/AnimationManager.hpp"
#include "../../include/game/Animation/Animation.hpp"
#include "../../include/game/UI/UIManager.hpp"

#include <algorithm>

/* COMPONENT */
Component::Component() : game(*Game::getInstance()), registry(*game.pRegistry), objectManager(*game.pObjectManager)
{
}

bool Component::init()
{
    enable();
    return true;
}

void Component::kill()
{
    disable();
}

void Component::setEntity(Entity *pEntity)
{
    this->pEntity = pEntity;
}

/* RENDERER COMPONENT */
RendererComponent::RendererComponent() : Component(), contentManager(*game.pContentManager)
{
}

bool RendererComponent::init(std::string textureName, std::shared_ptr<TransformComponent> pTransform, int renderOrder)
{
    Component::init();

    pTexture = contentManager.getTexture(textureName);
    this->pTransform = pTransform;
    this->renderOrder = renderOrder;
    sourceRect = SDL_Rect{0, 0, 16, 16};
    spriteDims = Vector2Int(16, 16);

    refreshDimensions();

    return true;
}

bool RendererComponent::init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, int renderOrder, bool startEnabled)
{
    if (startEnabled)
        Component::init();

    pCollider = nullptr;

    pTexture = contentManager.getTextureFromType(entityType);
    this->pTransform = pTransform;
    this->renderOrder = renderOrder;

    json jSpriteDims = objectManager.getEntityData(entityType)["spriteDims"];
    spriteDims = Vector2Int((int)jSpriteDims[0], (int)jSpriteDims[1]);

    sourceRect = SDL_Rect{0, 0, spriteDims.x, spriteDims.y};

    refreshDimensions();

    return true;
}

void RendererComponent::update(float time)
{
    enabled = game.isTransformOnScreen(*pTransform);
}

void RendererComponent::draw(SDL_Renderer *pRenderer)
{
    if (!pTransform)
        return;

    pTransform->updatePxDims();
    pTransform->updatePxRoot();
    pTransform->updatePxPos();
    spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    SDL_RenderCopyEx(pRenderer, pTexture, &sourceRect, &spriteRect, spriteAngle, NULL, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    if (pCollider && game.pUIManager->getDebugUI()->isEnabled())
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

        // draw box collider
        if (pCollider->colliderType == ColliderType::Box)
        {
            Vector2Int colStart = game.worldToPixel(Vector2(pCollider->bottomLeft.x, pCollider->bottomLeft.y)) - pTransform->pxRoot;
            Vector2Int colDims = game.worldToPixel(Vector2(pCollider->topRight.x, pCollider->topRight.y)) - colStart - pTransform->pxRoot;

            auto colliderRect = SDL_Rect{colStart.x, colStart.y, colDims.x, colDims.y};

            SDL_RenderDrawRect(pRenderer, &colliderRect);
        }

        // draw circle collider
        if (pCollider->colliderType == ColliderType::Circle)
        {
            Vector2Int center = game.worldToPixel(pTransform->pos - pTransform->root + (pCollider->start * pTransform->dims));
            int radius = pCollider->end.x * game.ppm;

            int circumference = 6.28 * radius;

            SDL_Point points[2 * circumference];

            int x, y, val;
            int i = 0;
            for (x = -radius; x <= radius; x++)
            {
                for (y = -radius; y <= radius; y++)
                {
                    val = sqrt((x * x) + (y * y));
                    if (val == radius && i < 2 * circumference)
                    {
                        points[i] = SDL_Point{center.x + x, center.y + y};
                        i++;
                    }
                }
            }

            SDL_RenderDrawPoints(pRenderer, points, 2 * circumference);
        }
    }
}

void RendererComponent::kill()
{
    pTexture = nullptr;
    pTransform = nullptr;
    spriteAngle = 0;
    isFlipped = false;
    renderOrder = 0;

    Component::kill();
}

bool RendererComponent::setTexture(std::string textureName)
{
    pTexture = contentManager.getTexture(textureName);

    return true;
}

void RendererComponent::refreshDimensions()
{
    // change dimensions of
    Vector2Int newPxDims = spriteDims * (float)game.ppm / game.SPRITE_PPM;
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

    this->root = Vector2(0.5, 0);

    updatePxPos();

    return true;
}

bool TransformComponent::init(EntityType entityType, Vector2 pos, Vector2 dims)
{
    Component::init();

    this->pos = pos;

    this->dims = dims;
    pxDims = (Vector2Int)(dims * game.ppm);

    json jRelRoot = objectManager.getEntityData(entityType)["relativeRoot"];
    Vector2 relativeRoot = Vector2((float)jRelRoot[0], (float)jRelRoot[1]);

    this->root = relativeRoot * dims;
    pxRoot = (Vector2Int)(root * game.ppm);

    updatePxPos();

    return true;
}

void TransformComponent::setPxDims(Vector2Int newPxDims)
{
    pxDims = newPxDims;
    dims = (Vector2)pxDims / game.ppm;
}

void TransformComponent::updatePxPos()
{
    pxPos = game.worldToPixel(pos - root) - Vector2Int(0, pxDims.y);
}

void TransformComponent::updatePxDims()
{
    pxDims = (Vector2Int)(dims * game.ppm);
}

void TransformComponent::updatePxRoot()
{
    pxRoot = (Vector2Int)(root * game.ppm);
}

/* COLLIDER COMPONENT */
std::map<std::string, ColliderType> ColliderComponent::_typeFromString = {
    {"Box", ColliderType::Box},
    {"Circle", ColliderType::Circle}};

ColliderComponent::ColliderComponent() : Component()
{
}

bool ColliderComponent::init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions, bool isTrigger)
{
    Component::init();

    // retrieve collider data json
    json jCollider = objectManager.getEntityData(entityType)["collider"];

    // set collider endpoints
    json jColliderStart = jCollider["start"];
    start = Vector2((float)jColliderStart[0], (float)jColliderStart[1]) * Vector2(1, 2); // multiply y by 2 to account for perspective

    json jColliderEnd = jCollider["end"];
    end = Vector2((float)jColliderEnd[0], (float)jColliderEnd[1]) * Vector2(1, 2); // multiply y by 2 to account for perspective

    // set collider type
    colliderType = _typeFromString[jCollider["type"]];

    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;
    this->doCollisions = doCollisions;
    this->isTrigger = isTrigger;

    borderEnabled[0] = 1;
    borderEnabled[1] = 1;
    borderEnabled[2] = 1;
    borderEnabled[3] = 1;

    return true;
}

void ColliderComponent::kill()
{
    pEntity = nullptr;
    borderEnabled[0] = 1;
    borderEnabled[1] = 1;
    borderEnabled[2] = 1;
    borderEnabled[3] = 1;
    collidersTouching.clear();

    Component::kill();
}

void ColliderComponent::update(float time)
{
    if (!enabled)
        return;

    for (auto other : collidersTouching)
        whileTouching(other);

    if (colliderType == ColliderType::Box)
    {
        bottomLeft = Vector2(pTransform->pos.x + (start.x * pTransform->dims.x), pTransform->pos.y + (start.y * pTransform->dims.y));
        topRight = Vector2(pTransform->pos.x + (end.x * pTransform->dims.x), pTransform->pos.y + (end.y * pTransform->dims.y));
        bottomRight = Vector2(topRight.x, bottomLeft.y);
        topLeft = Vector2(bottomLeft.x, topRight.y);
    }

    if (colliderType == ColliderType::Circle)
    {
        center = pTransform->pos + (start * pTransform->dims);
        bottomLeft = Vector2(center.x - (end.x * pTransform->dims.x), center.y - (end.x * pTransform->dims.y));
        topRight = Vector2(center.x + (end.x * pTransform->dims.x), center.y + (end.x * pTransform->dims.y));
        bottomRight = Vector2(topRight.x, bottomLeft.y);
        topLeft = Vector2(bottomLeft.x, topRight.y);
    }
}

void ColliderComponent::onCollisionEnter(std::shared_ptr<ColliderComponent> other)
{
    if (pEntity && other->pEntity)
        pEntity->onCollisionEnter(other->pEntity);

    if (enabled && other->enabled)
        collidersTouching.emplace(other);
}

void ColliderComponent::onCollisionExit(std::shared_ptr<ColliderComponent> other)
{
    collidersTouching.erase(other);

    if (pEntity && other->pEntity)
        pEntity->onCollisionExit(other->pEntity);
}

void ColliderComponent::whileTouching(std::shared_ptr<ColliderComponent> other)
{
    if (pEntity && other->pEntity)
        pEntity->whileTouching(other->pEntity);
}

bool ColliderComponent::isTouching(std::shared_ptr<ColliderComponent> other)
{
    return collidersTouching.find(other) != collidersTouching.end();
}

/* RIGIDBODY COMPONENT */
RigidbodyComponent::RigidbodyComponent() : Component()
{
}

bool RigidbodyComponent::init(std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<ColliderComponent> pCollider, bool isStatic)
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

void RigidbodyComponent::kill()
{
    velocity = Vector2::zero;

    Component::kill();
}

/* HEALTH COMPONENT */
HealthComponent::HealthComponent() : Component()
{
}

bool HealthComponent::init(float baseHealth)
{
    Component::init();

    this->baseHealth = baseHealth;
    health = baseHealth;

    auto pRedTransform = registry.newComponent<TransformComponent>();
    pRedTransform->init(Vector2::zero);

    auto pGreenTransform = registry.newComponent<TransformComponent>();
    pGreenTransform->init(Vector2(1, 1));

    pRedRenderer = registry.newComponent<RendererComponent>();
    pRedRenderer->init("red_bar", pRedTransform, -1);

    pGreenRenderer = registry.newComponent<RendererComponent>();
    pGreenRenderer->init("green_bar", pGreenTransform, -2);

    return true;
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
        if (pEntity)
            pEntity->kill();

        return true;
    }

    pGreenRenderer->pTransform->dims.x = health / baseHealth;

    return false;
}

void HealthComponent::kill()
{
    registry.killComponent(pGreenRenderer);
    registry.killComponent(pRedRenderer);

    Component::kill();
}

/* ANIMATION COMPONENT */
AnimatorComponent::AnimatorComponent() : Component(), _animationManager(*game.pAnimationManager)
{
}

bool AnimatorComponent::init(std::shared_ptr<RendererComponent> pRenderer)
{
    _pRenderer = pRenderer;
    _playing = false;

    Component::init();

    return true;
}

void AnimatorComponent::update(float time)
{
    if (_playing)
    {
        if (_timeSinceFrame >= _frameTime)
        {
            _currFrame++;
            if (_currFrame >= _frameCount)
            {
                _currFrame = 0;

                if (!_loops)
                    _playing = false;
            }

            _timeSinceFrame = 0;

            _pRenderer->sourceRect = _rects[_currFrame];
        }
        else
        {
            _timeSinceFrame += time;
        }
    }
}

void AnimatorComponent::setAnimation(EntityType entityType, std::string name)
{
    if (_animationName == name && _animationType == entityType)
        return;

    _pAnimation = _animationManager.getAnimation(entityType, name);
    if (_pAnimation == nullptr)
    {
        std::cout << "returning null with animation name " << name << " on EntityType " << Entity::getStringFromType(entityType) << "\n";
        return;
    }

    _animationName = name;
    _animationType = entityType;
    _currFrame = 0;
    _frameCount = _pAnimation->getFrameCount();
    _frameTime = _pAnimation->getFrameTime();
    _duration = _pAnimation->getDuration();
    _loops = _pAnimation->loops();
    _playing = true;
    _timeSinceFrame = 0;
    _rects = _pAnimation->getRects();
    _pRenderer->pTexture = _pAnimation->getTexture();
}

void AnimatorComponent::kill()
{
    Component::kill();
}