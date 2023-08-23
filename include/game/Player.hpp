#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

// #include "Game.h"
#include "Object.hpp"
#include <map>

class KeyboardHandler;

class Player : public Object
{
private:
    KeyboardHandler *_pKeyboardHandler;

public:
    std::shared_ptr<HealthComponent> pHealth;

    Player();
    void update(float time) override;
    void kill() override;
};

#endif