#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

// STL
#include <vector>

#include "LevelManager.h"

#include "Entity.h"

class LevelManager
{
    public:
        void registerRootEntity(Entity::Entity rootEntity);
        const std::vector<Entity::Entity>& getRootEntities() const;

    private:
        std::vector<Entity::Entity>  m_rootEntities;
};


#endif