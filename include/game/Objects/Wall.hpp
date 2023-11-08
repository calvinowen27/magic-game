#ifndef WALL_INCLUDE
#define WALL_INCLUDE

#include "../Util/Vector2.hpp"
#include "../Objects/Object.hpp"

enum class WallState
{
    Left,
    Right,
    Bottom,
    Top
};

class Wall : public Object
{
private:
    WallState state;

public:
    Wall();
    bool init(WallState state, Vector2 pos);
};

#endif