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

        std::shared_ptr<T> entity = _game.pObjectManager->newEntity<T>();
        auto spell = std::dynamic_pointer_cast<Spell>(entity);
        spell->init();
        spell->setLifeDur(0.25f);
        spell->setSpeed(20);

        return entity;
    }

    template <typename T>
    static void createSpell()
    {
        auto spellManager = Game::getInstance()->pSpellManager;
        if (spellManager->_pCurrSpell && std::is_same<ProjectileSpell, T>::value)
        {
            spellManager->_pCurrSpell->addAttribute(SpellAttribute::Projectile);
            return;
        }
        if (spellManager->_pCurrSpell && std::is_same<RadialSpell, T>::value)
        {
            spellManager->_pCurrSpell->addAttribute(SpellAttribute::Radial);
            return;
        }

        spellManager->setCurrSpell(std::dynamic_pointer_cast<Spell>(spellManager->newSpell<T>()));
    }

    inline std::shared_ptr<Spell> getCurrSpell() { return _pCurrSpell; }
    inline void setCurrSpell(std::shared_ptr<Spell> spell) { _pCurrSpell = spell; }
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