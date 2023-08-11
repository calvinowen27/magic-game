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
    shared_ptr<RendererComponent> pRenderer;

    string type;

public:
    Particle();
    bool init(string type, Vector2 pos, Vector2 dims, float lifeDur);
    void update(float time);
    void kill();
};

#endif