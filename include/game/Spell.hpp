#ifndef SPELL_INCLUDE
#define SPELL_INCLUDE

#include "Vector2.hpp"
#include "ECS/Entity.hpp"

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

class Spell : public Entity
{
protected:
    SpellManager &spellManager;

    shared_ptr<RigidbodyComponent> pRigidbody;
    shared_ptr<ColliderComponent> pCollider;

    SpellElement element;
    std::vector<SpellType> types;
    std::vector<SpellAttribute> attributes;
    Vector2 dir;
    bool isCast = false;

    float lifeDur = 5; // seconds
    float aliveTime = 0;

    float speed = 2; // m/s

    float damage = 4;

public:
    Spell();
    bool init(Vector2 pos, Vector2 dir, SpellElement element, vector<SpellType> types);
    void update(float time);
    void kill() override;

    inline void addType(SpellType type)
    {
        types.push_back(type);
    }

    inline void addAttribute(SpellAttribute attribute)
    {
        attributes.push_back(attribute);
    }

    void cast();

    void onCollisionEnter(Entity *pOther) override;
};

#endif