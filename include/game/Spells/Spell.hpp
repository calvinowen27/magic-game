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

    float damage = 4;

public:
    Spell();
    bool init(Vector2 pos, Vector2 dir, SpellElement element);
    void update(float time);
    void kill() override;

    void cast();

    void onCollisionEnter(Entity *pOther) override;
};

#endif