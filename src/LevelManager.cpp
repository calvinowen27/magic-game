#include "../include/game/LevelManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Wall.hpp"
#include "../include/game/Grass.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/Enemy.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

LevelManager *LevelManager::_pInstance;

LevelManager::LevelManager() : _game(*Game::getInstance()), _objectManager(*_game.pObjectManager)
{

}

bool LevelManager::init()
{
    return true;
}

bool LevelManager::loadLevel(int levelIdx)
{
    if(_currLevelIdx != -1)
        unloadLevel();
        
    // _worldMap.clear();

    _currLevelIdx = levelIdx;

    std::stringstream levelPath;
    levelPath << LEVEL_DIRECTORY << "level" << levelIdx << ".txt";

    std::ifstream in;
    in.open(levelPath.str());
    if(!in.is_open())
    {
        std::cerr << "Unable to open file " << levelPath.str() << " for reading." << std::endl;
        return false;
    }

    std::string line;
    int x, y, i;
    Vector2 pos, checkPos;

    std::getline(in, line);
    _currLevelHalfSize = std::stoi(line) / 2;

    for(x = -_currLevelHalfSize; x < _currLevelHalfSize; x++)
    {
        for(y = -_currLevelHalfSize; y < _currLevelHalfSize; y++)
        {
            _worldMap[Vector2(x, y)] = nullptr;
        }
    }

    pos.y = -_currLevelHalfSize;

    while (std::getline(in, line))
    {
        for (i = 0; i < line.length(); i++)
        {
            pos.x = i - _currLevelHalfSize;

            switch (line[i])
            {
            case '#':
                _worldMap[pos] = _objectManager.newEntity<Wall>();
                _worldMap[pos]->init(EntityType::Wall, pos);

                // left
                checkPos = pos + Vector2(-1, 0);
                if(_worldMap.find(checkPos) != _worldMap.end() && _worldMap[checkPos] && _worldMap[checkPos]->getType() == EntityType::Wall)
                {
                    _worldMap[pos]->getCollider()->borderEnabled[0] = 0;
                    _worldMap[checkPos]->getCollider()->borderEnabled[1] = 0;
                }

                // right
                checkPos = pos + Vector2(1, 0);
                if(_worldMap.find(checkPos) != _worldMap.end() && _worldMap[checkPos] && _worldMap[checkPos]->getType() == EntityType::Wall)
                {
                    _worldMap[pos]->getCollider()->borderEnabled[1] = 0;
                    // _worldMap[checkPos]->getCollider()->borderEnabled[0] = 0;
                }
                
                // bottom
                checkPos = pos + Vector2(0, -1);
                if(_worldMap.find(checkPos) != _worldMap.end() && _worldMap[checkPos] && _worldMap[checkPos]->getType() == EntityType::Wall)
                {
                    _worldMap[pos]->getCollider()->borderEnabled[2] = 0;
                    // _worldMap[checkPos]->getCollider()->borderEnabled[3] = 0;
                }

                // top
                checkPos = pos + Vector2(0, 1);
                if(_worldMap.find(checkPos) != _worldMap.end() && _worldMap[checkPos] && _worldMap[checkPos]->getType() == EntityType::Wall)
                {
                    _worldMap[pos]->getCollider()->borderEnabled[3] = 0;
                    _worldMap[checkPos]->getCollider()->borderEnabled[2] = 0;
                }

                break;
            case ',':
                _worldMap[pos] = _objectManager.newEntity<Grass>();
                _worldMap[pos]->init(EntityType::Grass, pos);
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

        pos.y++;
    }

    return true;
}

bool LevelManager::unloadLevel()
{
    int x, y;
    std::shared_ptr<Object> pObj;

    for(x = -_currLevelHalfSize; x < _currLevelHalfSize; x++)
    {
        for(y = -_currLevelHalfSize; y < _currLevelHalfSize; y++)
        {
            pObj = _worldMap[Vector2(x, y)];
            if(pObj)
                pObj->kill();
        }
    }

    _worldMap.clear();

    _objectManager.killEntitiesOfType<Enemy>();

    if(_game.pPlayer)
        _game.pPlayer->kill();

    return true;
}

LevelManager *LevelManager::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new LevelManager();
    }

    return _pInstance;
}

std::shared_ptr<Object> LevelManager::getObjAtPos(Vector2 pos)
{
    if(_worldMap.find(pos) != _worldMap.end())
        return _worldMap[pos];
    
    return nullptr;
}

bool LevelManager::placeObjAtPos(std::shared_ptr<Object> obj, Vector2 pos)
{
    if(_worldMap.find(pos) == _worldMap.end())
    {
        _worldMap[pos] = obj;
    } 

}

void LevelManager::removeObjAtPos(Vector2 pos)
{

}