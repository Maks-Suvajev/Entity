#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

// STL
#include <stdint.h>
#include <vector> 
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include "ComponentManager.h"

#include "Entity.h"

class EntityManager
{
    public:
        EntityManager();
        void deleteEntity(Entity::Entity entity);
        Entity::Entity generateNewEntity();

        void printActiveEntityIDs();
        void printActiveEntityComponents(Entity::Entity entity);

        template<typename T>
        bool addComponentData(Entity::Entity entity, T&& componentData);

        template<typename T>
        T* getPoolElement(Entity::Entity entity);

        template<typename T>
        ComponentManager<T>* getComponentPool();

        size_t numActiveIDs()
        {
            return activeIDs.size();
        }

        std::vector<Entity::Entity> getActiveEntityIDs()
        {
            return activeIDs;
        }

        size_t componentsSize()
        {
            size_t totalComponents = 0;

            for (auto& [type, pool] : componentPools)
            {
                totalComponents += pool->size();
            }

            return totalComponents;
        }

        std::vector<std::type_index> getEntityComponentTypes(Entity::Entity entity)
        {
            std::vector<std::type_index> componentTypes;

            for (auto& [type, pool] : componentPools)
            {
                if (pool->hasEntity(entity))
                {
                    componentTypes.push_back(type);
                }
            }

            return componentTypes;
        }

        int getEntityComponentCount(Entity::Entity entity)
        {
            int totalCount = 0;

            for (auto& [type, pool] : componentPools)
            {
                if (pool->hasEntity(entity))
                {
                    ++totalCount;
                }
            }

            return totalCount;
        }


    private:
        Entity::Entity getNewID();
        void resizeSparse(Entity::Entity entity);

        uint32_t nextID;
        std::vector<Entity::Entity> recyclingBucket;
        std::vector<Entity::Entity> activeIDs;
        std::vector<Entity::Entity> sparse;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;
};

template<typename T>
bool EntityManager::addComponentData(Entity::Entity entity, T&& componentData)
{
    auto index = std::type_index(typeid(T));

    if (!componentPools.contains(index))
    {
        componentPools[index] = std::make_unique<ComponentManager<T>>(); 
    }

    return static_cast<ComponentManager<T>*>(componentPools[index].get())->addComponent(entity, std::move(componentData));
}

template<typename T>
T* EntityManager::getPoolElement(Entity::Entity entity)
{
    auto pool = getComponentPool<T>();

    if (pool == nullptr)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::RenderSystem::getPoolElement::No pool for type found: " << typeid(T).name() << std::endl;
        #endif
    }

    auto component = pool->getComponentData(entity);

    if (component == nullptr)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::RenderSystem::getPoolElement::Entity component has no value for some reason." << std::endl;
        #endif

        return nullptr;        
    }
    
    return component;
}

template<typename T>
ComponentManager<T>* EntityManager::getComponentPool()
{
    auto index = std::type_index(typeid(T));

    //TODO: Use find instead of contains, currently doing 2 hash lookups instead of just pulling an iterator once and checking if its at the end

    if (!componentPools.contains(index))
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::EntityManager::Component requested doesn't exist and has no pool." << std::endl;
        #endif

        return nullptr;
    }

    return static_cast<ComponentManager<T>*>(componentPools[index].get());
}


#endif