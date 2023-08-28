#include "../../include/game/Spells/RadialSpell.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Enemy.hpp"
#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/Player.hpp"

RadialSpell::RadialSpell() : Spell()
{
}

bool RadialSpell::init()
{
    Spell::init();

    pRenderer->setTexture("radial_spell");

    return true;
}

void RadialSpell::update(float time)
{
    if(!isCast)
        return;

    if(aliveTime < lifeDur)
    {
        aliveTime += time;
        _currRadius += speed * time;

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

void RadialSpell::cast(Vector2 pos)
{
    isCast = true;
    _startPos = pos;
    pTransform->pos = pos;

    if(attributes.find(SpellAttribute::Projectile) != attributes.end())
    {
        std::shared_ptr<ProjectileSpell> proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(0, 1));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(0, -1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, 0));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, 0));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, 1));
        proj->cast(pos);

        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(1, -1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, 1));
        proj->cast(pos);
        
        proj = spellManager.newSpell<ProjectileSpell>();
        proj->init();
        proj->setSpeed(5);
        proj->setDamage(5);
        proj->setLifeDur(2);
        proj->setDir(Vector2(-1, -1));
        proj->cast(pos);

        kill();

        return;
    }

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