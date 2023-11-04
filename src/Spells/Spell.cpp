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

    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pHitbox = registry.newComponent<HitboxComponent>();

    pRigidbody->init(pTransform);

    pHitbox->init(EntityType::Spell, pTransform, pRigidbody);
    pHitbox->setEntity(this);

    pRenderer->disable();
    pRenderer->setHitbox(pHitbox);

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
    registry.killComponent(pHitbox);
}

void Spell::cast(Vector2 pos)
{
    pTransform->pos = pos;
    pRenderer->enable();
    isCast = true;
}

void Spell::onHitboxEnter(Entity *pEntity)
{
    if(!pEntity)
        return;

    Entity::onHitboxEnter(pEntity);

    if(alive && isCast && pEntity->getType() == EntityType::Enemy)
    {
        hit(pEntity);
    }

    if(pEntity->getType() != EntityType::Player && pEntity->getType() != EntityType::Spell)
    {
        kill();
    }
}

void Spell::setDir(Vector2 dir)
{
    this->dir = dir;

    if (pRenderer)
    {
        pTransform->rotRad = atan2(dir.x, dir.y / 2);
        pTransform->rotDeg = pTransform->rotRad * RAD_TO_DEGS;
        
        if(dir.x < 0)
            pRenderer->isFlipped = true;
    }
}