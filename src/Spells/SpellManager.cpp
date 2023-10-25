#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/UI/UIGroup.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Spells/TrailSpell.hpp"

SpellManager::SpellManager() : _game(*Game::getInstance())
{
}

void SpellManager::createSpell()
{
    _pCurrSpell = nullptr;

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
                    std::shared_ptr<RadialSpell> spell = newSpell<RadialSpell>();
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
                    std::shared_ptr<ProjectileSpell> spell = newSpell<ProjectileSpell>();
                    spell->init();
                    spell->setDamage(4);
                    spell->setLifeDur(2.5);
                    spell->setSpeed(5);

                }
                break;

            /* TRAIL */
            case SpellAttribute::Trail:
                if(_pCurrSpell)
                {
                    _pCurrSpell->addAttribute(SpellAttribute::Trail);
                }
                else
                {
                    std::shared_ptr<TrailSpell> spell = newSpell<TrailSpell>();
                    spell->init();
                    spell->setDamage(1.5);
                    spell->setLifeDur(5);
                    spell->setSpeed(0);
                    spell->setDir(Vector2::zero);
                }
                break;
        }
    }
}

void SpellManager::castCurrSpell(Vector2 pos, Vector2 dir)
{
    _hasValidSpell = false;

    // createSpell();

    if (_pCurrSpell)
    {
        _pCurrSpell->setDir(dir);
        _pCurrSpell->cast(pos);

        _pCurrSpell = nullptr;
    }
}

void SpellManager::killCurrSpell()
{
    if (_pCurrSpell)
    {
        killSpell(_pCurrSpell);
    }

    _pCurrSpell = nullptr;
}