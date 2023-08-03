#include "../include/game/SpellManager.hpp"

SpellManager *SpellManager::_pInstance;

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
    for (Spell *spell : spells)
    {
        delete spell;
    }
}

SpellManager *SpellManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new SpellManager();
    }

    return _pInstance;
}

void SpellManager::draw(SDL_Renderer *pRenderer)
{
    int size = spells.size();
    // for (Spell *spell : spells)
    for(int i = 0; i < size; i++)
    {
        if (spells[i]->isAlive())
            spells[i]->draw(pRenderer);
    }
}

void SpellManager::update(float time)
{
    // for (Spell *spell : spells)
    int size = spells.size();
    for(int i = 0; i < size; i++)
    {
        spells[i]->update(time);
    }
}

void SpellManager::addSpell(Spell *spell)
{
    spells.push_back(spell);
}