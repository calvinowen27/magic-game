#ifndef KEYBOARD_INCLUDE
#define KEYBOARD_INCLUDE

#include "../SDL2/SDL.h"
#include <queue>
#include <map>

enum class InputKey {
    Right,
    Left,
    Up,
    Down,
    ZoomIn,
    ZoomOut,
    Sprint,
    Close,
    Reset
};

class KeyboardHandler
{
    private:
        const Uint8 *_pKeyboardState;
        std::queue<SDL_Event> _inputEvents;
        std::map<InputKey, SDL_Scancode> _keybinds;
        std::map<SDL_Scancode, bool> _inputState;

    public:
        KeyboardHandler();
        ~KeyboardHandler();
        void processInputs();
        bool isPressed(SDL_Scancode scancode);
        bool isReleased(SDL_Scancode scancode);
        void pushEvent(SDL_Event event);
        bool getInputState(InputKey key);
};

#endif