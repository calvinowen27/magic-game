#include "../../include/game/Spells/ProjectileSpell.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Enemy.hpp"
#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/Spells/TrailSpell.hpp"

ProjectileSpell::ProjectileSpell() : Spell()
{
}

bool ProjectileSpell::init()
{
    Spell::init();

    pRenderer->setTexture("projectile_spell");

    return true;
}

void ProjectileSpell::update(float time)
{
    Spell::update(time);

    if(pRenderer)
        pRenderer->spriteAngle = atan2(dir.x, dir.y) * RAD_TO_DEGS;

    if(!_hasTrail)
    {
        if(!(_hasTrail = hasAttribute(SpellAttribute::Trail)))
            return;
    }

    if(_timeSinceTrail < _trailCreateTime)
    {
        _timeSinceTrail += time;
    }
    else
    {
        _timeSinceTrail = 0;

        auto trail = spellManager.newSpell<TrailSpell>();
        trail->init();
        trail->setDamage(1.5);
        trail->setLifeDur(5);
        trail->setSpeed(0);
        trail->setDir(Vector2::zero);
        trail->cast(pTransform->pos);
    }
}

void ProjectileSpell::kill()
{
    // if(attributes.find(SpellAttribute::Radial) != attributes.end())
    
    if(hasAttribute(SpellAttribute::Radial))
    {
        auto radial = spellManager.newSpell<RadialSpell>();
        radial->init();
        radial->setDamage(3);
        radial->setLifeDur(1);
        radial->setSpeed(3.5);
        radial->cast(pTransform->pos + (pTransform->dims / 2));
    }

    Spell::kill();
}