#ifndef GAME_INCLUDE
#define GAME_INCLUDE

#define RAD_TO_DEGS 57.2957795
#define PI 3.1415927
#define TWO_PI 6.2831853

#include <vector>
#include <map>
#include <mutex>
#include <queue>
#include <iostream>
#include <memory>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_image.h"
#include "../SDL2/SDL_ttf.h"

#include "Util/Vector2.hpp"

typedef void (*func_ptr)(void); // for passing function ptrs to ui buttons

class Object;
class UIElement;
class Player;
class UIManager;
class ObjectManager;
class ContentManager;
class KeyboardHandler;
class MouseHandler;
class Registry;
class ComponentHandler;
class TransformComponent;
class SpellManager;
class AnimationManager;
class LevelManager;

class Game
{
private:
    int _targetUPS = 60;  // physics updates
    int _targetFPS = 120; // aim for this many fps

    static Game *_pInstance;

    int _fps = 0, _ups = 0; // frames per sec, updates per sec

    std::mutex _mutex;

public:
    const int SPRITE_PPM = 16; // texture pixels per meter (determines scaling of sprite on screen by dimensions)

    ContentManager *pContentManager;
    KeyboardHandler *pKeyboardHandler;
    MouseHandler *pMouseHandler;
    UIManager *pUIManager;
    ObjectManager *pObjectManager;
    Registry *pRegistry;
    ComponentHandler *pComponentHandler;
    SpellManager *pSpellManager;
    AnimationManager *pAnimationManager;
    LevelManager *pLevelManager;

    std::shared_ptr<Player> pPlayer;

    int winWidth = 1280, winHeight = 960; // window dimensions, 1250x835 by default in windowed mode
    int screenWidth, screenHeight;
    int ppm = 64; // screen pixels per meter, 64 by default
    int maxPPM = 256;
    int minPPM = 64;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

    Vector2 cameraPos;
    bool running = true;
    bool paused = false;

    static Game *getInstance(); // get singleton instance
    Game();
    ~Game();
    int init();           // init window and manager class instances
    void start();         // start game
    void runPhysics();    // runs physics thread
    void pollEvents();    // polls input events for mouse and keyboard
    void frameUpdate();   // perform render cycle
    void physicsUpdate(); // perform physics cycle
    void draw();          // draw everything to screen
    void reset();         // reset game

    Vector2Int worldToPixel(const Vector2 &pos);   // converts a world position to a pixel position
    Vector2 pixelToWorld(const Vector2Int &pxPos); // converts a pixel position to a world position

    bool isTransformOnScreen(TransformComponent &transform); // returns true if transform is on screen, false otherwise

    void zoomIn();  // zooms camera in by zoomScale
    void zoomOut(); // zooms camera out by zoomScale

    static void togglePause();
    static void quit();

    inline int getFPS() { return _fps; }
    inline int getUPS() { return _ups; }
};

#endif