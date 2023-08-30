#include "../../include/game/Animation/AnimationManager.hpp"
#include "../../include/game/Animation/Animation.hpp"

#include <fstream>

AnimationManager::AnimationManager()
{
}

void AnimationManager::init()
{
    std::ifstream f(_animationDataPath);
    _animationData = json::parse(f);
    f.close();
}