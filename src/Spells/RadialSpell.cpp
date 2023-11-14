#include "../../include/game/Spells/RadialSpell.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Entity/Enemy.hpp"
#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/Entity/Player.hpp"

RadialSpell::RadialSpell() : Spell()
{
}

bool RadialSpell::init()
{
    Spell::init();

    pRenderer->setTexture("radial_spell");

    pHitbox->start = Vector2(0.5, 0.5);
    pHitbox->end = Vector2(0.5, 0.5);

    return true;
}

void RadialSpell::update(float time)
{
    if(!isCast)
        return;

    if(aliveTime < lifeDur)
    {
        aliveTime += time;
        _currRadius += speed * time;
        pTransform->dims = Vector2(_currRadius, _currRadius) * 2;
        pTransform->root = Vector2(0.5, 1) * pTransform->dims;
    }
    else
    {
        kill();
    }
}

void RadialSpell::setDir(Vector2 dir)
{
    this->dir = dir;
}

void RadialSpell::cast(Vector2 pos)
{
    isCast = true;
    _startPos = pos - Vector2(0, 0.5);
    pTransform->pos = _startPos;

    if(hasAttribute(SpellAttribute::Projectile))
    {
        std::shared_ptr<ProjectileSpell> proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(0, 1));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(0, -1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, 0));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, 0));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, 1));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, -1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, 1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, -1));
        proj->cast(pos);

        kill();

        return;
    }

    pRenderer->enable();
}

void RadialSpell::hit(Entity *pEntity)
{
    Spell::hit(pEntity);
}

void RadialSpell::kill()
{
    _entitiesHit.clear();

    Spell::kill();
}

void RadialSpell::onHitboxEnter(Entity *pEntity)
{
    if(!pEntity)
        return;

    Entity::onHitboxEnter(pEntity);

    if(alive && isCast && pEntity->getType() == EntityType::Enemy)
    {
        hit(pEntity);
    }

    if(pEntity->getType() != EntityType::Player && pEntity->getType() != EntityType::Spell && (pEntity->getType() < EntityType::Wall || pEntity->getType() > EntityType::WallNT))
    {
        kill();
    }
}