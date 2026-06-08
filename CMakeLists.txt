add_library(EntityLib 
    source/EntityManager.cpp
    source/EntityFactory.cpp
)

target_include_directories(EntityLib PUBLIC source)

target_link_libraries(EntityLib 
    PUBLIC 
        ComponentLib
        ResourceManagersLib
        TypeLib
)


