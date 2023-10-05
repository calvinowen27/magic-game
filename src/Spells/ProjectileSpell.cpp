#include "../../include/game/Spells/ProjectileSpell.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Enemy.hpp"
#include "../../include/game/Spells/SpellManager.hpp"

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