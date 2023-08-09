#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

class Game;
class Object;

using std::vector;
using std::shared_ptr;
using std::make_shared;

class ObjectManager
{
private:
    static ObjectManager *_pInstance;

    Game &_game;

    // vector<Object *> _objs;
    vector<shared_ptr<Object>> _objects;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void update(float time);

    // inline void addObject(Object *obj) { _objs.push_back(obj); }
    // inline std::vector<Object *> &getObjs() { return _objs; }

    template <typename T>
    shared_ptr<T> newObj()
    {
        auto obj = make_shared<T>();
        _objects.push_back(obj);
        return obj;
    }
};

#endif