#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include <string>
#include <memory>

#include "../../SDL2/SDL.h"

#include "../Vector2.hpp"

using std::shared_ptr;
using std::string;

class Game;
class ContentManager;
class TransformComponent;
class RigidbodyComponent;

class Component
{
protected:
    Game &game;

public:
    Component();
};

class RendererComponent : public Component
{
public:
    ContentManager &contentManager;

    SDL_Texture *pTexture;
    SDL_Rect spriteRect;

    shared_ptr<TransformComponent> pTransform;

    float spriteAngle = 0; // sprite rotation angle, degrees
    bool isFlipped = false; // over y axis
    bool isEnabled = true; // whether or not to render

    RendererComponent();
    ~RendererComponent();
    bool init(string objType, shared_ptr<TransformComponent> pTransform); // returns true if successful
    void update(float time);
    void draw(SDL_Renderer *pRenderer);
    bool setTexture(std::string textureName); // returns true if successful
};

class TransformComponent : public Component
{
public:
    Vector2 pos; // meters
    Vector2Int pxPos; // pixels
    Vector2 dims; // meters
    Vector2Int pxDims; // pixels

    TransformComponent();
    bool init(Vector2 pos, Vector2 dims); // returns true if successful
    void update(float time);
};

class ColliderComponent : public Component
{
public:
    Vector2 start; // scalar of dims, relative to bottom left of object
    Vector2 end; // scalar of dims, relative to top right of object

    shared_ptr<TransformComponent> pTransform;
    shared_ptr<RigidbodyComponent> pRigidbody;

    // positions of each border of collider
    float leftX;
    float rightX;
    float topY;
    float bottomY;

    int borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool doCollisions = true;

    ColliderComponent();
    bool init(Vector2 start, Vector2 end, shared_ptr<TransformComponent> pTransform, shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions = true); // returns true if successful
    void update(float time);
};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false; // doesn't move if static

    Vector2 velocity; // meter/sec

    shared_ptr<TransformComponent> pTransform;
    shared_ptr<ColliderComponent> pCollider;

    RigidbodyComponent();
    bool init(shared_ptr<TransformComponent> pTransform, shared_ptr<ColliderComponent> pCollider, bool isStatic = false); // returns true if successful
    void update(float time);
};

#endif