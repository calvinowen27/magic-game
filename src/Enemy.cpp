#include "../include/game/Enemy.hpp"
#include "../include/game/ECS/Registry.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/Spell.hpp"

Enemy::Enemy() : Object()
{
    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
    pHealth->setEntity(this);
}

void Enemy::update(float time)
{
    if(!alive)
        return;

    if (_timeSincePathFind > _pathFindTime)
    {
        _dir = -_dir;
        _timeSincePathFind = 0;
    }
    else
    {
        _timeSincePathFind += time;
        pRigidbody->velocity = Vector2(2 * _dir, 0);
    }

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Object::update(time);
}

void Enemy::kill()
{
    Object::kill();

    registry.killComponent(pHealth);
}