#include <iostream>
#include "EntityManager.h"

EntityManager::EntityManager()
: nextID(0U)
{
}

void EntityManager::resizeSparse(Entity entity)
{
    // Double the size of the sparse vector unless entityID is higher
    size_t newSize = std::max(static_cast<size_t>(entity + 1), static_cast<size_t>(sparse.size()) * 2);

    sparse.resize(newSize, maxEntityValue);
}


void EntityManager::printActiveEntityComponents(Entity entity)
{
    uint32_t totalComponents = 0U;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "| The entity with ID " << entity << " components:  |" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (auto& [id, pool] : componentPools)
    {
        if (pool->hasEntity(entity))
        {
            ++totalComponents;
            std::cout << id.name() << std::endl;
        }
    }

    std::cout << "------------------------------------------------" << std::endl;
}

void EntityManager::printActiveEntityIDs()
{
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "| Currently active entity IDs:            |" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (auto& id : activeIDs)
    {
        std::cout << " " <<  id << " ";
    }

    std::cout << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
}

Entity EntityManager::generateNewEntity()
{
    Entity newID = getNewID();

    activeIDs.push_back(newID);

    if (newID >= sparse.size())
    {
        resizeSparse(newID);
    }

    sparse[newID] = static_cast<Entity>(activeIDs.size() - 1);

    return newID;
}

Entity EntityManager::getNewID()
{
    if (recyclingBucket.empty())
    {
        return nextID++;
    }

    Entity newID = recyclingBucket.back();
    recyclingBucket.pop_back();

    return newID;
}

void EntityManager::deleteEntity(Entity entity)
{
    Entity position = sparse[entity]; // get position
    Entity backValue = activeIDs.back(); // get back value

    activeIDs[position] = backValue; // Move back data into position of deleted data
    sparse[backValue] = position;

    activeIDs.pop_back(); // remove from back ID

    sparse[entity] = maxEntityValue;

    recyclingBucket.push_back(entity); 

    // Delete it in all pools
    for (auto& [key, componentPool] : componentPools)
    {
        componentPool->destroyEntity(entity);
    }
}