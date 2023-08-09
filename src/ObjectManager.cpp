#include "../include/game/ObjectManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/WorldManager.hpp"
#include "../include/game/Object.hpp"

ObjectManager *ObjectManager::_pInstance;

ObjectManager::ObjectManager() : _game(*Game::getInstance())
{
    // _pGame = Game::getInstance();
    _pWorldManager = WorldManager::getInstance();
}

ObjectManager::~ObjectManager()
{
    for (Object *obj : _pInstance->_objs)
    {
        delete obj;
    }
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
    for (Object *obj : _pInstance->_objs)
    {
        obj->update(time);
    }
}