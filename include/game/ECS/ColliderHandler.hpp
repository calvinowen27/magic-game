#ifndef COLLIDER_HANDLER_INCLUDE
#define COLLIDER_HANDLER_INCLUDE

#include <map>
#include <string>
#include <vector>
#include "../Vector2.hpp"

class Game;
class Registry;
class ColliderComponent;

class ColliderHandler
{
private:
    Game *_pGame;
    Registry *_pRegistry;

    std::map<std::string, std::vector<Vector2>> _colliderEndpoints;

public:
    ColliderHandler(Registry *pRegistry);
    void update(float time);

    std::vector<Vector2> getEndsOfType(std::string type);
};

#endif