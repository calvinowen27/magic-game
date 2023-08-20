#include "../include/game/Enemy.hpp"
#include "../include/game/ECS/Registry.hpp"

Enemy::Enemy() : Object("Enemy")
{
    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
}

void Enemy::update(float time)
{
    if(_timeSincePathFind > _pathFindTime)
    {
        _dir = -_dir;
        _timeSincePathFind = 0;
    }
    else
    {
        _timeSincePathFind += time;
        pRigidbody->velocity = Vector2(2*_dir, 0);
    }

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Object::update(time);
}