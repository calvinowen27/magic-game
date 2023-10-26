#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include <string>
#include <memory>
#include <vector>
#include <set>

#include "../../SDL2/SDL.h"

#include "../Vector2.hpp"

class Game;
class ContentManager;
class TransformComponent;
class RigidbodyComponent;
class Registry;
class Object;
class Entity;
class ObjectManager;
class Animation;
class AnimationManager;

enum class EntityType;

class Component
{
protected:
    Game &game;
    Registry &registry;
    ObjectManager &objectManager;

    Entity *pEntity;

    bool enabled;

public:
    Component();
    bool init();
    virtual void kill();
    void setEntity(Entity *pEntity);
    void enable() { enabled = true; }
    void disable() { enabled = false; }
    bool isEnabled() { return enabled; }
};

class RendererComponent : public Component
{
public:
    ContentManager &contentManager;

    SDL_Texture *pTexture;
    SDL_Rect spriteRect;
    SDL_Rect sourceRect;

    Vector2Int spriteDims;

    std::shared_ptr<TransformComponent> pTransform;

    float spriteAngle = 0;  // sprite rotation angle, degrees
    bool isFlipped = false; // over y axis

    int renderOrder = 0;

    RendererComponent();
    RendererComponent *init(std::string textureName, std::shared_ptr<TransformComponent> pTransform, int renderOrder = 0); // returns true if successful
    RendererComponent *init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, int renderOrder = 0, bool startEnabled = false); // returns true if successful
    void update(float time);
    void draw(SDL_Renderer *pRenderer);
    void kill() override;
    bool setTexture(std::string textureName); // returns true if successful
    void refreshDimensions();
};

class TransformComponent : public Component
{
public:
    Vector2 pos;       // meters
    Vector2Int pxPos;  // pixels
    Vector2 dims;      // meters
    Vector2Int pxDims; // pixels

    TransformComponent();
    TransformComponent *init(Vector2 pos, Vector2 dims = Vector2(1, 1)); // returns true if successful
    void update(float time);

    void setDims(Vector2 newDims);        // set new dimensions
    void setPxDims(Vector2Int newPxDims); // set new px dims

    void updatePxPos();
};

class ColliderComponent : public Component
{
public:
    Vector2 start; // scalar of dims, relative to bottom left of object
    Vector2 end;   // scalar of dims, relative to top right of object

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<RigidbodyComponent> pRigidbody;

    // positions of each border of collider
    float leftX;
    float rightX;
    float topY;
    float bottomY;

    // colliders that are touching this on
    std::set<std::shared_ptr<ColliderComponent>> collidersTouching;

    int borderEnabled[4]{1, 1, 1, 1}; // left right bottom top

    bool doCollisions = true;
    bool isTrigger = false;

    ColliderComponent();
    ColliderComponent *init(Vector2 start, Vector2 end, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions = true, bool isTrigger = false); // returns true if successful
    void update(float time);
    void kill() override;
    void onCollisionEnter(std::shared_ptr<ColliderComponent> other);
    void onCollisionExit(std::shared_ptr<ColliderComponent> other);
    void whileTouching(std::shared_ptr<ColliderComponent> other);
    bool isTouching(std::shared_ptr<ColliderComponent> other);
};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false; // doesn't move if static

    Vector2 velocity; // meters/sec

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<ColliderComponent> pCollider;

    RigidbodyComponent();
    RigidbodyComponent *init(std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<ColliderComponent> pCollider, bool isStatic = false); // returns true if successful
    void update(float time);
    void kill() override;
};

class HealthComponent : public Component
{
public:
    float baseHealth;
    float health;

    std::shared_ptr<RendererComponent> pRedRenderer;
    std::shared_ptr<RendererComponent> pGreenRenderer;

    HealthComponent();
    HealthComponent *init(float baseHealth);
    void heal(float healAmount);
    bool damage(float dmgAmount); // returns true if dead after damage dealt
    void kill() override;
};

class AnimatorComponent : public Component
{
private:
    AnimationManager &_animationManager;

    std::shared_ptr<Animation> _pAnimation;

    std::shared_ptr<RendererComponent> _pRenderer;
    std::vector<SDL_Rect> _rects;

    int _currFrame;
    int _frameCount;
    float _frameTime;
    float _duration;
    float _timeSinceFrame;
    bool _loops;
    bool _playing;

    EntityType _animationType;
    std::string _animationName;

public:
    AnimatorComponent();
    void init(std::shared_ptr<RendererComponent> pRenderer);
    void update(float time);
    void setAnimation(EntityType entityType, std::string name);
    void kill();

    inline void startAnimation() { _playing = true; }
    inline void stopAnimation() { _playing = false; }
};

#endif