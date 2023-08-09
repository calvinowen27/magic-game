#ifndef COMPONENT_HANDLER_INCLUDE
#define COMPONENT_HANDLER_INCLUDE

#include "../../SDL2/SDL.h"

class Game;
class Registry;

class ComponentHandler
{
private:
    Game &_game;
    Registry &_registry;

public:
    ComponentHandler();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);
    void updateTransforms(float time);
    void updateColliders(float time);
};

#endif