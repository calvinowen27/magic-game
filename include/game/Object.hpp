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
    Game *_pGame;
    ObjectManager *_pObjectManager;
    ContentManager *_pContentManager;

    // Vector2 _pos;       // m --> 128 pixels/m default
    //                     // position is at bottom left of object
    // Vector2Int _pxPos;  // pixel position, at top left of object
    // Vector2 _dims;      // dimensions (m)
    // Vector2Int _pxDims; // pixel dimensions

    Vector2 _velocity;  // m/s

    TransformComponent *_pTransform;

    std::string _type;
    CollisionBox _colBox;

    float colLeftX;
    float colRightX;
    float colTopY;
    float colBottomY;

    int borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool _doCollisions = true;
    bool _flipped = false;
    bool _isStatic = true;

    float _textureAngle = 0; // degrees

    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;

public:
    Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions = true);
    virtual ~Object();
    virtual void draw(SDL_Renderer *pRenderer);
    virtual void update(float time);
    void handleCollisions(Vector2 deltaPos);

    inline std::string type() { return _type; }
    inline Vector2 pos() { return _pTransform->pos(); }
    inline Vector2 velocity() { return _velocity; }
    inline Vector2Int pxPos() { return _pTransform->pxPos(); }
    inline Vector2 dims() { return _pTransform->dims(); }
    inline Vector2Int pxDims() { return _pTransform->pxDims(); }
    inline SDL_Texture *getTexture() { return _pTexture; }

    inline void disableBorder(int i) { borderEnabled[i] = 0; }
    inline void enableBorder(int i) { borderEnabled[i] = 1; }
};

#endif