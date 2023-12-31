#include "../../include/game/Input/KeyboardHandler.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Spells/SpellManager.hpp"
#include"../../include/game/UI/UIManager.hpp"
#include "../../include/game/Level/LevelManager.hpp"
#include "../../include/game/Entity/ComponentHandler.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Objects/InteractableObject.hpp"

#include <iostream>

KeyboardHandler::KeyboardHandler() : _game(*Game::getInstance())
{
    _pKeyboardState = SDL_GetKeyboardState(NULL);

    _keybinds = {
        { InputKey::Up, SDL_SCANCODE_W },
        { InputKey::Down, SDL_SCANCODE_S },
        { InputKey::Right, SDL_SCANCODE_D },
        { InputKey::Left, SDL_SCANCODE_A },
        { InputKey::Sprint, SDL_SCANCODE_LSHIFT },
        { InputKey::Reset, SDL_SCANCODE_R },
        { InputKey::ZoomIn, SDL_SCANCODE_EQUALS },
        { InputKey::ZoomOut, SDL_SCANCODE_MINUS },
        { InputKey::Pause, SDL_SCANCODE_ESCAPE },
        { InputKey::ToggleSpellUI, SDL_SCANCODE_C },
        { InputKey::ToggleDebugUI, SDL_SCANCODE_B },
        { InputKey::ToggleShowColliders, SDL_SCANCODE_H },
        { InputKey::Interact, SDL_SCANCODE_E }
    };
}

KeyboardHandler::~KeyboardHandler()
{
    delete _pKeyboardState;
}

void KeyboardHandler::processInputs()
{
    SDL_Event event;
    while(_inputEvents.size())
    {
        event = _inputEvents.front();

        // on key released
        if(event.type == SDL_KEYUP)
            onKeyUp(event.key.keysym.scancode);

        // on key pressed
        if(event.type == SDL_KEYDOWN)
            onKeyDown(event.key.keysym.scancode);

        _keyDown[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);

        _inputEvents.pop();
    }
}

void KeyboardHandler::onKeyUp(SDL_Scancode scancode)
{
    // Toggle Spell UI
    if(scancode == _keybinds[InputKey::ToggleSpellUI])
    {
        _game.pUIManager->getSpellUI()->toggleEnabled();
    }

    // Toggle Debug UI
    if(scancode == _keybinds[InputKey::ToggleDebugUI])
    {
        _game.pUIManager->getDebugUI()->toggleEnabled();
    }

    // Reset game
    if(scancode == _keybinds[InputKey::Reset])
    {
        _game.reset();
    }

    // Toggle game pause
    if(scancode == _keybinds[InputKey::Pause])
    {
        auto spellUI = _game.pUIManager->getSpellUI();
        if(spellUI->isEnabled())
            spellUI->toggleEnabled();

        _game.togglePause();
    }

    // Interact
    if(scancode == _keybinds[InputKey::Interact])
    {
        InteractableObject *pInteractable = _game.pPlayer->getCurrInteractable();
        if(pInteractable)
            pInteractable->interact();
    }

    if(scancode == _keybinds[InputKey::ToggleShowColliders])
    {
        _game.pComponentHandler->toggleShowColliders();
    }
}

void KeyboardHandler::onKeyDown(SDL_Scancode scancode)
{
    
}

bool KeyboardHandler::isPressed(InputKey key)
{
    return _keyDown[_keybinds[key]];
}

void KeyboardHandler::pushEvent(SDL_Event event)
{
    _inputEvents.push(event);
}