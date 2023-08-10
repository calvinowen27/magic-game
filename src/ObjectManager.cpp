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
    for (auto obj : _pInstance->_objects)
    {
        obj->update(time);
    }
}

vector<Vector2> ObjectManager::getCollider(string objType)
{
    if(!_colliderData["colliders"].contains(objType))
    {
        std::cerr << "Invalid objType '" << objType << "'. Returning {Vector2(0, 0), Vector2(1, 1)}." << std::endl;
        return vector<Vector2>{Vector2::zero, Vector2(1, 1)};
    }
    auto endpoints = _colliderData["colliders"][objType];
    return vector<Vector2>{Vector2((float)endpoints["start"][0], (float)endpoints["start"][1]), Vector2((float)endpoints["end"][0], (float)endpoints["end"][1])};
}