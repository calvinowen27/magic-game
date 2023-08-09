#include "../include/game/Player.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/KeyboardHandler.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player() : Object()
{
    _pKeyboardHandler = game.pKeyboardHandler;
}

void Player::update(float time)
{
    Vector2 moveDir;

    if(_pKeyboardHandler->getInputState(InputKey::Right)) // move right
        moveDir.x += 1;
    if(_pKeyboardHandler->getInputState(InputKey::Left)) // move left
        moveDir.x -= 1;
    if(_pKeyboardHandler->getInputState(InputKey::Up)) // move up
        moveDir.y += 0.5f;
    if(_pKeyboardHandler->getInputState(InputKey::Down)) // move down
        moveDir.y -= 0.5f;

    if(_pKeyboardHandler->getInputState(InputKey::ZoomIn)) // zoom out
        game.zoomIn();
    if(_pKeyboardHandler->getInputState(InputKey::ZoomOut)) // zoom in
        game.zoomOut();

    pRigidbody->velocity = moveDir.normalized() * 2;

    if(_pKeyboardHandler->getInputState(InputKey::Sprint)) // sprint
        pRigidbody->velocity *= 2.5;

    Object::update(time);
}