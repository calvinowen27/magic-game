#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <vector>
#include <string>

#include "../../SDL2/SDL.h"

class AnimationManager;

class Animation
{
private:
    AnimationManager &_animationManager;

    float _duration; // seconds
    float _frameTime; // seconds, _duration / # of frames

    SDL_Texture *_pTexture;

    std::vector<SDL_Rect> _frameRects;

public:
    Animation(AnimationManager animationManager);
};

#endif