#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include "../Vector2.hpp"

class Game;
class Registry;

class Component
{
protected:
    Game *pGame;
    Registry *pRegistry;

public:
    Component();
};

class TransformComponent : public Component
{
private:
    Vector2 _pos;
    Vector2Int _pxPos;
    Vector2 _dims;
    Vector2Int _pxDims;

public:
    TransformComponent(Vector2 pos, Vector2 dims);
    void update(float time);

    inline Vector2& pos() { return _pos; }
    inline Vector2Int pxPos() { return _pxPos; }
    inline Vector2 dims() { return _dims; }
    inline Vector2Int pxDims() { return _pxDims; }
};

#endif