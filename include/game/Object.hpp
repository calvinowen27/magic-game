#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include <memory>

#include "../SDL2/SDL.h"

#include "Vector2.hpp"
#include "ECS/Components.hpp"

using namespace std;

class Game;
class ObjectManager;
class ContentManager;
class Registry;

class Object
{
protected:
    // Game *pGame;
    Game &game;
    ObjectManager &objectManager;
    ContentManager &contentManager;
    Registry &registry;

    shared_ptr<RendererComponent> pRenderer;
    shared_ptr<TransformComponent> pTransform;
    shared_ptr<RigidbodyComponent> pRigidbody;
    shared_ptr<ColliderComponent> pCollider;

    std::string type;

public:
    Object(std::string objType);
    ~Object();
    void init(Vector2 pos, Vector2 dims, bool doCollisions = true);
    virtual void update(float time);

    inline std::string getType() { return type; }
    inline Vector2 getVelocity() { return pRigidbody->velocity; }
    inline Vector2 pos() { return pTransform->pos; }
    inline Vector2Int pxPos() { return pTransform->pxPos; }
    inline Vector2 dims() { return pTransform->dims; }
    inline Vector2Int pxDims() { return pTransform->pxDims; }
    inline shared_ptr<ColliderComponent> getCollider() { return pCollider; }
};

#endif