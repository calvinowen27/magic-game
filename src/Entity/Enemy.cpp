#include "../../include/game/Entity/Enemy.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Spells/Spell.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Level/LevelManager.hpp"

Enemy::Enemy() : LiveEntity()
{
}

bool Enemy::init(Vector2 pos)
{
    LiveEntity::init(EntityType::Enemy, pos);

    pRenderer->setEntity(this);

    return true;
}

void Enemy::update(float time)
{
    if (!alive)
        return;

    LiveEntity::update(time);

    Vector2 playerPos = game.pPlayer->getPos();
    _dir = (playerPos - pTransform->pos).normalized();
    Vector2 dir = _dir / fmax(fabs(_dir.x), fabs(_dir.y));
    Vector2 checkPos = pTransform->pos;

    bool success = true;

    int max = 0; // 0 = y, 1 = x
    if(_dir.x > _dir.y)
        max = 1;

    int mode = 0; // 0 = max(dir), 1 = min(dir)

    while((Vector2Int)checkPos != (Vector2Int)playerPos)
    {
        if(levelManager.isWallAtPos((Vector2Int)checkPos))
        {
            success = false;
            break;
        }

        if(mode == 0)
        {
            if(max == 0)
            {
                checkPos += Vector2(0, dir.y);
            }
            else
            {
                checkPos += Vector2(dir.x, 0);
            }

            mode = 1;
        }
        else
        {
            if(max == 0)
            {
                checkPos += Vector2(dir.x, 0);
            }
            else
            {
                checkPos += Vector2(0, dir.y);
            }

            mode = 0;
        }
    }

    if(success)
    {
        pRigidbody->velocity = _dir * _speed;
    }
    else
    {
        pRigidbody->velocity = Vector2::zero;
    }

    // if (Vector2::distance(game.pPlayer->getPos(), pTransform->pos) < 3)
    // {
    //     if (_timeSincePathFind > _pathFindTime)
    //     {
    //         _dir = (game.pPlayer->getPos() - pTransform->pos).normalized();
    //         _timeSincePathFind = 0;
    //     }
    //     else
    //     {
    //         _timeSincePathFind += time;
    //         pRigidbody->velocity = _dir * _speed;
    //     }
    // }

    if (_timeSinceHit < _hitCooldown)
    {
        _timeSinceHit += time;
    }
}

void Enemy::onCollisionEnter(Entity *pOther)
{
    if (pOther && pOther->getType() == EntityType::Player && _timeSinceHit >= _hitCooldown)
    {
        Player *player = dynamic_cast<Player *>(pOther);
        player->damage(_damage);
        _timeSinceHit = 0;
    }
}