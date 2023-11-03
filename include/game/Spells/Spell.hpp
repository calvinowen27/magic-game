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

enum SpellAttribute
{
    Projectile = 0x01,
    Radial = 0x02,
    Trail = 0x04,
    Chain = 0x08
};

class SpellManager;

class Spell : public Entity
{
protected:
    SpellManager &spellManager;

    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<HitboxComponent> pHitbox;

    SpellElement element;
    uint8_t attributes;
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
    void setDir(Vector2 dir) ;
    inline void addAttribute(SpellAttribute attribute) { attributes |= attribute; }
    inline void removeAttribute(SpellAttribute attribute) { attributes &= (~attribute); }
    inline bool hasAttribute(SpellAttribute attribute) { return attributes & attribute; }
    inline void clearAttributes() { attributes = 0x00; }
    void update(float time) override;
    virtual void cast(Vector2 pos);
    inline bool hasBeenCast() { return isCast; }
    virtual void hit(Entity *pEntity);

    void kill() override;

    virtual void onHitboxEnter(Entity *pEntity) override;
};

#endif