#include "EntityFactory.h"

std::pair<Entity::Entity, Entity::Entity> EntityFactory::createRenderableEntityRecurse(const gfx::SceneNode& sceneNode)
{
    // Create new entityID
    Entity::Entity newEntity = m_entityManager->generateNewEntity();

    // Create mesh component
    gfx::RenderNode newRenderNode{.mesh = sceneNode.renderAssets.mesh, 
                                    .relativeTransform = sceneNode.relativeTransform};
    m_entityManager->addComponentData<gfx::RenderNode>(newEntity, std::move(newRenderNode));

    // Create material component
    gfx::Material nodeMaterial = m_materialManager->getElement(sceneNode.renderAssets.materialKey);
    m_entityManager->addComponentData<gfx::Material>(newEntity, std::move(nodeMaterial));


    Entity::Entity prevEntity = Entity::NullEntity;
    gfx::RenderHierarchy prevHierarchy{};
    Entity::Entity ownFirstChild = Entity::NullEntity;

    // Loop through children
    for (const auto& childNode : sceneNode.children)
    {
        auto [childEntity, childsFirstChild] = createRenderableEntityRecurse(childNode); 

        if (prevEntity == Entity::NullEntity)
        {
            ownFirstChild = childEntity;
        }
        else
        {
            prevHierarchy.nextSibling = childEntity;
            m_entityManager->addComponentData<gfx::RenderHierarchy>(prevEntity, std::move(prevHierarchy));
        }

        prevHierarchy = gfx::RenderHierarchy{};
        prevHierarchy.childEntity = childsFirstChild;
        prevEntity = childEntity;

    }

    if (prevEntity != Entity::NullEntity)
    {
        m_entityManager->addComponentData<gfx::RenderHierarchy>(prevEntity, std::move(prevHierarchy));
    }

    return {newEntity, ownFirstChild};
}

Entity::Entity EntityFactory::createRenderableEntity(const gfx::SceneNode& rootSceneNode)
{
    gfx::RenderHierarchy rootHierarchy{};

    auto [rootEntity, rootFirstChild] = createRenderableEntityRecurse(rootSceneNode);

    rootHierarchy.childEntity = rootFirstChild;
    
    m_entityManager->addComponentData<gfx::RenderHierarchy>(rootEntity, std::move(rootHierarchy));

    // Create transform component
    gfx::Transform newTransform{.position = {0.0f, 0.0f, 0.0f},
                                .rotation = {0.0f, 0.0f, 0.0f},
                                .scaleFactors = {1.0f, 1.0f, 1.0f}};

    m_entityManager->addComponentData<gfx::Transform>(rootEntity, std::move(newTransform)); // Just put it at origin for testing

    return rootEntity;
}

