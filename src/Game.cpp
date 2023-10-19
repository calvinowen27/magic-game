#define SDL_MAIN_HANDLED

#include "../include/game/Game.hpp"
#include "../include/game/UI/TextElement.hpp"
#include "../include/game/Object.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/Enemy.hpp"
#include "../include/game/Grass.hpp"
#include "../include/game/Wall.hpp"
#include "../include/game/UI/UIElement.hpp"
#include "../include/game/UI/UIManager.hpp"
#include "../include/game/Input/MouseHandler.hpp"
#include "../include/game/Input/KeyboardHandler.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/WorldManager.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/Spells/Spell.hpp"
#include "../include/game/Spells/RadialSpell.hpp"
#include "../include/game/ECS/Registry.hpp"
#include "../include/game/ECS/Components.hpp"
#include "../include/game/ECS/ComponentHandler.hpp"
#include "../include/game/Spells/SpellManager.hpp"
#include "../include/game/Animation/AnimationManager.hpp"

#include <thread>
#include <chrono>

Game *Game::_pInstance;

int main()
{
    Game *pGame = Game::getInstance();
    if (pGame->init())
    {
        return EXIT_FAILURE;
    }

    pGame->start();

    return 0;
}

Game::Game()
{
}

Game::~Game()
{
    delete pKeyboardHandler;
    delete pContentManager;
    delete pUIManager;
    delete pMouseHandler;
    delete pObjectManager;
    delete pRegistry;
    delete pComponentHandler;
    delete pSpellManager;
    delete pWorldManager;
    delete pAnimationManager;
}

Game *Game::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new Game();
    }

    return _pInstance;
}

int Game::init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cerr << "Error with SDL_image initialization: " << IMG_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    TTF_Init();

    float ddpi, hdpi, vdpi;
    SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi);
    winWidth *= hdpi / 96.f;
    winHeight *= vdpi / 96.f;
    ppm *= ddpi / 96.f;
    minPPM *= ddpi / 96.f;
    maxPPM *= ddpi / 96.f;
    zoomScale *= ddpi / 96.f;

    Uint64 windowFlags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

    pWindow = SDL_CreateWindow("magic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               winWidth, winHeight, windowFlags);

    // SDL_DisplayMode DM;
    // SDL_GetCurrentDisplayMode(0, &DM);
    // winWidth = DM.w;
    // winHeight = DM.h;

    // SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (pWindow == NULL)
    {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (pRenderer == NULL)
    {
        std::cerr << "Failed to create renderer " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    pRegistry = new Registry();
    pComponentHandler = new ComponentHandler();
    pKeyboardHandler = new KeyboardHandler();
    pMouseHandler = new MouseHandler();
    pContentManager = new ContentManager();
    pUIManager = UIManager::getInstance();
    pObjectManager = ObjectManager::getInstance();
    pWorldManager = WorldManager::getInstance();
    pSpellManager = new SpellManager();
    pAnimationManager = new AnimationManager();

    pContentManager->loadContent();
    pUIManager->init();
    pObjectManager->init();
    pAnimationManager->init();

    pWorldManager->loadWorld(); // last because player and enemies created here!

    return 0;
}

void Game::start()
{
    std::thread updateThread([this]
                             { runPhysics(); });

    while (_pInstance->running)
    {
        frameUpdate();
    }

    updateThread.join();

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::runPhysics()
{
    while (_pInstance->running)
    {
        physicsUpdate();
    }
}

void Game::pollEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        /* ------------- KEYBOARD INPUTS ------------------------ */
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            pKeyboardHandler->pushEvent(event);
        /* ------------------------------------------------------ */

        /* ------------ MOUSE INPUTS ---------------------------- */
        if (event.type == SDL_MOUSEWHEEL)
            pMouseHandler->mouseWheel(event.wheel.y);

        if (event.type == SDL_MOUSEBUTTONDOWN)
            pMouseHandler->onMouseButtonDown(event.button.button);

        if (event.type == SDL_MOUSEBUTTONUP)
            pMouseHandler->onMouseButtonUp(event.button.button);
        /* ------------------------------------------------------- */

        /* RESIZE WINDOW */
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            SDL_Window *win = SDL_GetWindowFromID(event.window.windowID);
            if (win == pWindow)
            {
                SDL_GetWindowSize(win, &winWidth, &winHeight);
            }
        }

        /* TEMP */
        // if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_F)
        //     pPlayer->pHealth->damage(1);

        // if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_G)
        //     pPlayer->pHealth->heal(1);
    }
}

