#ifndef SPELL_INCLUDE
#define SPELL_INCLUDE

#include "../Vector2.hpp"
#include "../ECS/Entity.hpp"

#include <string>
#include <vector>

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

class Spell : public Entity
{
protected:
    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<ColliderComponent> pCollider;

    SpellElement element;
    Vector2 dir;
    bool isCast = false;

    float lifeDur = 5; // seconds
    float aliveTime = 0;

    float speed = 5; // m/s

    float damage = 2;

public:
    Spell();
    virtual bool init(EntityType type, Vector2 dir, SpellElement element, float damage, float lifeDur);
    virtual void update(float time);
    virtual void cast(Vector2 pos);
    virtual void hit(Entity *pEntity);

    void kill() override;

    virtual void onCollisionEnter(Entity *pEntity) override;
};

#endif