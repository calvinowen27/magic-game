#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include <string>
#include <memory>
#include <vector>

#include "../../SDL2/SDL.h"

#include "../Vector2.hpp"

using std::shared_ptr;
using std::string;

class Game;
class ContentManager;
class TransformComponent;
class RigidbodyComponent;
class Registry;
class Object;
class Entity;

class Component
{
protected:
    Game &game;
    Registry &registry;

    bool enabled;

public:
    Component();
    bool init();
    void enable() { enabled = true; }
    void disable() { enabled = false; }
};

class RendererComponent : public Component
{
public:
    ContentManager &contentManager;

    SDL_Texture *pTexture;
    SDL_Rect spriteRect;

    std::shared_ptr<TransformComponent> pTransform;

    float spriteAngle = 0;  // sprite rotation angle, degrees
    bool isFlipped = false; // over y axis

    int renderOrder = 0;

    RendererComponent();
    ~RendererComponent();
    RendererComponent *init(string objType, shared_ptr<TransformComponent> pTransform, int renderOrder = 0); // returns true if successful
    void update(float time);
    void draw(SDL_Renderer *pRenderer);
    bool setTexture(std::string textureName); // returns true if successful
    void refreshDimensions();
};

class TransformComponent : public Component
{
public:
    Vector2 pos;       // meters
    Vector2Int pxPos;  // pixels
    Vector2 dims;      // meters
    Vector2Int pxDims; // pixels

    TransformComponent();
    TransformComponent *init(Vector2 pos, Vector2 dims = Vector2(1, 1)); // returns true if successful
    void update(float time);

    void setDims(Vector2 newDims);        // set new dimensions
    void setPxDims(Vector2Int newPxDims); // set new px dims
};

class ColliderComponent : public Component
{
public:
    Vector2 start; // scalar of dims, relative to bottom left of object
    Vector2 end;   // scalar of dims, relative to top right of object

    shared_ptr<TransformComponent> pTransform;
    shared_ptr<RigidbodyComponent> pRigidbody;

    Entity *pEntity;

    // positions of each border of collider
    float leftX;
    float rightX;
    float topY;
    float bottomY;

    // colliders that are touching this on
    // std::vector<ColliderComponent> collidersTouching;

    int borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool doCollisions = true;

    ColliderComponent();
    ColliderComponent *init(Vector2 start, Vector2 end, shared_ptr<TransformComponent> pTransform, shared_ptr<RigidbodyComponent> pRigidbody, Entity *pEntity, bool doCollisions = true); // returns true if successful
    void update(float time);
    // void onCollisionEnter(ColliderComponent &other);
    // void onCollisionExit(ColliderComponent &other);
    // void whileTouching(ColliderComponent &other);

};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false; // doesn't move if static

    Vector2 velocity; // meters/sec

    shared_ptr<TransformComponent> pTransform;
    shared_ptr<ColliderComponent> pCollider;

    RigidbodyComponent();
    RigidbodyComponent *init(shared_ptr<TransformComponent> pTransform, shared_ptr<ColliderComponent> pCollider, bool isStatic = false); // returns true if successful
    void update(float time);
};

class HealthComponent : public Component
{
public:
    float baseHealth;
    float health;

    shared_ptr<RendererComponent> pRedRenderer;
    shared_ptr<RendererComponent> pGreenRenderer;

    HealthComponent();
    HealthComponent *init(float baseHealth);
    void heal(float healAmount);
    bool damage(float dmgAmount); // returns true if dead after damage dealt
};

#endif