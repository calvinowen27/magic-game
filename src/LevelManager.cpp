#include "../include/game/LevelManager.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Wall.hpp"
#include "../include/game/Grass.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/Spells/SpellManager.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

LevelManager *LevelManager::_pInstance;

LevelManager::LevelManager() : _game(*Game::getInstance()), _objectManager(*_game.pObjectManager)
{
}

bool LevelManager::loadLevel(int levelIdx)
{
    // construct filepath for level .dat file
    std::stringstream levelPath;
    levelPath << LEVEL_DIRECTORY << "level" << levelIdx << ".dat";

    // open level .dat file
    std::ifstream in;
    in.open(levelPath.str());
    if (!in.is_open())
        return false; // invalid levelIdx

    if (_currLevelIdx != -1) // don't unnnecessarily unload level if no level is loaded
        unloadLevel();

    _currLevelIdx = levelIdx;

    std::string line;
    int wx, wy;
    Vector2 objPos;

    // get dimensions of level from first line of level file
    in >> _currLevelWidth >> _currLevelHeight;
    _currLevelHalfWidth = _currLevelWidth / 2;
    _currLevelHalfHeight = _currLevelHeight / 2;

    // allocate and initialize _world 2D array
    _world = new std::shared_ptr<Object> *[_currLevelWidth];

    for (wx = 0; wx < _currLevelWidth; wx++)
    {
        _world[wx] = new std::shared_ptr<Object>[_currLevelHeight];
        for (wy = 0; wy < _currLevelHeight; wy++)
        {
            _world[wx][wy] = nullptr;
        }
    }

    // middle of level (in file) should be position (0, 0) in game
    objPos.y = -_currLevelHalfHeight;

    std::getline(in, line); // for skipping first line, which contains dimensions of level
    for (wy = 0; wy < _currLevelHeight; wy++)
    {
        std::getline(in, line);
        for (wx = 0; wx < _currLevelWidth; wx++)
        {
            objPos.x = wx - _currLevelHalfWidth; // so middle of level is (0, 0)

            switch (line[wx])
            {
            /* ---------- WALL ------------------------------------- */
            case '#':
                // instantiate and initialize object, set position in world
                _world[wx][wy] = _objectManager.newEntity<Wall>();
                _world[wx][wy]->init(EntityType::Wall, objPos);

                // disable collider borders on sides with neighboring walls
                // left
                if (wx > 0 && _world[wx - 1][wy] && _world[wx - 1][wy]->getType() == EntityType::Wall)
                {
                    _world[wx][wy]->getCollider()->borderEnabled[0] = 0;
                    _world[wx - 1][wy]->getCollider()->borderEnabled[1] = 0;
                }

                // top
                if (wy > 0 && _world[wx][wy - 1] && _world[wx][wy - 1]->getType() == EntityType::Wall)
                {
                    _world[wx][wy]->getCollider()->borderEnabled[2] = 0;
                    _world[wx][wy - 1]->getCollider()->borderEnabled[3] = 0;
                }

                break;

            /* ---------- GRASS --------------------------------- */
            case ',':
                _world[wx][wy] = _objectManager.newEntity<Grass>();
                _world[wx][wy]->init(EntityType::Grass, objPos);
                break;

            /* ---------- PLAYER ------------------------------------ */
            case '@':
                // player not stored in map

                _playerStartPos = objPos; // set starting player position for the level

                // if player is alive, move to correct position, otherwise respawn
                if (_game.pPlayer && _game.pPlayer->isAlive())
                {
                    _game.pPlayer->setPos(_playerStartPos);
                }
                else
                {
                    _game.pPlayer = _objectManager.newEntity<Player>();
                    _game.pPlayer->init(EntityType::Player, objPos);
                }
                break;

            /* ---------- ENEMY ------------------------------------------------ */
            case '!':
                // enemy not stored in map

                _objectManager.newEntity<Enemy>()->init(EntityType::Enemy, objPos);
                break;
            }
        }

        objPos.y++;
    }

    return true;
}

bool LevelManager::unloadLevel()
{
    int wx, wy;
    std::shared_ptr<Object> pObj;

    for (wx = 0; wx < _currLevelWidth; wx++)
    {
        for (wy = 0; wy < _currLevelHeight; wy++)
        {
            // if object at position, kill
            pObj = _world[wx][wy];
            if (pObj)
            {
                pObj->kill();
                _world[wx][wy] = nullptr;
            }
        }

        // deallocate row
        delete[] _world[wx];
    }

    // deallocate array
    delete _world;

    // kill all enemies
    _objectManager.killEntitiesOfType<Enemy>();
    _game.pSpellManager->killAllSpells();

    return true;
}

LevelManager *LevelManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new LevelManager();
    }

    return _pInstance;
}

std::shared_ptr<Object> LevelManager::getObjAtPos(Vector2Int pos)
{
    // if pos within bounds of level, return object at that position, nullptr otherwise
    if (pos.x >= -_currLevelHalfWidth && pos.x < _currLevelHalfWidth &&
        pos.y >= -_currLevelHalfHeight && pos.y < _currLevelHalfHeight)
    {
        return _world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight];
    }

    return nullptr;
}

bool LevelManager::placeObjAtPos(std::shared_ptr<Object> obj, Vector2Int pos)
{
    // if pos within bounds at level and nothing at pos, place obj at pos
    if (pos.x >= -_currLevelHalfWidth && pos.x < _currLevelHalfWidth &&
        pos.y >= -_currLevelHalfHeight && pos.y < _currLevelHalfHeight &&
        !_world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight])
    {
        _world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight] = obj;
        return true;
    }

    return false;
}

void LevelManager::removeObjAtPos(Vector2Int pos)
{
    // if pos within bounds of level and there is an object at pos, kill object and remove from world
    if (pos.x >= -_currLevelHalfWidth && pos.x < _currLevelHalfWidth &&
        pos.y >= -_currLevelHalfHeight && pos.y < _currLevelHalfHeight &&
        _world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight])
    {
        _world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight]->kill();
        _world[pos.x + _currLevelHalfWidth][pos.y + _currLevelHalfHeight] = nullptr;
    }
}