void Game::frameUpdate()
{
    using namespace std::chrono;
    float frameTime = 1 / (float)TARGET_FPS;
    nanoseconds timeDiff((int)(frameTime * 1000000000));

    auto startTime = high_resolution_clock::now();

    pollEvents();

    pMouseHandler->update();

    pUIManager->update();

    draw();

    auto execTime = duration_cast<nanoseconds>(high_resolution_clock::now() - startTime);

    // sleep for any extra time we have in the update
    auto sleepTime = timeDiff - execTime;

    std::lock_guard<std::mutex> guard(_mutex);

    _fps = 1000000000 / (sleepTime.count() > 0 ? timeDiff.count() : execTime.count());

    if (sleepTime.count() > 0)
        std::this_thread::sleep_for(sleepTime);
}

void Game::physicsUpdate()
{
    using namespace std::chrono;
    float updateTime = 1 / (float)TARGET_UPS; // seconds

    nanoseconds timeDiff((int)(updateTime * 1000000000)); // convert updateTime to nanoseconds

    auto startTime = high_resolution_clock::now();

    pKeyboardHandler->processInputs();

    if (paused)
        return;

    pComponentHandler->update(updateTime);

    pObjectManager->update(updateTime);

    if (pPlayer && pPlayer->isAlive())
        cameraPos = pPlayer->getPos() + Vector2(pPlayer->getDims().x / 2, pPlayer->getDims().y / 2);

    auto execTime = duration_cast<nanoseconds>(high_resolution_clock::now() - startTime);

    // sleep for any extra time we have in the update
    auto sleepTime = timeDiff - execTime;

    std::lock_guard<std::mutex> guard(_mutex);

    _ups = 1000000000 / (sleepTime.count() > 0 ? timeDiff.count() : execTime.count());

    if (sleepTime.count() > 0)
        std::this_thread::sleep_for(sleepTime);
}

void Game::draw()
{
    SDL_RenderClear(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, 47, 120, 46, 255);

    pComponentHandler->draw(pRenderer);

    pUIManager->draw(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, 47, 120, 46, 255);

    SDL_RenderPresent(pRenderer);
}

void Game::reset()
{
    pObjectManager->killEntitiesOfType<Enemy>();
    pObjectManager->killEntitiesOfType<Grass>();
    pObjectManager->killEntitiesOfType<Wall>();
    pPlayer->kill();

    pWorldManager->reset();
    pWorldManager->loadWorld();
}

Vector2 Game::pixelToWorld(Vector2Int pxPos)
{
    pxPos -= Vector2Int(winWidth / 2, winHeight / 2);
    pxPos.y -= winHeight;
    pxPos /= ppm;
    return (Vector2)pxPos + cameraPos;
}

Vector2Int Game::worldToPixel(Vector2 pos)
{
    pos -= cameraPos;
    pos *= ppm;
    pos.y = winHeight - pos.y;
    return (Vector2Int)pos + Vector2Int(winWidth / 2, -winHeight / 2);
}

bool Game::objOnScreen(Object &obj)
{
    Vector2Int pxPos = obj.getPxPos();
    Vector2Int pxDims = obj.getPxDims();
    return pxPos.x <= winWidth && pxPos.x + pxDims.x >= 0 && pxPos.y <= winHeight && pxPos.y + pxDims.y >= 0;
}

bool Game::isTransformOnScreen(TransformComponent &transform)
{
    return transform.pxPos.x <= winWidth && transform.pxPos.x + transform.pxDims.x >= 0 &&
           transform.pxPos.y <= winHeight && transform.pxPos.y + transform.pxDims.y >= 0;
}

void Game::togglePause()
{
    auto game = Game::getInstance();
    game->paused = !game->paused;
    game->pUIManager->getPauseUI()->toggleEnabled();
}

void Game::quit()
{
    Game::getInstance()->running = false;
}