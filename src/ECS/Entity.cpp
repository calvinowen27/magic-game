#include "../../include/game/ECS/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/ObjectManager.hpp"

Entity::Entity(std::string type) : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager), objectManager(*game.pObjectManager)
{
    this->type = type;
}

bool Entity::init(Vector2 pos)
{
    alive = true;

    pTransform = registry.newComponent<TransformComponent>();
    pTransform->init(pos);

    pRenderer = registry.newComponent<RendererComponent>();
    pRenderer->init(type, pTransform);

    return true;
}

void Entity::kill()
{
    alive = false;

    registry.killComponent(pTransform);
    registry.killComponent(pRenderer);

    // std::shared_ptr<Entity> thisEntity(this);
    // objectManager.killEntity(thisEntity);
}

void Entity::onCollisionEnter(Entity *pOther)
{
}

void Entity::onCollisionExit(Entity *pOther)
{
}

void Entity::whileTouching(Entity *pOther)
{
}