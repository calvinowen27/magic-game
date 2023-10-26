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

#include "Vector2.hpp"

typedef void (*func_ptr)(void);

class Object;
class UIElement;
class Player;
class UIManager;
class ObjectManager;
class ContentManager;
class KeyboardHandler;
class MouseHandler;
class WorldManager;
class Registry;
class ComponentHandler;
class TransformComponent;
class SpellManager;
class AnimationManager;

class Game
{
private:
    const int TARGET_UPS = 60;  // physics updates
    const int TARGET_FPS = 120; // aim for this many fps

    static Game *_pInstance;

    int _fps = 0, _ups = 0; // frames per sec, updates per sec

    std::mutex _mutex;

public:
    const int SPRITE_PPM = 16; // texture pixels per meter

    ContentManager *pContentManager;
    KeyboardHandler *pKeyboardHandler;
    MouseHandler *pMouseHandler;
    UIManager *pUIManager;
    WorldManager *pWorldManager;
    ObjectManager *pObjectManager;
    Registry *pRegistry;
    ComponentHandler *pComponentHandler;
    SpellManager *pSpellManager;
    AnimationManager *pAnimationManager;

    std::shared_ptr<Player> pPlayer;

    int winWidth = 1250, winHeight = 835;
    int ppm = 64; // screen pixels per meter, 64 by default
    int maxPPM = 512;
    int minPPM = 8;
    float zoomScale = 1.125;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

    Vector2 cameraPos;
    bool running = true;
    bool paused = false;

    static Game *getInstance();
    Game();
    ~Game();
    int init();
    void start();
    void runPhysics();
    void pollEvents();
    void frameUpdate();
    void physicsUpdate();
    void draw();
    void reset();

    Vector2 pixelToWorld(Vector2Int pxPos);
    Vector2Int worldToPixel(Vector2 pos);

    bool objOnScreen(Object &obj);
    bool isTransformOnScreen(TransformComponent &transform);

    inline int getFPS() { return _fps; }
    inline int getUPS() { return _ups; }
    inline void zoomIn()
    {
        ppm *= ppm * zoomScale <= maxPPM ? zoomScale : 1;
        if (ppm > maxPPM)
            ppm = maxPPM;
    }
    inline void zoomOut()
    {
        ppm /= ppm / zoomScale >= minPPM ? zoomScale : 1;
        if (ppm < minPPM)
            ppm = minPPM;
    }
    static void togglePause();
    static void quit();
};

#endif