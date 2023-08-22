#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"
#include "../nlohmann_json/json.hpp"
#include "Vector2.hpp"

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

    std::string _colliderDataPath = "./data/colliders.json";
    json _colliderData;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void update(float time);

    std::vector<Vector2> getCollider(std::string objType);

    template <typename T>
    std::shared_ptr<T> newObj()
    {
        auto obj = std::make_shared<T>();
        // _objects.push_back(obj);
        _objects.emplace(obj);
        return obj;
    }
};

#endif