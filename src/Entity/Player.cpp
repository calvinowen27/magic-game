#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Input/KeyboardHandler.hpp"
#include "../../include/game/Entity/Registry.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player() : LiveEntity()
{
    _pKeyboardHandler = game.pKeyboardHandler;
}

bool Player::init(EntityType entityType, Vector2 pos)
{
    LiveEntity::init(entityType, pos);

    pHealth = registry.newComponent<HealthComponent>();
    pHealth->init(10);
    pHealth->setEntity(this);

    pAnimator = registry.newComponent<AnimatorComponent>();
    pAnimator->init(pRenderer);
    pAnimator->setAnimation(entityType, "idle_down");

    return true;
}

void Player::update(float time)
{
    if (!alive)
        return;

    std::string nextAnimation;

    _moveDir = Vector2::zero;

    if (_pKeyboardHandler->isPressed(InputKey::Right)) // move right
        _moveDir.x += 1;
    if (_pKeyboardHandler->isPressed(InputKey::Left)) // move left
        _moveDir.x -= 1;
    if (_pKeyboardHandler->isPressed(InputKey::Up)) // move up
        _moveDir.y += 1;
    if (_pKeyboardHandler->isPressed(InputKey::Down)) // move down
        _moveDir.y -= 1;

    if (_moveDir == Vector2::zero && _prevMoveDir != Vector2::zero)
    {
        nextAnimation = _prevMoveDir.x < 0 ? "idle_left" : "idle_right";

        if (_prevMoveDir.y != 0)
            nextAnimation = _prevMoveDir.y > 0 ? "idle_up" : "idle_down";
    }

    _prevMoveDir = _moveDir;

    if (_pKeyboardHandler->isPressed(InputKey::ZoomIn)) // zoom in
        game.zoomIn();
    if (_pKeyboardHandler->isPressed(InputKey::ZoomOut)) // zoom out
        game.zoomOut();

    if (_moveDir.x != 0)
        pRigidbody->velocity = _moveDir.normalized() * 2;
    else
        pRigidbody->velocity = _moveDir * 2;

    if (_pKeyboardHandler->isPressed(InputKey::Sprint)) // sprint
    {
        pRigidbody->velocity *= 2.5;

        if (_moveDir.y > 0)
        {
            nextAnimation = "run_up";
        }
        if (_moveDir.y < 0)
        {
            nextAnimation = "run_down";
        }
        if (_moveDir.x > 0)
        {
            nextAnimation = "run_right";
        }
        if (_moveDir.x < 0)
        {
            nextAnimation = "run_left";
        }
    }
    else
    {
        if (_moveDir.y > 0)
        {
            nextAnimation = "walk_up";
        }
        if (_moveDir.y < 0)
        {
            nextAnimation = "walk_down";
        }
        if (_moveDir.x > 0)
        {
            nextAnimation = "walk_right";
        }
        if (_moveDir.x < 0)
        {
            nextAnimation = "walk_left";
        }
    }

    if(nextAnimation != "")
        pAnimator->setAnimation(type, nextAnimation);

    pHealth->pGreenRenderer->pTransform->pos = pTransform->pos + Vector2(0, pTransform->dims.y + 0.5);
    pHealth->pRedRenderer->pTransform->pos = pHealth->pGreenRenderer->pTransform->pos;

    Entity::update(time);
}

void Player::kill()
{
    LiveEntity::kill();

    registry.killComponent(pHealth);
    registry.killComponent(pAnimator);
}