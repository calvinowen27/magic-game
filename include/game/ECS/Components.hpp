#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include "../Vector2.hpp"
#include "../../SDL2/SDL.h"

class Game;
class RigidbodyComponent;

enum class ComponentType
{
    Transform,
    Collider,
    Rigidbody
};

class Component
{
protected:
    Game *pGame;

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

    TransformComponent();
    bool init(Vector2 pos, Vector2 dims);
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

    ColliderComponent();
    bool init(Vector2 start, Vector2 end, TransformComponent *pTransform, RigidbodyComponent *pRigidbody, bool doCollisions = true);
    void update(float time);
};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false;

    Vector2 velocity;

    TransformComponent *pTransform;
    ColliderComponent *pCollider;

    RigidbodyComponent();
    bool init(TransformComponent *pTransform, ColliderComponent *pCollider, bool isStatic = false);
    void update(float time);
};

#endif