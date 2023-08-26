#include "../include/game/Player.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/KeyboardHandler.hpp"
#include "../include/game/ECS/Registry.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player() : Object()
{
    _pKeyboardHandler = game.pKeyboardHandler;
}

bool Player::init(EntityType entityType, Vector2 pos)
{
    Object::init(entityType, pos);

    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
    pHealth->setEntity(this);
}

void Player::update(float time)
{
    if(!alive)
        return;

    Vector2 moveDir;

    if(_pKeyboardHandler->isPressed(InputKey::Right)) // move right
        moveDir.x += 1;
    if(_pKeyboardHandler->isPressed(InputKey::Left)) // move left
        moveDir.x -= 1;
    if(_pKeyboardHandler->isPressed(InputKey::Up)) // move up
        moveDir.y += 0.5f;
    if(_pKeyboardHandler->isPressed(InputKey::Down)) // move down
        moveDir.y -= 0.5f;

    if(_pKeyboardHandler->isPressed(InputKey::ZoomIn)) // zoom in
        game.zoomIn();
    if(_pKeyboardHandler->isPressed(InputKey::ZoomOut)) // zoom out
        game.zoomOut();

    if(moveDir.x != 0)
        pRigidbody->velocity = moveDir.normalized() * 2;
    else
        pRigidbody->velocity = moveDir * 2;

    if(_pKeyboardHandler->isPressed(InputKey::Sprint)) // sprint
        pRigidbody->velocity *= 2.5;

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Object::update(time);
}

void Player::kill()
{
    Object::kill();

    registry.killComponent(pHealth);
}