#ifndef TRAIL_SPELL_INCLUDE
#define TRAIL_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>

class TrailSpell : public Spell
{
public:
    TrailSpell();
    bool init() override;
    void cast(Vector2 pos) override;
    void kill() override;
};

#endif