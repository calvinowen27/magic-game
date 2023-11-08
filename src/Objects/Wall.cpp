#include "../../include/game/Objects/Wall.hpp"
#include "../../include/nlohmann_json/json.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"

bool Wall::init(Vector2 pos)
{
    Object::init(EntityType::Wall, pos);

    pRenderer->sourceRect = SDL_Rect{16, 0, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallTL::init(Vector2 pos)
{
    Object::init(EntityType::WallTL, pos);

    pRenderer->sourceRect = SDL_Rect{0, 0, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallT::init(Vector2 pos)
{
    Object::init(EntityType::WallT, pos);

    pRenderer->sourceRect = SDL_Rect{16, 0, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallTR::init(Vector2 pos)
{
    Object::init(EntityType::WallTR, pos);

    pRenderer->sourceRect = SDL_Rect{32, 0, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallL::init(Vector2 pos)
{
    Object::init(EntityType::WallL, pos);

    pRenderer->sourceRect = SDL_Rect{0, 32, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallR::init(Vector2 pos)
{
    Object::init(EntityType::WallR, pos);

    pRenderer->sourceRect = SDL_Rect{32, 32, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallBL::init(Vector2 pos)
{
    Object::init(EntityType::WallBL, pos);

    pRenderer->sourceRect = SDL_Rect{0, 48, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallB::init(Vector2 pos)
{
    Object::init(EntityType::WallB, pos);

    pRenderer->sourceRect = SDL_Rect{16, 48, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}

bool WallBR::init(Vector2 pos)
{
    Object::init(EntityType::WallBR, pos);

    pRenderer->sourceRect = SDL_Rect{32, 48, pRenderer->spriteDims.x, pRenderer->spriteDims.y};

    return true;
}