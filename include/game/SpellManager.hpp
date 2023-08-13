#ifndef SPELL_MANAGER_INCLUDE
#define SPELL_MANAGER_INCLUDE

#include "Spell.hpp"
#include "ECS/Registry.hpp"

#include <vector>
#include <memory>

class SpellManager
{
private:
    static SpellManager *_pInstance;

public:
    std::vector<std::shared_ptr<Spell>> spells;

    SpellManager();
    static SpellManager *getInstance();
    void update(float time);

    std::shared_ptr<Spell> newSpell();
    void releaseSpell(std::shared_ptr<Spell> spell);
    void releaseSpells(std::vector<std::shared_ptr<Spell>> spells);
};

#endif