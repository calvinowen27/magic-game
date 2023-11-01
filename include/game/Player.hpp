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
    Vector2 _moveDir;
    Vector2 _prevMoveDir;

public:
    std::shared_ptr<HealthComponent> pHealth;
    std::shared_ptr<AnimatorComponent> pAnimator;

    Player();
    bool init(EntityType entityType, Vector2 pos) override;
    void update(float time) override;
    void kill() override;

    inline void setPos(Vector2 newPos) { pTransform->pos = newPos; }
};

#endif