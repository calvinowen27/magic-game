#ifndef REGISTRY_INCLUDE
#define REGISTRY_INCLUDE

#include <vector>
#include <memory>
#include <map>
#include <type_traits>

#include "../../SDL2/SDL.h"

#include "Components.hpp"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;
using std::is_base_of;

template <typename T>
class TypeVector
{
private:
    static shared_ptr<TypeVector<T>> _pInstance;

    vector<shared_ptr<T>> _vector;

public:
    static shared_ptr<TypeVector<T>> getInstance();
    static vector<shared_ptr<T>> &getVector();
    static void push_back(const shared_ptr<T> ptr);
    static shared_ptr<T> back();
};

class Registry
{
public:
    Registry();
    ~Registry();
    void update(float time);
    void draw(SDL_Renderer *pRenderer);

    template <typename T>
    shared_ptr<T> newComponent();

    template <typename T>
    vector<shared_ptr<T>> &getComponents();
};

#include "Registry_impl.hpp"

#endif