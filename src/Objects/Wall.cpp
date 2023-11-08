#include "../../include/game/Objects/Wall.hpp"
#include "../../include/nlohmann_json/json.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Level/LevelManager.hpp"

void Wall::pickState(Vector2Int pos)
{
    _possibleStates = {WallState::TopLeft, WallState::TopRight, WallState::Horizontal, WallState::Vertical, WallState::BottomLeft, WallState::BottomRight};

    int colBorderEnabled[4] = {1, 1, 1, 1};

    // left
    if (levelManager.isWallAtPos(pos + Vector2Int(-1, 0)))
    {
        _possibleStates.erase(WallState::TopLeft);
        _possibleStates.erase(WallState::BottomLeft);
        _possibleStates.erase(WallState::Vertical);
        colBorderEnabled[0] = 0;
    }

    // right
    if (levelManager.isWallAtPos(pos + Vector2Int(1, 0)))
    {
        _possibleStates.erase(WallState::TopRight);
        _possibleStates.erase(WallState::BottomRight);
        _possibleStates.erase(WallState::Vertical);
        colBorderEnabled[1] = 0;
    }

    // bottom
    if (levelManager.isWallAtPos(pos + Vector2Int(0, -1)))
    {
        _possibleStates.erase(WallState::Horizontal);
        _possibleStates.erase(WallState::BottomLeft);
        _possibleStates.erase(WallState::BottomRight);
        colBorderEnabled[2] = 0;
    }

    // top
    if (levelManager.isWallAtPos(pos + Vector2Int(0, 1)))
    {
        _possibleStates.erase(WallState::Horizontal);
        _possibleStates.erase(WallState::TopLeft);
        _possibleStates.erase(WallState::TopRight);
        colBorderEnabled[3] = 0;
    }

    if (_possibleStates.find(WallState::TopLeft) != _possibleStates.end())
        Object::init(EntityType::WallTL, (Vector2)pos);
    else if (_possibleStates.find(WallState::TopRight) != _possibleStates.end())
        Object::init(EntityType::WallTR, (Vector2)pos);
    else if (_possibleStates.find(WallState::Horizontal) != _possibleStates.end())
        Object::init(EntityType::WallH, (Vector2)pos);
    else if (_possibleStates.find(WallState::Vertical) != _possibleStates.end())
        Object::init(EntityType::WallV, (Vector2)pos);
    else if (_possibleStates.find(WallState::BottomLeft) != _possibleStates.end())
        Object::init(EntityType::WallBL, (Vector2)pos);
    else if (_possibleStates.find(WallState::BottomRight) != _possibleStates.end())
        Object::init(EntityType::WallBR, (Vector2)pos);

    for(int i = 0; i < 4; i++)
        pCollider->borderEnabled[i] = colBorderEnabled[i];
}