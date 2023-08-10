#ifndef SPELL_MANAGER_INCLUDE
#define SPELL_MANAGER_INCLUDE

#include "Spell.hpp"

#include <vector>

class SpellManager
{
private:
    static SpellManager *_pInstance;

public:
    std::vector<Spell *> spells;

    SpellManager();
    ~SpellManager();
    static SpellManager *getInstance();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);
    void addSpell(Spell *spell);
    void removeSpell(Spell *spell);
};

#endif