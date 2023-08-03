#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include "../SDL2/SDL.h"
#include "Vector2.hpp"
#include "CollisionBox.hpp"
// #include "ObjectManager.hpp"
// #include "Game.hpp"
#include "Components.hpp"

class Game;
class ObjectManager;
class ContentManager;

class Object
{
protected:
    Game *pGame;
    ObjectManager *pObjectManager;
    ContentManager *pContentManager;

    Vector2 velocity;  // m/s

    TransformComponent *pTransform;

    std::string type;
    CollisionBox colBox;

    float colLeftX;
    float colRightX;
    float colTopY;
    float colBottomY;

    bool isStatic = true;

private:
    int _borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool _doCollisions = true;
    bool _flipped = false;

    float _textureAngle = 0; // degrees

    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;

public:
    Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions = true);
    virtual ~Object();
    virtual void draw(SDL_Renderer *pRenderer);
    virtual void update(float time);
    void handleCollisions(Vector2 deltaPos);

    inline std::string getType() { return type; }
    inline Vector2 getVelocity() { return velocity; }
    inline Vector2 pos() { return pTransform->pos(); }
    inline Vector2Int pxPos() { return pTransform->pxPos(); }
    inline Vector2 dims() { return pTransform->dims(); }
    inline Vector2Int pxDims() { return pTransform->pxDims(); }
    inline SDL_Texture *getTexture() { return _pTexture; }

    inline void disableBorder(int i) { _borderEnabled[i] = 0; }
    inline void enableBorder(int i) { _borderEnabled[i] = 1; }
};

#endif