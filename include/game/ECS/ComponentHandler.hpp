#ifndef COMPONENT_HANDLER_INCLUDE
#define COMPONENT_HANDLER_INCLUDE

#include "../../SDL2/SDL.h"

#include <vector>
#include <memory>
#include <mutex>

class Game;
class Registry;
class RendererComponent;
class Vector2;
class ColliderComponent;

class ComponentHandler
{
private:
    Game &_game;
    Registry &_registry;

public:
    ComponentHandler();
    ~ComponentHandler();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);

    void updateColliders(float time);
    Vector2 handleCollision(std::shared_ptr<ColliderComponent> pCol1, std::shared_ptr<ColliderComponent> pCol2, Vector2 dPos);

    void updateRigidbodies(float time);
    void updateHitboxes(float time);
    void updateAnimators(float time);
};

bool rendererComparator(std::shared_ptr<RendererComponent> a, std::shared_ptr<RendererComponent> b);

#endif