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
    std::ifstream f(_colliderDataPath);
    _colliderData = json::parse(f);
    f.close();
}

void ObjectManager::update(float time)
{
    // obj.update();
    std::vector<shared_ptr<Entity>> deadEntities;

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
    if (!_colliderData["colliders"].contains(typeString))
    {
        std::cerr << "Invalid objType '" << typeString << "'. Returning {Vector2(0, 0), Vector2(1, 0.5)}." << std::endl;
        return std::vector<Vector2>{Vector2::zero, Vector2(1, 0.5)};
    }
    auto endpoints = _colliderData["colliders"][typeString];
    return std::vector<Vector2>{Vector2((float)endpoints["start"][0], (float)endpoints["start"][1]), Vector2((float)endpoints["end"][0], (float)endpoints["end"][1])};
}