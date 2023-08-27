#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/TextButton.hpp"
#include "../../include/game/UI/Button.hpp"
#include "../../include/game/Input/KeyboardHandler.hpp"

#include <cmath>

UIManager *UIManager::_pInstance;

UIManager::UIManager() : _game(*Game::getInstance())
{
}

UIManager *UIManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new UIManager();
    }

    return _pInstance;
}

void UIManager::init()
{
    _pSpellUI = newUIGroup<SpellUIGroup>();
    _pSpellUI->init();

    _pDebugUI = newUIGroup<DebugUIGroup>();
    _pDebugUI->init();
}

void UIManager::update()
{
    // el.update()
    for (auto group : _uiGroups)
    {
        if (group->isEnabled())
            group->update();
    }
}

void UIManager::draw(SDL_Renderer *pRenderer)
{
    // el.draw();
    for (auto group : _uiGroups)
    {
        if (group->isEnabled())
            group->draw(pRenderer);
    }
}