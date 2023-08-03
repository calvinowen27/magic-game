#include "../include/game/Player.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/KeyboardHandler.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player(Vector2 pos) : Object("player", pos, Vector2(1, 1))
{
    _pKeyboardHandler = pGame->pKeyboardHandler;
    isStatic = false;
}

void Player::update(float time)
{
    Vector2 moveDir;

    if(_pKeyboardHandler->getInputState(InputKey::Right)) // move right
        moveDir.x += 1;
    if(_pKeyboardHandler->getInputState(InputKey::Left)) // move left
        moveDir.x -= 1;
    if(_pKeyboardHandler->getInputState(InputKey::Up)) // move up
        moveDir.y += 1;
    if(_pKeyboardHandler->getInputState(InputKey::Down)) // move down
        moveDir.y -= 1;

    if(_pKeyboardHandler->getInputState(InputKey::ZoomIn)) // zoom out
        pGame->zoomIn();
    if(_pKeyboardHandler->getInputState(InputKey::ZoomOut)) // zoom in
        pGame->zoomOut();

    velocity = moveDir.normalized() * 2;

    if(_pKeyboardHandler->getInputState(InputKey::Sprint)) // sprint
        velocity *= 2.5;

    Object::update(time);
}