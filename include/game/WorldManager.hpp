#ifndef WORLD_MANAGER_INCLUDE
#define WORLD_MANAGER_INCLUDE

#include <string>
#include <memory>

using std::shared_ptr;
using std::string;

class Game;
class Object;
class ObjectManager;
class Entity;

class WorldManager
{
public:
    static const int WORLD_SIZE = 200;

private:
    static WorldManager *_pInstance;

    Game &_game;
    ObjectManager &_objectManager;

    shared_ptr<Entity> _worldMap[WORLD_SIZE][WORLD_SIZE];

    string _filePath = "./world/world.txt";

public:
    WorldManager();
    static WorldManager *getInstance();
    void loadWorld();
};

#endif