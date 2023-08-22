#include "../../include/game/ECS/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"

Entity::Entity(std::string type) : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager)
{
    this->type = type;
}

bool Entity::init(Vector2 pos)
{
    pTransform = registry.newComponent<TransformComponent>();
    pTransform->init(pos);

    pRenderer = registry.newComponent<RendererComponent>();
    pRenderer->init(type, pTransform);

    return true;
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