#include "../../include/game/Spells/TrailSpell.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Entity/Enemy.hpp"
#include "../../include/game/Spells/SpellManager.hpp"

TrailSpell::TrailSpell() : Spell()
{
}

bool TrailSpell::init()
{
    Spell::init();

    pRenderer->setTexture("projectile_spell");

    return true;
}

void TrailSpell::cast(Vector2 pos)
{
    Spell::cast(pos + dir);
}

void TrailSpell::kill()
{
    if(hasAttribute(SpellAttribute::Radial))
    {
        std::shared_ptr<RadialSpell> radial = spellManager.newSpell<RadialSpell>();
        radial->init();
        radial->setDamage(1);
        radial->setLifeDur(0.5);
        radial->setSpeed(4.5);
        radial->cast(pTransform->pos + (pTransform->dims / 2));
    }

    Spell::kill();
}