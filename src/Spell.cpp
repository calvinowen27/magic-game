#include "../include/game/Spell.hpp"
#include "../include/game/SpellManager.hpp"

Spell::Spell() : Particle(), _spellManager(*SpellManager::getInstance())
{
}

bool Spell::init(string type, Vector2 pos, Vector2 dir, SpellElement element, vector<SpellType> types)
{
    Particle::init(type, pos, Vector2(1, 1), 2);

    _element = element;
    _types = types;
    _dir = dir;
    _cast = true;
    _alive = true;
    _aliveTime = 0;

    return true;
}

void Spell::update(float time)
{
    if(!enabled)
        return;
    
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
                    _spellManager.newSpell()->init(type, pTransform->pos, _dir, _element, newTypes);
                }

                if(_types[1] == SpellType::Projectile)
                {
                    _spellManager.newSpell()->init(type, pTransform->pos, _dir, _element, newTypes);
                }

                if(_types[1] == SpellType::Radial)
                {
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(0, 1), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(1, 1).normalized(), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(1, 0), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(1, -1).normalized(), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(0, -1), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(-1, -1).normalized(), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(-1, 0), _element, newTypes);
                    _spellManager.newSpell()->init(type, pTransform->pos, Vector2(-1, 1).normalized(), _element, newTypes);
                }
            }

            kill();
        }
    }
}

void Spell::cast()
{
    _cast = true;
}

void Spell::disable()
{
    _alive = false;
    Particle::disable();
}

void Spell::kill()
{
    _alive = false;
    Particle::kill();
}