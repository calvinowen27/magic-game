#ifndef SPELL_MANAGER_INCLUDE
#define SPELL_MANAGER_INCLUDE

#include "Spell.hpp"

#include <vector>
#include <memory>

using std::shared_ptr;

class SpellManager
{
private:
    static SpellManager *_pInstance;

public:
    std::vector<shared_ptr<Spell>> spells;

    SpellManager();
    static SpellManager *getInstance();
    void update(float time);

    shared_ptr<Spell> newSpell();
};

#endif