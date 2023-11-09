#ifndef LEVEL_CHANGER_INCLUDE
#define LEVEL_CHANGER_INCLUDE

#include "InteractableObject.hpp"

enum class LevelChangeMode
{
    Next,
    Previous
};

class LevelChanger : public InteractableObject
{
private:
    LevelChangeMode _mode;

public:
    bool init(Vector2 pos, LevelChangeMode mode);
    bool interact() override;
};

#endif