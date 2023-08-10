#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"
#include "../nlohmann_json/json.hpp"
#include "Vector2.hpp"

#include <vector>
#include <string>
#include <map>
#include <memory>

class Game;
class Object;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using json = nlohmann::json;

class ObjectManager
{
private:
    static ObjectManager *_pInstance;

    Game &_game;

    vector<shared_ptr<Object>> _objects;

    string _colliderDataPath = "./data/colliders.json";
    json _colliderData;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void update(float time);

    vector<Vector2> getCollider(string objType);

    template <typename T>
    shared_ptr<T> newObj()
    {
        auto obj = make_shared<T>();
        _objects.push_back(obj);
        return obj;
    }
};

#endif