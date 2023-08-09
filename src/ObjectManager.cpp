#include "../include/game/ObjectManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Object.hpp"

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
}

void ObjectManager::update(float time)
{
    // obj.update();
    for (auto obj : _pInstance->_objects)
    {
        obj->update(time);
    }
}