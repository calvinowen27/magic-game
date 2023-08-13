#ifndef SPELL_INCLUDE
#define SPELL_INCLUDE

#include "Vector2.hpp"
#include "Particle.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

enum class SpellType
{
    Projectile,
    Radial,
    Exploding
};

enum class SpellElement
{
    Ice,
    Water,
    Fire,
    Earth,
    Soul,
    Lightning
};

enum class SpellAttribute
{
    Chain
};

class SpellManager;

class Spell : public Particle
{
private:
    SpellManager &_spellManager;

    SpellElement _element;
    std::vector<SpellType> _types;
    std::vector<SpellAttribute> _attributes;
    Vector2 _dir;
    bool _cast = false;
    bool _alive = false;

    float _lifeDur = 2; // seconds
    float _aliveTime = 0;

    float _speed = 2; // m/s

public:
    Spell();
    bool init(string type, Vector2 pos, Vector2 dir, SpellElement element, vector<SpellType> types);
    void update(float time);

    inline void addType(SpellType type)
    {
        _types.push_back(type);
    }

    inline void addAttribute(SpellAttribute attribute)
    {
        _attributes.push_back(attribute);
    }

    void cast();
    void disable() override;
    void kill();

    inline bool isAlive() { return _alive; }
};

#endif