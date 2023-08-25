#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"
#include "../nlohmann_json/json.hpp"
#include "Vector2.hpp"
#include "ECS/Registry.hpp"

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <set>

class Game;
class Object;

using json = nlohmann::json;

class ObjectManager
{
private:
    static ObjectManager *_pInstance;

    Game &_game;

    std::vector<std::shared_ptr<Object>> _objects;
    std::set<std::shared_ptr<Entity>> _entities;

    std::string _colliderDataPath = "./data/colliders.json";
    json _colliderData;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void update(float time);
    inline std::set<std::shared_ptr<Entity>> getEntities() { return _entities; }

    std::vector<Vector2> getCollider(EntityType entityType);

    template <typename T>
    std::shared_ptr<T> newEntity()
    {
        // do not use entity creation function if type is not a entity
        if (!std::is_base_of<Entity, T>::value)
        {
            throw new std::invalid_argument("ObjectManager::newEntity<Type>() : Type must be derived from Entity class.");
        }

        std::shared_ptr<T> entity = TypePool<T>::getInstance()->instantiate();
        _entities.emplace(entity);
        return entity;
    }

    template <typename T>
    void killEntity(std::shared_ptr<T> pEntity)
    {
        // do not use entity kill function if type is not a entity
        if (!std::is_base_of<Entity, T>::value)
        {
            throw new std::invalid_argument("ObjectManager::killEntity<Type>() : Type must be derived from Entity class.");
        }

        _entities.erase(pEntity);
        auto pool = TypePool<T>::getInstance();
        pool->release(pEntity);
    }
};

#endif