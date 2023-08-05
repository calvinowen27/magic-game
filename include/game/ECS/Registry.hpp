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

public:
    Registry();
    ~Registry();
    void update(float time);
    void draw(SDL_Renderer *pRenderer);

    inline void addTransform(TransformComponent *pTransform) { _transforms.push_back(pTransform); }
    inline std::vector<TransformComponent *> getTransforms() { return _transforms; }
    inline void addCollider(ColliderComponent *pCollider) { _colliders.push_back(pCollider); }
    inline std::vector<ColliderComponent *> getColliders() { return _colliders; }
};

#endif