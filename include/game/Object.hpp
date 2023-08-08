#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include "../SDL2/SDL.h"
#include "Vector2.hpp"
#include "ECS/Components.hpp"

class Game;
class ObjectManager;
class ContentManager;
class Registry;

class Object
{
protected:
    Game *pGame;
    ObjectManager *pObjectManager;
    ContentManager *pContentManager;
    Registry *pRegistry;

    RendererComponent *pRenderer;
    TransformComponent *pTransform;
    RigidbodyComponent *pRigidbody;
    ColliderComponent *pCollider;

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
    inline ColliderComponent *getCollider() { return pCollider; }
};

#endif