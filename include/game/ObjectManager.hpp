#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"

#include <vector>
#include <string>
#include <map>

class Game;
class WorldManager;
class Object;

class ObjectManager
{
private:
    static ObjectManager *_pInstance;

    Game *_pGame;
    WorldManager *_pWorldManager;

    std::vector<Object *> _objs;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);

    inline void addObject(Object *obj) { _objs.push_back(obj); }
    inline std::vector<Object *> &getObjs() { return _objs; }
};

#endif