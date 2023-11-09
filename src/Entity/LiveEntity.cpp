#include "../../include/game/Entity/LiveEntity.hpp"

LiveEntity::LiveEntity() : Entity()
{
}

bool LiveEntity::init(EntityType entityType, Vector2 pos)
{
    Entity::init(entityType, pos);

    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
    pHitbox = registry.newComponent<HitboxComponent>();

    pRenderer->setCollider(pCollider);

    pRigidbody->init(pTransform, pCollider);

    pCollider->init(entityType, pTransform, pRigidbody);
    pCollider->setEntity(this);

    pHitbox->init(entityType, pTransform, pRigidbody);
    pHitbox->setEntity(this);

    pRenderer->setHitbox(pHitbox);

    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
    pHealth->setEntity(this);
    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y + 0.5);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    return true;
}

void LiveEntity::update(float time)
{
    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y + 0.5);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Entity::update(time);
}

void LiveEntity::kill()
{
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
    registry.killComponent(pHitbox);

    Entity::kill();
}