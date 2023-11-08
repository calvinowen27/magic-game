#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE

#include <string>
#include <memory>

#include "../../SDL2/SDL.h"

#include "../Objects/Object.hpp"
#include "../Util/Vector2.hpp"
#include "Components.hpp"

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

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<RendererComponent> pRenderer;

    std::string type;

public:
    Particle();
    bool init(std::string type, float lifeDur);
    void update(float time);
    void disable();
    void kill();
};

#endif