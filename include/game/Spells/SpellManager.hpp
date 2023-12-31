#ifndef SPELL_MANAGER_INCLUDE
#define SPELL_MANAGER_INCLUDE

#include "Spell.hpp"
#include "RadialSpell.hpp"
#include "ProjectileSpell.hpp"
#include "../Objects/ObjectManager.hpp"
#include "../Game.hpp"

#include <memory>
#include <set>
#include <vector>
#include <type_traits>
#include <queue>

class Game;

class SpellManager
{
private:
    std::shared_ptr<Spell> _pCurrSpell;

    Game &_game;

    std::queue<SpellAttribute> _spellAttributes;
    bool _hasValidSpell = false;

    std::set<std::shared_ptr<Spell>> _spells;

public:
    SpellManager();

    template <typename T>
    std::shared_ptr<T> newSpell()
    {
        if (!std::is_base_of<Spell, T>::value)
        {
            throw new std::invalid_argument("SpellManager::newSpell<Type>() : Type must be derived from Spell class.");
            return nullptr;
        }
        else
        {
            std::shared_ptr<T> spell = _game.pObjectManager->newEntity<T>();
            try
            {
                _pCurrSpell = std::dynamic_pointer_cast<Spell>(spell);
                _spells.emplace(_pCurrSpell);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught: " << e.what() << std::endl;
                return nullptr;
            }

            return spell;
        }
    }

    inline std::shared_ptr<Spell> getCurrSpell() { return _pCurrSpell; }

    void createSpell();
    void castCurrSpell(Vector2 pos, Vector2 dir);
    void killCurrSpell();

    template <typename T>
    void killSpell(std::shared_ptr<T> spell)
    {
        if (!spell)
            return;

        // do not use spell kill function if type is not a spell
        if (!std::is_base_of<Spell, T>::value)
        {
            throw new std::invalid_argument("SpellManager::killSpell<Type>() : Type must be derived from Spell class.");
        }

        std::dynamic_pointer_cast<Spell>(spell)->kill();
        auto pool = TypePool<T>::getInstance();
        pool->release(spell);
    }

    void killAllSpells()
    {
        for(auto pSpell : _spells)
        {
            pSpell->kill();
        }

        _spells.clear();
    }

    inline void addSpellAttribute(SpellAttribute attribute)
    {
        _spellAttributes.push(attribute);
        _hasValidSpell = true;
    }
    inline void resetCurrSpell() { _pCurrSpell = nullptr; }
    inline bool hasValidSpell() { return _hasValidSpell; }
};

#endif