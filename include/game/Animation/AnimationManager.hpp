#ifndef ANIMATION_MANAGER_INCLUDE
#define ANIMATION_MANAGER_INCLUDE

#include "../../nlohmann_json/json.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

using json = nlohmann::json;

class Animation;

class AnimationManager
{
private:
    std::map<std::string, std::shared_ptr<Animation>> _animations;

    std::string _animationDataPath = "./data/Animations.json";

    json _animationData;

public:
    AnimationManager();
    void init();
};

#endif