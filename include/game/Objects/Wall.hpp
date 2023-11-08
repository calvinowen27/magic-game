#ifndef WALL_INCLUDE
#define WALL_INCLUDE

#include "../Util/Vector2.hpp"
#include "../Objects/Object.hpp"

#include <map>
#include <string>

enum class WallState
{
    TopLeft,
    Top,
    TopRight,
    Left,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

class Wall : public Object
{
public:
    virtual bool init(Vector2 pos);
};

class WallTL : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallT : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallTR : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallL : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallR : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallBL : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallB : public Wall
{
public:
    bool init(Vector2 pos) override;
};

class WallBR : public Wall
{
public:
    bool init(Vector2 pos) override;
};

#endif