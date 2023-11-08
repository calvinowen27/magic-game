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

    return true;
}

void LiveEntity::kill()
{
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
    registry.killComponent(pHitbox);

    Entity::kill();
}