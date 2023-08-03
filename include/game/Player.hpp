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
    Player(Vector2 pos);
    void update(float time) override;
};

#endif