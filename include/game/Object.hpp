#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include <memory>

#include "../SDL2/SDL.h"

#include "ECS/Entity.hpp"

class Game;
class ObjectManager;
class ContentManager;

class Object : public Entity
{
protected:
    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<ColliderComponent> pCollider;

public:
    Object();
    bool init(std::string type, Vector2 pos) override;
    void update(float time) override;
    void kill() override;

    inline std::string getType() { return type; }
    inline Vector2 getVelocity() { return pRigidbody->velocity; }
    inline Vector2 pos() { return pTransform->pos; }
    inline Vector2Int pxPos() { return pTransform->pxPos; }
    inline Vector2 dims() { return pTransform->dims; }
    inline Vector2Int pxDims() { return pTransform->pxDims; }
    inline std::shared_ptr<ColliderComponent> getCollider() { return pCollider; }
};

#endif