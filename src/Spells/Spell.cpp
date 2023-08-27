#include "../include/game/Spells/Spell.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/ObjectManager.hpp"

#include <type_traits>

Spell::Spell() : Entity()
{
}

bool Spell::init()
{
    Entity::init(EntityType::Spell, Vector2::zero);

    aliveTime = 0;

    pCollider = registry.newComponent<ColliderComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();

    pCollider->init(Vector2::zero, Vector2(1, 1), pTransform, pRigidbody, true, true);
    pCollider->setEntity(this);
    pCollider->disable();

    pRigidbody->init(pTransform, pCollider);
    pRenderer->disable();

    return true;
}

void Spell::update(float time)
{
    if(!isCast || !alive)
        return;
    
    if(aliveTime < lifeDur)
    {
        aliveTime += time;
        pRigidbody->velocity = dir * speed;
    }
    else
    {
        kill();
    }
}

void Spell::hit(Entity *pEntity)
{
    if(Enemy *enemy = dynamic_cast<Enemy *>(pEntity))
    {
        enemy->pHealth->damage(damage);
    }
    else
    {
        std::cerr << "Spell::onCollisionEnter() : failed to cast entity as enemy, incorrect type?\n";
    }
}

void Spell::kill()
{
    Entity::kill();

    isCast = false;
    
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
}

void Spell::cast(Vector2 pos)
{
    isCast = true;
    pCollider->enable();
    pRenderer->enable();
    pTransform->pos = pos;
}

void Spell::onCollisionEnter(Entity *pEntity)
{
    Entity::onCollisionEnter(pEntity);

    if(alive && isCast && pEntity && pEntity->getType() == EntityType::Enemy)
    {
        hit(pEntity);
    }

    kill();
}