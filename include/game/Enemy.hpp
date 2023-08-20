#ifndef ENEMY_INCLUDE
#define ENEMY_INCLUDE

#include "Object.hpp"

class Enemy : public Object
{
private:
    float _pathFindTime = 5;
    float _timeSincePathFind = 0;
    int _dir = 1;

public:
    std::shared_ptr<HealthComponent> pHealth;

    Enemy();
    void update(float time) override;
};

#endif