#include "../include/game/SpellManager.hpp"
#include <algorithm>

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
    for (int i = 0; i < size; i++)
    {
        if (spells[i]->isAlive())
            spells[i]->draw(pRenderer);
    }
}

void SpellManager::update(float time)
{
    // for (Spell *spell : spells)
    int size = spells.size();
    for (int i = 0; i < size; i++)
    {
        if (spells[i]->isAlive())
            spells[i]->update(time);
        else
        {
            spells.erase(spells.begin() + i);
            i--;
            size--;
        }
    }
}

void SpellManager::addSpell(Spell *spell)
{
    spells.push_back(spell);
}

void SpellManager::removeSpell(Spell *spell)
{
    spells.erase(std::remove(spells.begin(), spells.end(), spell), spells.end());
}