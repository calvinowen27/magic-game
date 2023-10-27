#include "../../include/game/ECS/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/Input/MouseHandler.hpp"

std::map<EntityType, std::string> Entity::_stringFromType = {
    {EntityType::Player, "Player"},
    {EntityType::Enemy, "Enemy"},
    {EntityType::Wall, "Wall"},
    {EntityType::Grass, "Grass"},
    {EntityType::Spell, "Spell"}
};

std::map<std::string, EntityType> Entity::_typeFromString = {
    {"Player", EntityType::Player},
    {"Enemy", EntityType::Enemy},
    {"Wall", EntityType::Wall},
    {"Grass", EntityType::Grass},
    {"Spell", EntityType::Spell}
};

Entity::Entity() : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager), objectManager(*game.pObjectManager)
{
}

bool Entity::init(EntityType type, Vector2 pos)
{
    this->type = type;
    alive = true;

    pTransform = registry.newComponent<TransformComponent>();
    pRenderer = registry.newComponent<RendererComponent>();

    pTransform->init(pos);
    pRenderer->init(type, pTransform);
    // pRenderer->enable();

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