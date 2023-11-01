#ifndef LEVEL_MANAGER_INCLUDE
#define LEVEL_MANAGER_INCLUDE

#include "Vector2.hpp"

#include <memory>
#include <string>
#include <map>

class Game;
class Object;
class ObjectManager;

class LevelManager
{
const std::string LEVEL_DIRECTORY = "../../level/";

private:
    static LevelManager *_pInstance;

    Game &_game;
    ObjectManager &_objectManager;

    std::map<Vector2, std::shared_ptr<Object>> _worldMap;

    int _currLevelIdx;
    int _currLevelHalfSize;

public:
    LevelManager();
    static LevelManager *getInstance();
    bool init();

    bool loadLevel(int levelIdx);
    bool unloadLevel();
    inline void nextLevel() { loadLevel(_currLevelIdx + 1); }
    inline void prevLevel() { loadLevel(_currLevelIdx - 1); }

    std::shared_ptr<Object> getObjAtPos(Vector2 pos);
    bool placeObjAtPos(std::shared_ptr<Object> obj, Vector2 pos);
    void removeObjAtPos(Vector2 pos);
};

#endif