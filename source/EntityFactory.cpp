#include "EntityFactory.h"


Entity::Entity EntityFactory::createRenderableEntity(const gfx::SceneNode& sceneNode)
{
    bool firstChildDone = false;

    // Create new entityID
    Entity::Entity newEntity = m_entityManager->generateNewEntity();

    // Create mesh component
    gfx::RenderNode newRenderNode{.mesh = sceneNode.renderAssets.mesh, 
                                    .relativeTransform = sceneNode.relativeTransform};

    m_entityManager->addComponentData<gfx::RenderNode>(newEntity, std::move(newRenderNode));

    // Create transform component
    gfx::Transform newTransform{.position = {0.0f, 0.0f, 0.0f},
                                .rotation = {0.0f, 0.0f, 0.0f},
                                .scaleFactors = {1.0f, 1.0f, 1.0f}};

    m_entityManager->addComponentData<gfx::Transform>(newEntity, std::move(newTransform)); // Just put it at origin for testing

    // Create component
    Entity::Entity parentEntity = newEntity;
    Entity::Entity childEntity;

    gfx::RenderHierarchy childHierarchy = gfx::RenderHierarchy{};

    // Loop through children
    for (const auto& sceneNode : sceneNode.children)
    {
        if (!firstChildDone )
        {
            childEntity = createRenderableEntity(sceneNode);
 
            childHierarchy.childEntity = childEntity;

            m_entityManager->addComponentData<gfx::RenderHierarchy>(parentEntity, std::move(childHierarchy));

            firstChildDone = true;
        }
        else
        {
            parentEntity = childEntity; // Previous child is parent for the sibling

            childEntity = createRenderableEntity(sceneNode);

            childHierarchy.nextSibling = childEntity;

            m_entityManager->addComponentData<gfx::RenderHierarchy>(parentEntity, std::move(childHierarchy));
        }

        childHierarchy = gfx::RenderHierarchy{};
    }

    return newEntity;
}