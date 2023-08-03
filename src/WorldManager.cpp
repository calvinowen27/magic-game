#include "../include/game/WorldManager.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Wall.hpp"
#include "../include/game/Grass.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/Game.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

WorldManager *WorldManager::_pInstance;

WorldManager::WorldManager()
{
    _pGame = Game::getInstance();
}

WorldManager *WorldManager::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new WorldManager();
    }

    return _pInstance;
}

void WorldManager::loadWorld()
{
    std::ifstream in;
    in.open(_filePath);
    if (!in.is_open())
    {
        std::cerr << "Unable to open file " << _filePath << " for reading." << std::endl;
        return;
    }

    std::string line;
    uint32_t i, j;
    int x, y;

    Vector2 pos;

    for (x = 0; x < WORLD_SIZE; x++)
    {
        for (y = 0; y < WORLD_SIZE; y++)
        {
            _worldMap[x][y] = nullptr;
        }
    }

    y = 0;

    while (std::getline(in, line))
    {
        for (i = 0; i < line.length(); i++)
        {
            pos.x = i;
            x = i;

            switch (line[i])
            {
            case '#':
                _worldMap[x][y] = new Wall(pos);
                break;
            case ',':
                _worldMap[x][y] = new Grass(pos);
                break;
            case '@':
                _pGame->pPlayer = new Player(pos);
                break;
            }
        }

        pos.y+=0.5f;
        y++;
    }

    Object *obj;

    for (i = 0; i < WORLD_SIZE; i++)
    {
        for (j = 0; j < WORLD_SIZE; j++)
        {
            if (_worldMap[i][j] == nullptr)
                continue;

            obj = _worldMap[i][j];

            if (obj->getType() == "wall")
            {
                if (i > 0 && _worldMap[i - 1][j] != nullptr && _worldMap[i - 1][j]->getType() == "wall")
                    obj->disableBorder(0);

                if (i < WORLD_SIZE - 1 && _worldMap[i + 1][j] != nullptr && _worldMap[i + 1][j]->getType() == "wall")
                    obj->disableBorder(1);

                if (j > 0 && _worldMap[i][j - 1] != nullptr && _worldMap[i][j - 1]->getType() == "wall")
                    obj->disableBorder(2);

                if (j < WORLD_SIZE - 1 && _worldMap[i][j + 1] != nullptr && _worldMap[i][j + 1]->getType() == "wall")
                    obj->disableBorder(3);
            }
        }
    }
}