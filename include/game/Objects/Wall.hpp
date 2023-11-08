#ifndef WALL_INCLUDE
#define WALL_INCLUDE

#include "../Util/Vector2.hpp"
#include "../Objects/Object.hpp"

#include <map>
#include <set>
#include <string>

enum class WallState
{
    TopLeft,
    TopRight,
    Horizontal,
    Vertical,
    BottomLeft,
    BottomRight
};

class Wall : public Object
{
private:
    std::set<WallState> _possibleStates;

public:
    void pickState(Vector2Int pos);
};

#endif