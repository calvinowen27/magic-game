#ifndef RADIAL_SPELL_INCLUDE
#define RADIAL_SPELL_INCLUDE

#include "Spell.hpp"

#include <set>
#include <memory>

class RadialSpell : public Spell
{
private:
    float _currRadius = 0;
    Vector2 _startPos;

    std::set<std::shared_ptr<Entity>> _entitiesHit;

public:
    RadialSpell();
    bool init() override;
    void update(float time) override;
    void cast(Vector2 pos) override;
    void hit(Entity *pEntity) override;
    void kill() override;
};

#endif