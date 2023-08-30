#include "../include/game/ObjectManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Grass.hpp"

#include <fstream>
#include <type_traits>

ObjectManager *ObjectManager::_pInstance;

ObjectManager::ObjectManager() : _game(*Game::getInstance())
{
}

ObjectManager::~ObjectManager()
{
}

ObjectManager *ObjectManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new ObjectManager();
    }

    return _pInstance;
}

void ObjectManager::init()
{
    std::ifstream f(_entityDataPath);
    _entityData = json::parse(f);
    f.close();
}

void ObjectManager::update(float time)
{
    // obj.update();
    std::vector<std::shared_ptr<Entity>> deadEntities;

    for (auto entity : _entities)
    {
        if (entity->isAlive())
            entity->update(time);
        else
            deadEntities.push_back(entity);
    }

    for (auto entity : deadEntities)
        killEntity(entity);
}

std::vector<Vector2> ObjectManager::getCollider(EntityType entityType)
{
    std::string typeString = Entity::getStringFromType(entityType);
    if (!_entityData["colliders"].contains(typeString))
    {
        std::cerr << "ObjectManager::getCollider(entityType): Invalid entityType '" << typeString << "'. Returning {Vector2(0, 0), Vector2(1, 0.5)}." << std::endl;
        return std::vector<Vector2>{Vector2::zero, Vector2(1, 0.5)};
    }
    auto endpoints = _entityData["colliders"][typeString];
    return std::vector<Vector2>{Vector2((float)endpoints["start"][0], (float)endpoints["start"][1]), Vector2((float)endpoints["end"][0], (float)endpoints["end"][1])};
}

Vector2Int ObjectManager::getSpriteDims(EntityType entityType)
{
    std::string typeString = Entity::getStringFromType(entityType);
    if (!_entityData["spriteDims"].contains(typeString))
    {
        std::cerr << "ObjectManager::getSpriteDims(entityType): Invalid entityType '" << typeString << "'. Returning Vector2Int(16, 16)." << std::endl;
        return Vector2Int(16, 16);
    }
    auto spriteDims = _entityData["spriteDims"][typeString];
    return Vector2Int((int)spriteDims[0], (int)spriteDims[1]);
}