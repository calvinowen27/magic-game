#ifndef REGISTRY_INCLUDE
#define REGISTRY_INCLUDE

#include "Components.hpp"
#include <vector>
#include "../../SDL2/SDL.h"

class Registry
{
private:
    std::vector<TransformComponent *> _transformComponents;

public:
    Registry();
    ~Registry();
    void update(float time);
    void draw(SDL_Renderer *pRenderer);

    inline void addTransform(TransformComponent *pTransform) { _transformComponents.push_back(pTransform); }
};

#endif