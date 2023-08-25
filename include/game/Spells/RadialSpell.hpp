#ifndef RADIAL_SPELL_INCLUDE
#define RADIAL_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>

class RadialSpell : public Spell
{
private:
    float _expansionSpeed = 5; // meters/sec
    float _currRadius = 0;
    Vector2 _startPos;

    std::set<std::shared_ptr<Entity>> _entitiesHit;

public:
    bool init(Vector2 pos, SpellElement element, float damage, float lifeDur);
    void update(float time) override;
    void cast() override;
    void hit(Entity *pEntity) override;
    void kill() override;
};

#endif