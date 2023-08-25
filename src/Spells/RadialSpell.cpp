#include "../../include/game/Spells/RadialSpell.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Enemy.hpp"

bool RadialSpell::init(Vector2 pos, SpellElement element, float damage, float lifeDur)
{
    Spell::init(EntityType::RadialSpell, pos, Vector2::zero, element, damage, lifeDur);

    _startPos = pos;

    return true;
}

void RadialSpell::update(float time)
{
    if(!isCast)
        return;

    if(aliveTime < lifeDur)
    {
        aliveTime += time;
        _currRadius += _expansionSpeed * time;

        auto entities = objectManager.getEntities();
        std::shared_ptr<Enemy> enemy;
        Vector2 enemyBL;
        Vector2 enemyTL;
        Vector2 enemyBR;
        Vector2 enemyTR;

        for(auto pEntity : entities)
        {
            if(pEntity->getType() != EntityType::Enemy)
                continue;

            if(_entitiesHit.find(pEntity) != _entitiesHit.end())
                continue;
            
            enemy = std::dynamic_pointer_cast<Enemy>(pEntity);
            enemyBL = Vector2(enemy->getCollider()->leftX, enemy->getCollider()->bottomY);
            enemyTL = Vector2(enemy->getCollider()->leftX, enemy->getCollider()->topY);
            enemyBR = Vector2(enemy->getCollider()->rightX, enemy->getCollider()->bottomY);
            enemyTR = Vector2(enemy->getCollider()->rightX, enemy->getCollider()->topY);

            if(Vector2::distance(enemyBL, _startPos) <= _currRadius || 
                Vector2::distance(enemyTL, _startPos) <= _currRadius ||
                Vector2::distance(enemyBR, _startPos) <= _currRadius ||
                Vector2::distance(enemyTR, _startPos) <= _currRadius)
            {
                enemy->pHealth->damage(damage);
                _entitiesHit.emplace(pEntity);
            }
        }

        pTransform->pos = _startPos - Vector2(_currRadius, _currRadius);
        pTransform->dims = Vector2(_currRadius, _currRadius) * 2;
    }
    else
    {
        kill();
    }
}

void RadialSpell::cast()
{
    isCast = true;
    pRenderer->enable();
}

void RadialSpell::hit(Entity *pEntity)
{
    Spell::hit(pEntity);
}

void RadialSpell::kill()
{
    _entitiesHit.clear();

    Spell::kill();
}