#ifndef REGISTRY_INCLUDE
#define REGISTRY_INCLUDE

#include <vector>
#include <memory>
#include <map>
#include <type_traits>
#include <queue>
#include <set>

#include "../../SDL2/SDL.h"

#include "Components.hpp"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;
using std::is_base_of;
using std::queue;
using std::set;

template <typename T>
class TypeVector
{
private:
    static shared_ptr<TypeVector<T>> _pInstance; // singleton vector for each type

    vector<shared_ptr<T>> _vector;

public:
    static shared_ptr<TypeVector<T>> getInstance();
    vector<shared_ptr<T>> &getVector();

    // wrapper functions
    void push_back(const shared_ptr<T> ptr);
    shared_ptr<T> back();
};

template <typename T>
class TypeQueue
{
private:
    static shared_ptr<TypeQueue<T>> _pInstance; // singleton queue for each type

    queue<shared_ptr<T>> _queue;

public:
    static shared_ptr<TypeQueue<T>> getInstance();
    queue<shared_ptr<T>> &getQueue();

    // wrapper functions
    bool isEmpty();
    shared_ptr<T> pop();
    void push(shared_ptr<T>);
};

template <typename T>
class TypeSet
{
private:
    static shared_ptr<TypeSet<T>> _pInstance; // singleton set for each type

    set<shared_ptr<T>> _set;

public:
    static shared_ptr<TypeSet<T>> getInstance();
    set<shared_ptr<T>> &getSet();

    // wrapper functions
    void emplace(shared_ptr<T> component);
    size_t erase(shared_ptr<T> component);
};

template <typename T>
class TypePool
{
private:
    static shared_ptr<TypePool<T>> _pInstance;

    TypeQueue<T> _pool;
    TypeSet<T> _alive;

public:
    TypePool();
    TypePool(int size);
    static shared_ptr<TypePool> getInstance();
    shared_ptr<T> instantiate();
    bool release(shared_ptr<T> component);
    void expandPool(int amount);
    set<shared_ptr<T>> &getAlive();
    queue<shared_ptr<T>> &getPooled();
};

class Registry
{
public:
    template <typename T>
    shared_ptr<T> newComponent(); // create/add a new component to TypeVector<T>

    template <typename T>
    void killComponent(shared_ptr<T> component);

    template <typename T>
    set<shared_ptr<T>> &getComponents();
};

#include "Registry_impl.hpp"

#endif