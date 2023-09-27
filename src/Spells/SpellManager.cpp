#include "../../include/game/Spells/SpellManager.hpp"

SpellManager::SpellManager() : _game(*Game::getInstance())
{
}

void SpellManager::killCurrSpell()
{
    if (_pCurrSpell)
    {
        _pCurrSpell->kill();
    }

    _pCurrSpell = nullptr;
}

void SpellManager::castCurrSpell(Vector2 pos)
{
    _pCurrSpell->cast(pos);
    _pCurrSpell = nullptr;
}