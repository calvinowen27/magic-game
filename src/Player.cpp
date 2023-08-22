#include "../include/game/Player.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/KeyboardHandler.hpp"
#include "../include/game/ECS/Registry.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player() : Object("Player")
{
    _pKeyboardHandler = game.pKeyboardHandler;
    
    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
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

    if(_pKeyboardHandler->getInputState(InputKey::ZoomIn)) // zoom in
        game.zoomIn();
    if(_pKeyboardHandler->getInputState(InputKey::ZoomOut)) // zoom out
        game.zoomOut();

    if(moveDir.x != 0)
        pRigidbody->velocity = moveDir.normalized() * 2;
    else
        pRigidbody->velocity = moveDir * 2;

    if(_pKeyboardHandler->getInputState(InputKey::Sprint)) // sprint
        pRigidbody->velocity *= 2.5;

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Object::update(time);
}