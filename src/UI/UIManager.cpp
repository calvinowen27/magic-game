#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/Button.hpp"

#include <cmath>

UIManager* UIManager::_pInstance;

UIManager::UIManager() : _game(*Game::getInstance())
{
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
    _pFPScounter = newUIElement<TextElement>();
    _pFPScounter->init("ui_text_back", "FPS: ", "arial", WHITE, Vector2::zero, Vector2(0.1, 0.025));

    _pUPScounter = newUIElement<TextElement>();
    _pUPScounter->init("ui_text_back", "UPS: ", "arial", WHITE, Vector2(0, 0.026), Vector2(0.1, 0.025));
    
    _pPPMDisplay = newUIElement<TextElement>();
    _pPPMDisplay->init("ui_text_back", "PPM: ", "arial", WHITE, Vector2(0, 0.052), Vector2(0.1, 0.025));
    
    _pPosDisplay = newUIElement<TextElement>();
    _pPosDisplay->init("ui_text_back", "Position: ", "arial", WHITE, Vector2(0.7, 0), Vector2(0.3, 0.025));
    
    _pVelDisplay = newUIElement<TextElement>();
    _pVelDisplay->init("ui_text_back", "Velocity: ", "arial", WHITE, Vector2(0.7, 0.026), Vector2(0.3, 0.025));
}

void UIManager::update()
{
    _pFPScounter->setText("FPS: " + std::to_string(_game.getFPS()));
    _pUPScounter->setText("UPS: " + std::to_string(_game.getUPS()));
    _pPPMDisplay->setText("PPM: " + std::to_string(_game.ppm));
    _pPosDisplay->setText("Position: " + _game.pPlayer->getPos().round(3).to_string());
    _pVelDisplay->setText("Velocity: " + _game.pPlayer->getVelocity().round(3).to_string());

    // el.update()
    for(auto el : _uiElements)
    {
        el->update();
    }
}

void UIManager::draw(SDL_Renderer *pRenderer)
{
    // el.draw();
    for(auto el : _uiElements)
    {
        el->draw(pRenderer);
    }
}