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