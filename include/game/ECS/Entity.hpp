#ifndef ENTITY_INCLUDE
#define ENTITY_INCLUDE

#include "Registry.hpp"
#include "../Vector2.hpp"

#include <string>

class Game;
class ContentManager;
class Registry;

class Entity
{
protected:
    Game &game;
    ContentManager &contentManager;
    Registry &registry;

    std::string type;

    std::shared_ptr<RendererComponent> pRenderer;
    std::shared_ptr<TransformComponent> pTransform;

public:
    Entity(std::string type);
    virtual bool init(Vector2 pos);
    inline std::string getType() { return type; }
};

#endif