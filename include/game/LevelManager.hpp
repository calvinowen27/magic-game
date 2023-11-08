#ifndef LEVEL_MANAGER_INCLUDE
#define LEVEL_MANAGER_INCLUDE

#include "Vector2.hpp"

#include <memory>
#include <string>
#include <map>

class Game;
class Object;
class ObjectManager;
class TransformComponent;

class LevelManager
{
const std::string LEVEL_DIRECTORY = "../../level/";

private:
    static LevelManager *_pInstance;

    Game &_game;
    ObjectManager &_objectManager;

    std::shared_ptr<Object> **_world; // 2D array, each cell containing an object

    int _currLevelIdx = -1; // -1 at start of game

    int _currLevelWidth;
    int _currLevelHeight;
    int _currLevelHalfWidth;
    int _currLevelHalfHeight;

    Vector2 _playerStartPos; // position to place player when level loads
    Vector2 _levelBoundsStart;
    Vector2 _levelBoundsEnd;

public:
    LevelManager();
    static LevelManager *getInstance();

    bool loadLevel(int levelIdx); // creates world map and initializes all objects, returns true if levelIdx valid and successful, false otherwise
    bool unloadLevel(); // unloads the current level, returns true if successful
    inline void nextLevel() { loadLevel(_currLevelIdx + 1); }
    inline void prevLevel() { loadLevel(_currLevelIdx - 1); }

    std::shared_ptr<Object> getObjAtPos(Vector2Int pos); // returns object at pos, nullptr if nothing there or if position outside of level dimensions
    bool placeObjAtPos(std::shared_ptr<Object> obj, Vector2Int pos); // places obj at pos if there is nothing at that position, otherwise does nothing, returns true if successful
    void removeObjAtPos(Vector2Int pos); // kills object at pos

    inline Vector2 getLevelBoundsStart() { return _levelBoundsStart; }
    inline Vector2 getLevelBoundsEnd() { return _levelBoundsEnd; }
    bool isTransformInLevel(const TransformComponent &transform);
};

#endif