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

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_image.h"
#include "../SDL2/SDL_ttf.h"
#include "Vector2.hpp"

class Object;
class UIElement;
class Player;
class UIManager;
class ObjectManager;
class ContentManager;
class KeyboardHandler;
class MouseHandler;
class WorldManager;
class SpellManager;
class Registry;
class ColliderHandler;

class Game
{
private:
    const int TARGET_UPS = 60; // physics updates
    const int TARGET_FPS = 120; // aim for this many fps

    static Game* _pInstance;

    int _fps = 0, _ups = 0; // frames per sec, updates per sec

    std::mutex _mutex;

public:
    ContentManager *pContentManager;
    KeyboardHandler *pKeyboardHandler;
    MouseHandler *pMouseHandler;
    UIManager *pUIManager;
    WorldManager *pWorldManager;
    ObjectManager *pObjectManager;
    SpellManager *pSpellManager;
    Registry *pRegistry;
    ColliderHandler *pColliderHandler;

    Player *pPlayer;

    int winWidth = 1500, winHeight = 1000;
    int ppm = 64; // pixels per meter, 128 by default
    int maxPPM = 128;
    int minPPM = 32;
    int zoomStep = 8;
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;

    Vector2 cameraPos;
    bool running = true;

    static Game* getInstance();
    Game();
    ~Game();
    int gameInit();
    void start();
    void runPhysics();
    void pollEvents();
    void frameUpdate();
    void physicsUpdate();
    void draw();

    Vector2 pixelToWorld(Vector2Int pxPos);
    Vector2Int worldToPixel(Vector2 pos);

    bool objOnScreen(Object &obj);

    friend bool operator==(const Game &a, const Game &b);

    inline int getFPS() { return _fps; }
    inline int getUPS() { return _ups; }
    inline void zoomIn() { ppm += ppm < maxPPM ? zoomStep : 0; }
    inline void zoomOut() { ppm -= ppm > minPPM ? zoomStep : 0; }
};

#endif