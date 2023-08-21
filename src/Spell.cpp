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
    
    if(_cast && _alive)
    {
        if(_aliveTime < _lifeDur)
        {
            _aliveTime += time;
            pRigidbody->velocity = _dir * _speed;
        }
        else
        {
            kill();
        }
    }

    Particle::update(time);
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