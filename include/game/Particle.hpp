#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE

#include "Object.hpp"
#include "Vector2.hpp"
#include "../SDL2/SDL.h"

#include <string>

class Game;
class ContentManager;

class Particle
{
protected:
    Game *_pGame;
    ContentManager *_pContentManager;

    Vector2 _pos;
    Vector2Int _pxPos;
    Vector2 _dims;
    Vector2Int _pxDims;

    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;

    std::string _type;

    float _textureAngle = 0; // degrees
    bool _flipped = false;

public:
    Particle(std::string type, Vector2 pos, Vector2 dims, float lifeDur);
    ~Particle();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);
};

#endif