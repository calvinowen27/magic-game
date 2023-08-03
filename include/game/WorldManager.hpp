#ifndef WORLD_MANAGER_INCLUDE
#define WORLD_MANAGER_INCLUDE

#include <string>

class Game;
class Object;

class WorldManager
{
public:
    static const int WORLD_SIZE = 50;

private:
    static WorldManager *_pInstance;

    Game *_pGame;

    Object *_worldMap[WORLD_SIZE][WORLD_SIZE];

    std::string _filePath = "./world/world.txt";

public:
    WorldManager();
    static WorldManager *getInstance();
    void loadWorld();
};

#endif