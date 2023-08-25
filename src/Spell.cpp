#include "../include/game/Spell.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/ObjectManager.hpp"

#include <type_traits>

Spell::Spell() : Entity()
{
}

bool Spell::init(Vector2 pos, Vector2 dir, SpellElement element)
{
    Entity::init(EntityType::Spell, pos);

    this->element = element;
    this->dir = dir;
    isCast = true;
    aliveTime = 0;

    pCollider = registry.newComponent<ColliderComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();

    pCollider->init(Vector2::zero, Vector2(1, 1), pTransform, pRigidbody, true, true);
    pCollider->setEntity(this);
    pRigidbody->init(pTransform, pCollider);

    return true;
}

void Spell::update(float time)
{
    if(isCast && alive)
    {
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
}

void Spell::onCollisionEnter(Entity *pOther)
{
    Entity::onCollisionEnter(pOther);

    if(pOther && pOther->getType() == EntityType::Enemy)
    {
        if(Enemy *enemy = dynamic_cast<Enemy *>(pOther))
        {
            enemy->pHealth->damage(damage);
        }
        else
        {
            std::cerr << "Spell::onCollisionEnter() : failed to cast entity as enemy, incorrect type?\n";
        }
    }

    kill();
}