#ifndef ENEMY_INCLUDE
#define ENEMY_INCLUDE

#include "Object.hpp"

class Enemy : public Object
{
public:
    std::shared_ptr<HealthComponent> pHealth;

    Enemy();
    void update(float time) override;
};

#endif