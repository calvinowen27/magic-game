#ifndef PROJECTILE_SPELL_INCLUDE
#define PROJECTILE_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>

class ProjectileSpell : public Spell
{
public:
    ProjectileSpell();
    bool init() override;
    void update(float time) override;
    void kill() override;
};

#endif