#include "../../include/game/Entity/Components.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Objects/Object.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Animation/AnimationManager.hpp"
#include "../../include/game/Animation/Animation.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Entity/ComponentHandler.hpp"
#include "../../include/game/Level/LevelManager.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Entity/Entity.hpp"

#include <algorithm>

/* COMPONENT */
Component::Component() : game(*Game::getInstance()), registry(*game.pRegistry), objectManager(*game.pObjectManager)
{
}

bool Component::init()
{
    pEntity = nullptr;

    enable();
    return true;
}

void Component::kill()
{
    pEntity = nullptr;

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
    Component::init();

    if (!startEnabled)
        disable();
    
    pCollider = nullptr;
    pHitbox = nullptr;

    pTexture = contentManager.getTextureFromType(entityType);
    this->pTransform = pTransform;
    this->renderOrder = renderOrder;

    json jEntity = objectManager.getEntityData(entityType);
    json jSpriteDims = jEntity["spriteDims"];
    spriteDims = Vector2Int((int)jSpriteDims[0], (int)jSpriteDims[1]);

    if (jEntity.contains("sourceRect"))
        sourceRect = SDL_Rect{(int)jEntity["sourceRect"][0], (int)jEntity["sourceRect"][1], spriteDims.x, spriteDims.y};
    else    
        sourceRect = SDL_Rect{0, 0, spriteDims.x, spriteDims.y};

    refreshDimensions();

    return true;
}

void RendererComponent::update(float time)
{
    enabled = game.isTransformOnScreen(*pTransform) && game.pLevelManager->isTransformInLevel(*pTransform);
}

void RendererComponent::draw(SDL_Renderer *pRenderer)
{
    if (!pTransform)
        return;

    // update pixel values
    pTransform->updatePxDims();
    pTransform->updatePxRoot();
    pTransform->updatePxPos();

    // construct rect
    spriteRect = SDL_Rect{pTransform->pxPos.x, pTransform->pxPos.y, pTransform->pxDims.x, pTransform->pxDims.y};

    // draw sprite
    SDL_RenderCopyEx(pRenderer, pTexture, &sourceRect, &spriteRect, pTransform->rotDeg, NULL, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // SDL_RenderCopy(pRenderer, pTexture, &sourceRect, &spriteRect);


    // skip if not showing colliders
    if (!game.pComponentHandler->isShowingColliders())
        return;

    // draw collider
    if (pCollider)
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

        // draw box collider
        Vector2Int colStart = game.worldToPixel(Vector2(pCollider->bottomLeft.x, pCollider->bottomLeft.y)) - pTransform->pxRoot;
        Vector2Int colDims = game.worldToPixel(Vector2(pCollider->topRight.x, pCollider->topRight.y)) - colStart - pTransform->pxRoot;

        auto colliderRect = SDL_Rect{colStart.x, colStart.y, colDims.x, colDims.y};

        SDL_RenderDrawRect(pRenderer, &colliderRect);
    }

    // draw hitbox
    if (pHitbox)
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 0, 255);

        if (pHitbox->hitboxType == HitboxType::Box) // draw box hitbox
        {
            Vector2Int hbStart = pHitbox->bottomLeft;
            Vector2Int hbDims = pHitbox->topRight - hbStart;

            auto hbRect = SDL_Rect{hbStart.x, hbStart.y, hbDims.x, hbDims.y};

            SDL_RenderDrawRect(pRenderer, &hbRect);
        }
        else if (pHitbox->hitboxType == HitboxType::Circle) // draw circle collider
        {
            Vector2Int center = pHitbox->center;
            int radius = pHitbox->end.x * pTransform->pxDims.x;

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

    if(pEntity && pEntity->getType() == EntityType::Enemy)
    {
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

        Vector2Int playerPxPos = game.worldToPixel(game.pPlayer->getPos());
        Vector2Int pxPos = game.worldToPixel(pEntity->getPos());

        SDL_RenderDrawLine(pRenderer, playerPxPos.x, playerPxPos.y, pxPos.x, pxPos.y);
    }
}

