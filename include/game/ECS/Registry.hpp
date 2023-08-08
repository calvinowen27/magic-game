#ifndef REGISTRY_INCLUDE
#define REGISTRY_INCLUDE

#include "Components.hpp"
#include <vector>
#include "../../SDL2/SDL.h"

class Registry
{
private:
    std::vector<TransformComponent *> _transforms;
    std::vector<ColliderComponent *> _colliders;
    std::vector<RigidbodyComponent *> _rigidbodies;

public:
    Registry();
    ~Registry();
    void update(float time);
    void draw(SDL_Renderer *pRenderer);

    inline std::vector<TransformComponent *> getTransforms() { return _transforms; }
    inline std::vector<ColliderComponent *> getColliders() { return _colliders; }
    inline std::vector<RigidbodyComponent *> getRigidbodies() { return _rigidbodies; }

    Component *newComponent(ComponentType type);
};

#endif