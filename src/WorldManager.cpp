#include "../include/game/WorldManager.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Wall.hpp"
#include "../include/game/Grass.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ECS/Entity.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

WorldManager *WorldManager::_pInstance;

WorldManager::WorldManager() : _game(*Game::getInstance()), _objectManager(*_game.pObjectManager)
{
}

WorldManager *WorldManager::getInstance()
{
    if (_pInstance == nullptr)
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
                _worldMap[x][y] = _objectManager.newEntity<Wall>();
                _worldMap[x][y]->init(EntityType::Wall, pos);
                break;
            case ',':
                _worldMap[x][y] = _objectManager.newEntity<Grass>();
                _worldMap[x][y]->init(EntityType::Grass, pos);
                break;
            case '@':
                _game.pPlayer = _objectManager.newEntity<Player>();
                _game.pPlayer->init(EntityType::Player, pos);
                break;
            case '!':
                _objectManager.newEntity<Enemy>()->init(EntityType::Enemy, pos);
                break;
            }
        }

        pos.y += 0.5f;
        y++;
    }

    shared_ptr<Entity> entity;

    for (i = 0; i < WORLD_SIZE; i++)
    {
        for (j = 0; j < WORLD_SIZE; j++)
        {
            if (_worldMap[i][j] == nullptr)
                continue;

            entity = _worldMap[i][j];

            if (entity->getType() == EntityType::Wall)
            {
                auto wall = std::static_pointer_cast<Wall>(entity);

                if (i > 0 && _worldMap[i - 1][j] != nullptr && _worldMap[i - 1][j]->getType() == EntityType::Wall)
                {
                    wall->getCollider()->borderEnabled[0] = 0;
                }

                if (i < WORLD_SIZE - 1 && _worldMap[i + 1][j] != nullptr && _worldMap[i + 1][j]->getType() == EntityType::Wall)
                {
                    wall->getCollider()->borderEnabled[1] = 0;
                }

                if (j > 0 && _worldMap[i][j - 1] != nullptr && _worldMap[i][j - 1]->getType() == EntityType::Wall)
                {
                    wall->getCollider()->borderEnabled[2] = 0;
                }

                if (j < WORLD_SIZE - 1 && _worldMap[i][j + 1] != nullptr && _worldMap[i][j + 1]->getType() == EntityType::Wall)
                {
                    wall->getCollider()->borderEnabled[3] = 0;
                }
            }
        }
    }
}

void WorldManager::reset()
{
    int i, j;
    for(i = 0; i < WORLD_SIZE; i++)
    {
        for(j = 0; j < WORLD_SIZE; j++)
        {
            _worldMap[i][j] = nullptr;
        }
    }
}