void RendererComponent::kill()
{
    pTexture = nullptr;
    pTransform = nullptr;
    isFlipped = false;
    renderOrder = 0;
    pCollider = nullptr;
    pHitbox = nullptr;

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
TransformComponent::TransformComponent() : Component(), _levelManager(*LevelManager::getInstance())
{
}

bool TransformComponent::init(Vector2 pos, Vector2 dims, bool constrain)
{
    Component::init();

    this->pos = pos;
    this->rotDeg = 0;
    this->rotRad = 0;

    this->dims = dims;
    pxDims = (Vector2Int)(dims * game.ppm);

    this->root = Vector2(0.5, 0);

    this->constrain = constrain;

    updatePxPos();

    return true;
}

bool TransformComponent::init(EntityType entityType, Vector2 pos, Vector2 dims)
{
    Component::init();

    this->pos = pos;
    this->rotDeg = 0;
    this->rotRad = 0;

    json jEntity = objectManager.getEntityData(entityType);

    this->dims = dims;
    pxDims = (Vector2Int)(dims * game.ppm);

    json jRelRoot = jEntity["relativeRoot"];
    Vector2 relativeRoot = Vector2((float)jRelRoot[0], (float)jRelRoot[1]);

    this->root = relativeRoot * dims;
    pxRoot = (Vector2Int)((root * dims) + Vector2(0.9, 0.9));

    updatePxPos();

    return true;
}

void TransformComponent::update(float time)
{
    if(!constrain)
        return;

    Vector2 levelBoundsStart = _levelManager.getLevelBoundsStart();
    Vector2 levelBoundsEnd = _levelManager.getLevelBoundsEnd();

    if (pos.x < levelBoundsStart.x)
    {
        pos.x = levelBoundsStart.x;
    }

    if (pos.x > levelBoundsEnd.x)
    {
        pos.x = levelBoundsEnd.x;
    }

    if (pos.y < levelBoundsStart.y)
    {
        pos.y = levelBoundsStart.y;
    }

    if (pos.y > levelBoundsEnd.y)
    {
        pos.y = levelBoundsEnd.y;
    }
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
    pxDims = (Vector2Int)((dims * game.ppm) + Vector2(0.9, 0.9));
}

void TransformComponent::updatePxRoot()
{
    pxRoot = (Vector2Int)((root * dims) + Vector2(0.9, 0.9));
}

/* COLLIDER COMPONENT */
ColliderComponent::ColliderComponent() : Component()
{
}

bool ColliderComponent::init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions)
{
    Component::init();

    this->entityType = entityType;

    // retrieve collider data json
    json jCollider = objectManager.getEntityData(entityType)["collider"];

    // set collider endpoints
    json jColliderStart = jCollider["start"];
    start = Vector2((float)jColliderStart[0], (float)jColliderStart[1]) * Vector2(1, 2); // multiply y by 2 to account for perspective

    json jColliderEnd = jCollider["end"];
    end = Vector2((float)jColliderEnd[0], (float)jColliderEnd[1]) * Vector2(1, 2); // multiply y by 2 to account for perspective

    // initialize edge positions
    bottomLeft = Vector2(pTransform->pos.x + (start.x * pTransform->dims.x), pTransform->pos.y + (start.y * pTransform->dims.y));
    topRight = Vector2(pTransform->pos.x + (end.x * pTransform->dims.x), pTransform->pos.y + (end.y * pTransform->dims.y));

    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;
    this->doCollisions = doCollisions;
    if(jCollider.contains("isTrigger"))
        isTrigger = (bool)jCollider["isTrigger"];
    else
        isTrigger = false;

    borderEnabled[0] = 1;
    borderEnabled[1] = 1;
    borderEnabled[2] = 1;
    borderEnabled[3] = 1;

    return true;
}

void ColliderComponent::kill()
{
    borderEnabled[0] = 1;
    borderEnabled[1] = 1;
    borderEnabled[2] = 1;
    borderEnabled[3] = 1;
    colsTouching.clear();

    Component::kill();
}

void ColliderComponent::update(float time)
{
    if (!enabled)
        return;

    for (auto other : colsTouching)
        whileTouching(other);

    // update corner positions
    bottomLeft = Vector2(pTransform->pos.x + (start.x * pTransform->dims.x), pTransform->pos.y + (start.y * pTransform->dims.y));
    topRight = Vector2(pTransform->pos.x + (end.x * pTransform->dims.x), pTransform->pos.y + (end.y * pTransform->dims.y));
}

void ColliderComponent::onCollisionEnter(std::shared_ptr<ColliderComponent> pOther)
{
    if (pEntity && pOther->pEntity)
        pEntity->onCollisionEnter(pOther->pEntity);

    if (enabled && pOther->enabled)
        colsTouching.emplace(pOther);
}

void ColliderComponent::onCollisionExit(std::shared_ptr<ColliderComponent> pOther)
{
    colsTouching.erase(pOther);

    if (pEntity && pOther->pEntity)
        pEntity->onCollisionExit(pOther->pEntity);
}

void ColliderComponent::whileTouching(std::shared_ptr<ColliderComponent> pOther)
{
    if (pEntity && pOther->pEntity)
        pEntity->whileTouching(pOther->pEntity);
}

bool ColliderComponent::isTouching(std::shared_ptr<ColliderComponent> pOther)
{
    return colsTouching.find(pOther) != colsTouching.end();
}

/* HITBOX COMPONENT */
std::map<std::string, HitboxType> HitboxComponent::_typeFromString = {
    {"Box", HitboxType::Box},
    {"Circle", HitboxType::Circle}};

HitboxComponent::HitboxComponent() : Component()
{
}

