#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

// #include "Game.h"
#include "LiveEntity.hpp"
#include <map>

class KeyboardHandler;

class Player : public LiveEntity
{
private:
    KeyboardHandler *_pKeyboardHandler;
    Vector2 _moveDir;
    Vector2 _prevMoveDir;

public:
    std::shared_ptr<AnimatorComponent> pAnimator;

    Player();
    bool init(EntityType entityType, Vector2 pos) override;
    void update(float time) override;
    void kill() override;

    inline void setPos(Vector2 newPos) { pTransform->pos = newPos; }
    inline bool damage(float damage) { return pHealth->damage(damage); }
};

#endif