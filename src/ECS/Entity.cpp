#include "../../include/game/ECS/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/ObjectManager.hpp"

Entity::Entity() : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager), objectManager(*game.pObjectManager)
{
}

bool Entity::init(std::string type, Vector2 pos)
{
    this->type = type;
    alive = true;

    pTransform = registry.newComponent<TransformComponent>();
    pRenderer = registry.newComponent<RendererComponent>();

    pTransform->init(pos);
    pRenderer->init(type, pTransform);

    return true;
}

void Entity::update(float time)
{
}

void Entity::kill()
{
    alive = false;

    registry.killComponent(pTransform);
    registry.killComponent(pRenderer);
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