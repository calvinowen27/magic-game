#include "../../include/game/Entity/Enemy.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Spells/Spell.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Game.hpp"

Enemy::Enemy() : LiveEntity()
{
}

bool Enemy::init(EntityType entityType, Vector2 pos)
{
    LiveEntity::init(entityType, pos);

    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
    pHealth->setEntity(this);

    return true;
}

void Enemy::update(float time)
{
    if (!alive)
        return;

    if (Vector2::distance(game.pPlayer->getPos(), pTransform->pos) < 3)
    {
        if (_timeSincePathFind > _pathFindTime)
        {
            _dir = (game.pPlayer->getPos() - pTransform->pos).normalized();
            _timeSincePathFind = 0;
        }
        else
        {
            _timeSincePathFind += time;
            pRigidbody->velocity = _dir * _speed;
        }
    }

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y + 0.5);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    if (_timeSinceHit < _hitCooldown)
    {
        _timeSinceHit += time;
    }

    LiveEntity::update(time);
}

void Enemy::kill()
{
    LiveEntity::kill();

    registry.killComponent(pHealth);
}

void Enemy::onCollisionEnter(Entity *pOther)
{
    if (pOther && pOther->getType() == EntityType::Player && _timeSinceHit >= _hitCooldown)
    {
        Player *player = dynamic_cast<Player *>(pOther);
        player->pHealth->damage(_damage);
        _timeSinceHit = 0;
    }
}