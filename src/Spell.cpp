#include "../include/game/Spell.hpp"
#include "../include/game/SpellManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/ObjectManager.hpp"

#include <type_traits>

Spell::Spell() : Entity("Spell"), spellManager(*game.pSpellManager)
{
}

bool Spell::init(Vector2 pos, Vector2 dir, SpellElement element, vector<SpellType> types)
{
    Entity::init(pos);

    this->element = element;
    this->types = types;
    this->dir = dir;
    this->isCast = true;
    this->alive = true;
    aliveTime = 0;

    pCollider = registry.newComponent<ColliderComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();

    pCollider->init(Vector2::zero, Vector2(1, 0.5f), pTransform, pRigidbody, this, true, true);
    pRigidbody->init(pTransform, pCollider);

    return true;
}

void Spell::update(float time)
{
    if(isCast && alive)
    {
        if(aliveTime < lifeDur)
        {
            aliveTime += time;
            pRigidbody->velocity = dir * speed;
        }
        else
        {
            kill();
        }
    }
}

void Spell::kill()
{
    alive = false;
    isCast = false;
    
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);

    Entity::kill();
}

void Spell::cast()
{
    isCast = true;
}

void Spell::onCollisionEnter(Entity *pOther)
{
    if(pOther->getType() == "Enemy")
    {
        Enemy *enemy = dynamic_cast<Enemy *>(pOther);
        if(enemy)
        {
            // enemy->pHealth->damage(damage);

            if(enemy->pHealth->damage(damage))
            {
                enemy->kill();
                // std::shared_ptr<Enemy> sharedEnemy(enemy);
                // objectManager.killEntity(sharedEnemy);
            }

            kill();
            
        }
        else
        {
            std::cerr << "Spell::onCollisionEnter() : failed to cast entity as enemy, incorrect type?\n";
        }
    }
}