#include "../include/game/Spell.hpp"
#include "../include/game/SpellManager.hpp"

Spell::Spell(std::string type, Vector2 pos, Vector2 dir, SpellElement element, std::vector<SpellType> types) : Particle(type, pos, Vector2(1, 1), 2)
{
    _pSpellManager = SpellManager::getInstance();
    _element = element;
    _types = types;
    _dir = dir;
    _cast = true;
    _alive = true;

    _pSpellManager->spells.push_back(this);
}

void Spell::update(float time)
{
    Particle::update(time);

    if(_cast && _alive)
    {
        if(_aliveTime < _lifeDur)
        {
            _aliveTime += time;

            pTransform->pos += _dir * _speed * time;
        }
        else
        {
            if(_types.size() > 1)
            {
                std::vector<SpellType> newTypes;
                for(int i = 1; i < _types.size(); i++)
                {
                    newTypes.push_back(_types[i]);
                }

                if(_types[1] == SpellType::Exploding)
                {
                    std::cout << "EXPLODE!" << std::endl;
                    new Spell(type, pTransform->pos, _dir, _element, newTypes);
                }

                if(_types[1] == SpellType::Projectile)
                {
                    new Spell(type, pTransform->pos, _dir, _element, newTypes);
                }

                if(_types[1] == SpellType::Radial)
                {
                    new Spell(type, pTransform->pos, Vector2(0, 1), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(1, 1).normalized(), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(1, 0), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(1, -1).normalized(), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(0, -1), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(-1, -1).normalized(), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(-1, 0), _element, newTypes);
                    new Spell(type, pTransform->pos, Vector2(-1, 1).normalized(), _element, newTypes);
                }
            }

            _alive = false;
            // _pSpellManager->removeSpell(this);
        }
    }
}

void Spell::cast()
{
    _cast = true;
}