#ifndef ENEMY_INCLUDE
#define ENEMY_INCLUDE

#include "Object.hpp"

class Enemy : public Object
{
private:
    float _pathFindTime = 2;
    float _timeSincePathFind = 0;
    Vector2 _dir;
    float _speed = 2.5;

    float _timeSinceHit = 0;
    float _hitCooldown = 1;
    float _damage = 2;

public:
    std::shared_ptr<HealthComponent> pHealth;

    Enemy();
    void update(float time) override;
    void kill() override;
    void onCollisionEnter(Entity *pOther) override;
};

#endif