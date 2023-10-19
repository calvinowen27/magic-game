#ifndef SPELL_INCLUDE
#define SPELL_INCLUDE

#include "../Vector2.hpp"
#include "../ECS/Entity.hpp"

#include <string>
#include <vector>
#include <set>

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
    Chain,
    Projectile,
    Radial,
    Trail
};

class SpellManager;

class Spell : public Entity
{
protected:
    SpellManager &spellManager;

    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<ColliderComponent> pCollider;

    SpellElement element;
    std::set<SpellAttribute> attributes;
    Vector2 dir;
    bool isCast = false;

    float lifeDur = 5; // seconds
    float aliveTime = 0;

    float speed = 5; // m/s

    float damage = 2;

public:
    Spell();
    virtual bool init();
    void setElement(SpellElement element);
    inline void setDamage(float damage) { this->damage = damage; }
    inline void setLifeDur(float lifeDur) { this->lifeDur = lifeDur; }
    inline void setSpeed(float speed) { this->speed = speed; }
    inline void setDir(Vector2 dir) { this->dir = dir; }
    inline void addAttribute(SpellAttribute attribute) { attributes.emplace(attribute); }
    void update(float time) override;
    virtual void cast(Vector2 pos);
    inline bool hasBeenCast() { return isCast; }
    virtual void hit(Entity *pEntity);

    void kill() override;

    virtual void onCollisionEnter(Entity *pEntity) override;
};

#endif