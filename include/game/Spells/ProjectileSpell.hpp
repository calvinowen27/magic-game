#ifndef PROJECTILE_SPELL_INCLUDE
#define PROJECTILE_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>

class ProjectileSpell : public Spell
{
private:
    bool _hasTrail = false;

    float _trailCreateTime = 0.25;
    float _timeSinceTrail = 0;

public:
    ProjectileSpell();
    bool init() override;
    void update(float time) override;
    void kill() override;
};

#endif