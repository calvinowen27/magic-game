#include "../include/game/SpellManager.hpp"
#include <algorithm>

using std::make_shared;

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
    vector<int> removeIdxs;
    int size = spells.size();
    for (int i = 0; i < size; i++)
    {
        if (spells[i]->isAlive())
            spells[i]->update(time);
        // else
        //     removeIdxs.push_back(i);
    }

    // for(auto i : removeIdxs)
    //     spells.erase(spells.begin() + i);
}

shared_ptr<Spell> SpellManager::newSpell()
{
    auto spell = make_shared<Spell>();
    spells.push_back(spell);
    return spell;
}