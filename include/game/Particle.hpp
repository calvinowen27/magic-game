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

    bool enabled;

    shared_ptr<TransformComponent> pTransform;
    shared_ptr<RendererComponent> pRenderer;

    string type;

public:
    Particle();
    bool init(string type, float lifeDur);
    void update(float time);
    void disable();
    void kill();
};

#endif