#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/UI/UIGroup.hpp"
#include "../../include/game/UI/UIManager.hpp"

SpellManager::SpellManager() : _game(*Game::getInstance())
{
}

void SpellManager::createSpell()
{
    SpellAttribute attribute;
    while(_spellAttributes.size())
    {
        attribute = _spellAttributes.front();
        _spellAttributes.pop();
        
        switch(attribute)
        {
            /* RADIAL */
            case SpellAttribute::Radial:
                if (_pCurrSpell)
                {
                    _pCurrSpell->addAttribute(SpellAttribute::Radial);
                }
                else
                {
                    auto spell = newSpell<RadialSpell>();
                    spell->init();
                    spell->setDamage(3);
                    spell->setLifeDur(1);
                    spell->setSpeed(3.5);
                }
                break;

            /* PROJECTILE */
            case SpellAttribute::Projectile:
                if (_pCurrSpell)
                {
                    _pCurrSpell->addAttribute(SpellAttribute::Projectile);
                }
                else
                {
                    auto spell = newSpell<ProjectileSpell>();
                    spell->init();
                    spell->setDamage(4);
                    spell->setLifeDur(2.5);
                    spell->setSpeed(5);
                }
                break;
        }
    }
}

void SpellManager::castCurrSpell(Vector2 pos, Vector2 dir)
{
    _pCurrSpell = nullptr;
    createSpell();

    if (_pCurrSpell)
    {
        _pCurrSpell->setDir(dir);
        _pCurrSpell->cast(pos);
        _pCurrSpell = nullptr;
    }

    _hasValidSpell = false;
}

void SpellManager::killCurrSpell()
{
    if (_pCurrSpell)
    {
        _pCurrSpell->kill();
    }

    _pCurrSpell = nullptr;
}