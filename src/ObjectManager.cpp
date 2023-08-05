#include "../include/game/ObjectManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/WorldManager.hpp"
#include "../include/game/Object.hpp"

ObjectManager *ObjectManager::_pInstance;

ObjectManager::ObjectManager()
{
    _pGame = Game::getInstance();
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

void ObjectManager::draw(SDL_Renderer *pRenderer)
{
    // draw objects, y value render order
    for(float y = (WorldManager::WORLD_SIZE / 2) - 1; y > -(WorldManager::WORLD_SIZE / 2); y -= 0.5f)
    {
        for (Object *obj : _objs)
        {
            if((float)((int)(obj->pos().y*4))/2.f == y)
                obj->draw(pRenderer);
        }
    }
}

void ObjectManager::update(float time)
{
    // obj.update();
    for (Object *obj : _pInstance->_objs)
    {
        obj->update(time);
    }
}