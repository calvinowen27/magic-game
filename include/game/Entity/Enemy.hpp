#ifndef ENEMY_INCLUDE
#define ENEMY_INCLUDE

#include "LiveEntity.hpp"

class Enemy : public LiveEntity
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
    Enemy();
    bool init(EntityType entityType, Vector2 pos) override;
    void update(float time) override;
    void kill() override;
    void onCollisionEnter(Entity *pOther) override;
};

#endif