#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include "../Vector2.hpp"
#include "../../SDL2/SDL.h"

class Game;
class Registry;
class RigidbodyComponent;

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
public:
    Vector2 pos;
    Vector2Int pxPos;
    Vector2 dims;
    Vector2Int pxDims;

    TransformComponent(Vector2 pos, Vector2 dims);
    void update(float time);
};

class ColliderComponent : public Component
{
public:
    Vector2 start;
    Vector2 end;

    TransformComponent *pTransform;
    RigidbodyComponent *pRigidbody;

    float leftX;
    float rightX;
    float topY;
    float bottomY;

    int borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool doCollisions = true;

    ColliderComponent(Vector2 start, Vector2 end, TransformComponent *pTransform, RigidbodyComponent *pRigidbody, bool doCollisions = true);
    void update(float time);
};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false;

    Vector2 velocity;

    TransformComponent *pTransform;
    ColliderComponent *pCollider;

    RigidbodyComponent(TransformComponent *pTransform, ColliderComponent *pCollider, bool isStatic = false);
    void update(float time);
};

#endif