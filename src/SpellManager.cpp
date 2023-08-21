#include "../include/game/SpellManager.hpp"
#include <algorithm>

using std::shared_ptr, std::make_shared, std::vector;

SpellManager *SpellManager::_pInstance;

SpellManager::SpellManager()
{
}

SpellManager *SpellManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new SpellManager();
    }

    return _pInstance;
}

void SpellManager::update(float time)
{
    auto spells = TypePool<Spell>::getInstance()->getAlive();
    vector<shared_ptr<Spell>> deadSpells;

    for(auto spell : spells)
    {
        if(spell->isAlive())
            spell->update(time);
        else
            deadSpells.push_back(spell);
    }

    releaseSpells(deadSpells);
}

shared_ptr<Spell> SpellManager::newSpell()
{
    return TypePool<Spell>::getInstance()->instantiate();
}

void SpellManager::releaseSpell(shared_ptr<Spell> spell)
{
    TypePool<Spell>::getInstance()->release(spell);
}

void SpellManager::releaseSpells(vector<shared_ptr<Spell>> spells)
{
    auto pool = TypePool<Spell>::getInstance();
    for(auto spell : spells)
        pool->release(spell);
}