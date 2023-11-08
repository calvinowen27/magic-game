#include "../../include/game/Objects/Wall.hpp"
#include "../../include/nlohmann_json/json.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Level/LevelManager.hpp"

void Wall::pickState(Vector2Int pos)
{
    _possibleStates = {WallState::TopLeft, WallState::TopRight, WallState::Horizontal, WallState::Vertical, WallState::BottomLeft, WallState::BottomRight, WallState::NotLeft, WallState::NotRight, WallState::NotBottom, WallState::NotTop};

    int colBorderEnabled[4] = {1, 1, 1, 1};

    // left
    if (levelManager.isWallAtPos(pos + Vector2Int(-1, 0)))
    {
        _possibleStates.erase(WallState::TopLeft);
        _possibleStates.erase(WallState::BottomLeft);
        _possibleStates.erase(WallState::Vertical);
        _possibleStates.erase(WallState::NotLeft);
        colBorderEnabled[0] = 0;
    }

    // right
    if (levelManager.isWallAtPos(pos + Vector2Int(1, 0)))
    {
        _possibleStates.erase(WallState::TopRight);
        _possibleStates.erase(WallState::BottomRight);
        _possibleStates.erase(WallState::Vertical);
        _possibleStates.erase(WallState::NotRight);
        colBorderEnabled[1] = 0;
    }

    // bottom
    if (levelManager.isWallAtPos(pos + Vector2Int(0, -1)))
    {
        _possibleStates.erase(WallState::Horizontal);
        _possibleStates.erase(WallState::BottomLeft);
        _possibleStates.erase(WallState::BottomRight);
        _possibleStates.erase(WallState::NotBottom);
        colBorderEnabled[2] = 0;
    }

    // top
    if (levelManager.isWallAtPos(pos + Vector2Int(0, 1)))
    {
        _possibleStates.erase(WallState::Horizontal);
        _possibleStates.erase(WallState::TopLeft);
        _possibleStates.erase(WallState::TopRight);
        _possibleStates.erase(WallState::NotTop);
        colBorderEnabled[3] = 0;
    }

    if (_possibleStates.find(WallState::Horizontal) != _possibleStates.end())
        Object::init(EntityType::WallH, (Vector2)pos);
    else if (_possibleStates.find(WallState::Vertical) != _possibleStates.end())
        Object::init(EntityType::WallV, (Vector2)pos);
    else if (_possibleStates.find(WallState::TopLeft) != _possibleStates.end())
        Object::init(EntityType::WallTL, (Vector2)pos);
    else if (_possibleStates.find(WallState::TopRight) != _possibleStates.end())
        Object::init(EntityType::WallTR, (Vector2)pos);
    else if (_possibleStates.find(WallState::BottomLeft) != _possibleStates.end())
        Object::init(EntityType::WallBL, (Vector2)pos);
    else if (_possibleStates.find(WallState::BottomRight) != _possibleStates.end())
        Object::init(EntityType::WallBR, (Vector2)pos);
    else if (_possibleStates.find(WallState::NotLeft) != _possibleStates.end())
        Object::init(EntityType::WallNL, (Vector2)pos);
    else if (_possibleStates.find(WallState::NotRight) != _possibleStates.end())
        Object::init(EntityType::WallNR, (Vector2)pos);
    else if (_possibleStates.find(WallState::NotBottom) != _possibleStates.end())
        Object::init(EntityType::WallNB, (Vector2)pos);
    else if (_possibleStates.find(WallState::NotTop) != _possibleStates.end())
        Object::init(EntityType::WallNT, (Vector2)pos);
    else
        Object::init(EntityType::Wall, (Vector2)pos);

    if (pCollider)
    {
        for (int i = 0; i < 4; i++)
            pCollider->borderEnabled[i] = colBorderEnabled[i];
    }
}