bool HitboxComponent::init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody)
{
    Component::init();

    this->entityType = entityType;

    // retrieve hitbox data json
    json jHitbox = objectManager.getEntityData(entityType);
    if (!jHitbox.contains("hitbox"))
        return false;

    jHitbox = jHitbox["hitbox"];

    // set hitbox type
    if (jHitbox.contains("type"))
        hitboxType = _typeFromString[jHitbox["type"]];

    // set hitbox endpoints
    json jHitboxStart = jHitbox["start"];
    start = Vector2((float)jHitboxStart[0], 1 - (float)jHitboxStart[1]); // 1 - y to convert from bottom left to top left (pixels)

    json jHitboxEnd = jHitbox["end"];
    end = Vector2((float)jHitboxEnd[0], 1 - (float)jHitboxEnd[1]); // 1 - y to convert from bottom left to top left (pixels)

    this->pTransform = pTransform;
    this->pRigidbody = pRigidbody;

    // initialize edge pixel positions
    pTransform->updatePxDims();
    pTransform->updatePxPos();

    if (hitboxType == HitboxType::Box)
    {
        bottomLeft = pTransform->pxPos + (Vector2Int)(start * (Vector2)pTransform->pxDims);
        topRight = pTransform->pxPos + (Vector2Int)(end * (Vector2)pTransform->pxDims);
    }
    else if (hitboxType == HitboxType::Circle)
    {
        center = (Vector2Int)(start * (Vector2)pTransform->pxDims);
        bottomLeft = center - (Vector2Int)(end * (Vector2)pTransform->pxDims);
        topRight = center + (Vector2Int)(end * (Vector2)pTransform->pxDims);
    }

    return true;
}

void HitboxComponent::update(float time)
{
    // update edge pixel positions

    if (hitboxType == HitboxType::Box)
    {
        bottomLeft = pTransform->pxPos + (Vector2Int)(start * (Vector2)pTransform->pxDims);
        topRight = pTransform->pxPos + (Vector2Int)(end * (Vector2)pTransform->pxDims);
    }
    else if (hitboxType == HitboxType::Circle)
    {
        Vector2Int rotationOffset = Vector2Int(sin(pTransform->rotRad) * pTransform->pxDims.x * end.x, (1 - cos(pTransform->rotRad)) * pTransform->pxDims.y * (1 - end.y)); // (1 -) because inverse y for pixels

        center = pTransform->pxPos + (Vector2Int)(start * (Vector2)pTransform->pxDims) + rotationOffset;
        bottomLeft = center - (Vector2Int)(end * (Vector2)pTransform->pxDims);
        topRight = center + (Vector2Int)(end * (Vector2)pTransform->pxDims * Vector2(1, -1));
    }
}

void HitboxComponent::onHitboxEnter(std::shared_ptr<HitboxComponent> pOther)
{
    if (pEntity && pOther->pEntity)
        pEntity->onHitboxEnter(pOther->pEntity);

    if (enabled && pOther->enabled)
        hbTouching.emplace(pOther);
}
void HitboxComponent::onHitboxExit(std::shared_ptr<HitboxComponent> pOther)
{
    hbTouching.erase(pOther);

    if (pEntity && pOther->pEntity)
        pEntity->onHitboxExit(pOther->pEntity);
}

bool HitboxComponent::isTouching(std::shared_ptr<HitboxComponent> pOther)
{
    return hbTouching.find(pOther) != hbTouching.end();
}

void HitboxComponent::kill()
{
    hbTouching.clear();

    Component::kill();
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
    nextPosSet = false;

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

    if (nextPosSet)
    {
        pTransform->pos = nextPos;
    }
    else
    {
        nextPosSet = true;
        pTransform->pos += velocity * time;
    }

    nextPos = pTransform->pos + velocity * time;
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

bool HealthComponent::init(float baseHealth, std::shared_ptr<TransformComponent> pFollowTransform, Vector2 followOffset)
{
    Component::init();

    this->baseHealth = baseHealth;
    health = baseHealth;

    this->pFollowTransform = pFollowTransform;
    this->followOffset = followOffset;

    pRedTransform = registry.newComponent<TransformComponent>();
    pRedTransform->init(pFollowTransform->pos + followOffset, Vector2(1, 1), false);

    pGreenTransform = registry.newComponent<TransformComponent>();
    pGreenTransform->init(pFollowTransform->pos + followOffset, Vector2(1, 1), false);

    pRedRenderer = registry.newComponent<RendererComponent>();
    pRedRenderer->init("red_bar", pRedTransform, -1);

    pGreenRenderer = registry.newComponent<RendererComponent>();
    pGreenRenderer->init("green_bar", pGreenTransform, -2);

    return true;
}

void HealthComponent::update(float time)
{
    setPos(pFollowTransform->pos + followOffset);
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

void HealthComponent::setPos(Vector2 pos)
{
    pGreenTransform->pos = pos;
    pRedTransform->pos = pos;
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