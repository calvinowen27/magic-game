#include "../include/game/Spells/Spell.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/ObjectManager.hpp"

#include <type_traits>

Spell::Spell() : Entity()
{
}

bool Spell::init(EntityType type, Vector2 pos, Vector2 dir, SpellElement element, float damage, float lifeDur)
{
    Entity::init(type, pos);

    this->dir = dir;
    this->element = element;
    this->damage = damage;
    this->lifeDur = lifeDur;
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

void Spell::cast()
{
    isCast = true;
    pCollider->enable();
    pRenderer->enable();
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