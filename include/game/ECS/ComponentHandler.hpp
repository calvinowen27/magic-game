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
    Vector2 handleCollisionBoxBox(std::shared_ptr<ColliderComponent> box1, std::shared_ptr<ColliderComponent> box2, Vector2 dPos);
    Vector2 handleCollisionCircleBox(std::shared_ptr<ColliderComponent> circle, std::shared_ptr<ColliderComponent> box, Vector2 dPos);
    Vector2 handleCollisionCircleCircle(std::shared_ptr<ColliderComponent> circle1, std::shared_ptr<ColliderComponent> circle2, Vector2 dPos);

    void updateAnimators(float time);
};

bool rendererComparator(std::shared_ptr<RendererComponent> a, std::shared_ptr<RendererComponent> b);

#endif