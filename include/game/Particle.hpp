#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE

#include "Object.hpp"
#include "Vector2.hpp"
#include "Components.hpp"

#include "../SDL2/SDL.h"

#include <string>

class Game;
class ContentManager;

class Particle
{
protected:
    Game *pGame;
    ContentManager *pContentManager;

    TransformComponent *pTransform;

    std::string type;

private:
    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;


    float _textureAngle = 0; // degrees
    bool _flipped = false;

public:
    Particle(std::string type, Vector2 pos, Vector2 dims, float lifeDur);
    ~Particle();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);
};

#endif