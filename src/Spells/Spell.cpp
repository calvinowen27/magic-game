#include "../../include/game/Spells/Spell.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Enemy.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Spells/SpellManager.hpp"

#include <type_traits>

Spell::Spell() : Entity(), spellManager(*game.pSpellManager)
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

    clearAttributes();

    return true;
}

void Spell::update(float time)
{
    if (!isCast || !alive)
        return;

    if (aliveTime < lifeDur)
    {
        aliveTime += time;
        pRigidbody->velocity = dir.normalized() * speed;
    }
    else
    {
        kill();
    }
}

void Spell::hit(Entity *pEntity)
{
    if (Enemy *enemy = dynamic_cast<Enemy *>(pEntity))
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
    clearAttributes();

    Entity::kill();

    isCast = false;

    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
}

void Spell::cast(Vector2 pos)
{
    pTransform->pos = pos;
    pRenderer->enable();
    pCollider->enable();
    isCast = true;
}

void Spell::onCollisionEnter(Entity *pEntity)
{
    if (!pEntity)
        return;

    Entity::onCollisionEnter(pEntity);

    if (alive && isCast && pEntity->getType() == EntityType::Enemy)
    {
        hit(pEntity);
    }

    if (pEntity->getType() != EntityType::Player && pEntity->getType() != EntityType::Spell)
    {
        kill();
    }
}

void Spell::setDir(Vector2 dir)
{
    this->dir = dir;

    if (pRenderer)
    {
        pRenderer->spriteAngle = atan2(dir.x, dir.y) * RAD_TO_DEGS;
        
        if(dir.x < 0)
            pRenderer->isFlipped = true;
    }
}