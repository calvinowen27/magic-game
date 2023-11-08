#include "../../include/game/Entity/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Level/LevelManager.hpp"

std::map<EntityType, std::string> Entity::_stringFromType = {
    {EntityType::Player, "Player"},
    {EntityType::Enemy, "Enemy"},
    {EntityType::Wall, "Wall"},
    {EntityType::WallTL, "WallTL"},
    {EntityType::WallTR, "WallTR"},
    {EntityType::WallH, "WallH"},
    {EntityType::WallV, "WallV"},
    {EntityType::WallBL, "WallBL"},
    {EntityType::WallBR, "WallBR"},
    {EntityType::WallNL, "WallNL"},
    {EntityType::WallNR, "WallNR"},
    {EntityType::WallNB, "WallNB"},
    {EntityType::WallNT, "WallNT"},
    {EntityType::Grass, "Grass"},
    {EntityType::Spell, "Spell"}};

std::map<std::string, EntityType> Entity::_typeFromString = {
    {"Player", EntityType::Player},
    {"Enemy", EntityType::Enemy},
    {"Wall", EntityType::Wall},
    {"WallTL", EntityType::WallTL},
    {"WallTR", EntityType::WallTR},
    {"WallH", EntityType::WallH},
    {"WallV", EntityType::WallV},
    {"WallBL", EntityType::WallBL},
    {"WallBR", EntityType::WallBR},
    {"WallNL", EntityType::WallNL},
    {"WallNR", EntityType::WallNR},
    {"WallNB", EntityType::WallNB},
    {"WallNT", EntityType::WallNT},
    {"Grass", EntityType::Grass},
    {"Spell", EntityType::Spell}};

Entity::Entity() : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager), objectManager(*game.pObjectManager), levelManager(*game.pLevelManager)
{
}

bool Entity::init(EntityType type, Vector2 pos)
{
    this->type = type;
    alive = true;

    pTransform = registry.newComponent<TransformComponent>();
    pRenderer = registry.newComponent<RendererComponent>();

    pTransform->init(type, pos);
    pRenderer->init(type, pTransform);
    pRenderer->enable();

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

void Entity::onHitboxEnter(Entity *pOther)
{
}

void Entity::onHitboxExit(Entity *pOther)
{
}

void Entity::whileTouching(Entity *pOther)
{
}