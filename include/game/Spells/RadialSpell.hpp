#ifndef RADIAL_SPELL_INCLUDE
#define RADIAL_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>

class RadialSpell : public Spell
{
private:
    float _currRadius = 0;
    Vector2 _startPos;

    std::set<std::shared_ptr<Entity>> _entitiesHit;

public:
    RadialSpell();
    bool init();
    void update(float time) override;
    void cast(Vector2 pos) override;
    void hit(Entity *pEntity) override;
    void kill() override;
};

#endif