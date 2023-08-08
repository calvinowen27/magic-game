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

    TransformComponent *pTransform;
    RigidbodyComponent *pRigidbody;
    ColliderComponent *pCollider;

    std::string type;

private:
    bool _flipped = false;

    float _textureAngle = 0; // degrees

    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;

public:
    Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions = true);
    virtual ~Object();
    virtual void draw(SDL_Renderer *pRenderer);
    virtual void update(float time);

    inline std::string getType() { return type; }
    inline Vector2 getVelocity() { return pRigidbody->velocity; }
    inline Vector2 pos() { return pTransform->pos; }
    inline Vector2Int pxPos() { return pTransform->pxPos; }
    inline Vector2 dims() { return pTransform->dims; }
    inline Vector2Int pxDims() { return pTransform->pxDims; }
    inline SDL_Texture *getTexture() { return _pTexture; }
    inline ColliderComponent *getCollider() { return pCollider; }
};

#endif