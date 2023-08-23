#ifndef ENTITY_INCLUDE
#define ENTITY_INCLUDE

#include "Registry.hpp"
#include "../Vector2.hpp"

#include <string>

class Game;
class ContentManager;
class Registry;
class ObjectManager;

class Entity
{
protected:
    Game &game;
    ObjectManager &objectManager;
    ContentManager &contentManager;
    Registry &registry;

    std::string type;

    std::shared_ptr<RendererComponent> pRenderer;
    std::shared_ptr<TransformComponent> pTransform;

    bool alive;

public:
    Entity(std::string type);
    virtual bool init(Vector2 pos);
    virtual void kill();

    virtual void onCollisionEnter(Entity *pOther);
    virtual void onCollisionExit(Entity *pOther);
    virtual void whileTouching(Entity *pOther);

    inline std::string getType() { return type; }
    inline bool isAlive() { return alive; }
};

#endif