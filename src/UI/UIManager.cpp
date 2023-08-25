#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/Button.hpp"

#include <cmath>

UIManager* UIManager::_pInstance;

UIManager::UIManager() : _game(*Game::getInstance())
{
    // _pGame = Game::getInstance();
    // _pPlayer = _pGame->pPlayer;
}

UIManager::~UIManager()
{
    for(UIElement *el : _uiElements)
    {
        delete el;
    }

    delete _pFPScounter;
    delete _pUPScounter;
    delete _pPPMDisplay;
    delete _pPosDisplay;
    delete _pVelDisplay;
}

UIManager* UIManager::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new UIManager();
    }

    return _pInstance;
}

void UIManager::init()
{
    _pFPScounter = new TextElement("ui_text_back.png", "FPS: ", "arial.ttf", WHITE, Vector2::zero, Vector2(0.1, 0.025));
    _pUPScounter = new TextElement("ui_text_back.png", "UPS: ", "arial.ttf", WHITE, Vector2(0, 0.026), Vector2(0.1, 0.025));
    _pPPMDisplay = new TextElement("ui_text_back.png", "PPM: ", "arial.ttf", WHITE, Vector2(0, 0.052), Vector2(0.1, 0.025));
    _pPosDisplay = new TextElement("ui_text_back.png", "Position: ", "arial.ttf", WHITE, Vector2(0.7, 0), Vector2(0.3, 0.025));
    _pVelDisplay = new TextElement("ui_text_back.png", "Velocity: ", "arial.ttf", WHITE, Vector2(0.7, 0.026), Vector2(0.3, 0.025));
}

void UIManager::update()
{
    _pFPScounter->setText("FPS: " + std::to_string(_game.getFPS()));
    _pUPScounter->setText("UPS: " + std::to_string(_game.getUPS()));
    _pPPMDisplay->setText("PPM: " + std::to_string(_game.ppm));
    _pPosDisplay->setText("Position: " + _game.pPlayer->getPos().round(3).to_string());
    _pVelDisplay->setText("Velocity: " + _game.pPlayer->getVelocity().round(3).to_string());

    // el.update()
    for(UIElement *el : _uiElements)
    {
        el->update();
    }
}

void UIManager::draw(SDL_Renderer *pRenderer)
{
    // el.draw();
    for(UIElement *el : _uiElements)
    {
        el->draw(pRenderer);
    }
}

void UIManager::addElement(UIElement *pEl)
{
    _uiElements.push_back(pEl);
}