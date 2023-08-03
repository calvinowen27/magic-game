#include "../include/game/KeyboardHandler.hpp"
#include <iostream>

KeyboardHandler::KeyboardHandler()
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
        { InputKey::Close, SDL_SCANCODE_ESCAPE }
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

        _inputState[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);

        _inputEvents.pop();
    }
}

bool KeyboardHandler::isPressed(SDL_Scancode scancode)
{
    return _pKeyboardState[scancode];
}

bool KeyboardHandler::isReleased(SDL_Scancode scancode)
{
    return !_pKeyboardState[scancode];
}

void KeyboardHandler::pushEvent(SDL_Event event)
{
    _inputEvents.push(event);
}

bool KeyboardHandler::getInputState(InputKey key)
{
    return _inputState[_keybinds[key]];
}