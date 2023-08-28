#ifndef SPELL_MANAGER_INCLUDE
#define SPELL_MANAGER_INCLUDE

#include "Spell.hpp"
#include "RadialSpell.hpp"
#include "ProjectileSpell.hpp"
#include "../ObjectManager.hpp"
#include "../Game.hpp"

#include <memory>
#include <set>
#include <vector>
#include <type_traits>

class Game;

class SpellManager
{
private:
    std::shared_ptr<Spell> _pCurrSpell;

    Game &_game;

public:
    SpellManager();

    template <typename T>
    std::shared_ptr<T> newSpell()
    {
        if (!std::is_base_of<Spell, T>::value)
        {
            throw new std::invalid_argument("SpellManager::newSpell<Type>() : Type must be derived from Spell class.");
        }

        std::shared_ptr<T> spell = _game.pObjectManager->newEntity<T>();
        _pCurrSpell = std::dynamic_pointer_cast<Spell>(spell);

        return spell;
    }

    inline std::shared_ptr<Spell> getCurrSpell() { return _pCurrSpell; }
    inline void killCurrSpell()
    {
        if (_pCurrSpell)
        {
            _pCurrSpell->kill();
            _pCurrSpell = nullptr;
        }
    }
};

#endif