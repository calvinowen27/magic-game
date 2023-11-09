#include "../../include/game/Objects/LevelChanger.hpp"
#include "../../include/game/Level/LevelManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Entity/Player.hpp"

bool LevelChanger::init(Vector2 pos, LevelChangeMode mode)
{
    InteractableObject::init(EntityType::LevelChanger, pos);

    _mode = mode;

    if(_mode == LevelChangeMode::Previous)
        pRenderer->sourceRect = SDL_Rect{0, 0, 16, 16};
    else if(_mode == LevelChangeMode::Next)
        pRenderer->sourceRect = SDL_Rect{16, 0, 16, 16};

    if(pRigidbody)
        pRigidbody->isStatic = true;

    if(pRenderer)
        pRenderer->renderOrder = 1;
}

bool LevelChanger::interact()
{
    InteractableObject::interact();

    if(_mode == LevelChangeMode::Previous)
        levelManager.prevLevel();
    else if(_mode == LevelChangeMode::Next)
        levelManager.nextLevel();

    game.pPlayer->setCurrInteractable(nullptr);

    return true;
}