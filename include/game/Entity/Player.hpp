#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

// #include "Game.h"
#include "LiveEntity.hpp"
#include <map>

class KeyboardHandler;
class InteractableObject;

class Player : public LiveEntity
{
private:
    KeyboardHandler *_pKeyboardHandler;
    Vector2 _moveDir;
    Vector2 _prevMoveDir;

    float _walkSpeed = 2.5; // meters/sec
    float _sprintSpeed = 5; // meters/sec

    InteractableObject *_pCurrInteractable;

public:
    std::shared_ptr<AnimatorComponent> pAnimator;

    Player();
    bool init(Vector2 pos);
    void update(float time) override;
    void kill() override;

    inline void setPos(Vector2 newPos) { pTransform->pos = newPos; }
    inline void setCurrInteractable(InteractableObject *pInteractable) { _pCurrInteractable = pInteractable; }
    inline InteractableObject *getCurrInteractable() { return _pCurrInteractable; }
};

#endif