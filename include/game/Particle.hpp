#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE

#include <string>
#include <memory>

#include "../SDL2/SDL.h"

#include "Object.hpp"
#include "Vector2.hpp"
#include "ECS/Components.hpp"

class Game;
class ContentManager;
class Registry;

class Particle
{
protected:
    Game &game;
    ContentManager &contentManager;
    Registry &registry;

    shared_ptr<TransformComponent> pTransform;

    string type;

private:
    SDL_Texture *_pTexture;
    SDL_Rect _spriteRect;


    float _textureAngle = 0; // degrees
    bool _flipped = false;

public:
    Particle(string type, Vector2 pos, Vector2 dims, float lifeDur);
    ~Particle();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);
};

#endif