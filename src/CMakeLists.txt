include(GenerateExportHeader)

add_library(ecs SHARED
    "ecs/internal/component_manager.cpp"
    "ecs/internal/entity_manager.cpp"
    "ecs/internal/system_manager.cpp")

add_library(lib::ecs ALIAS ecs)

target_include_directories(ecs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

generate_export_header(ecs
    BASE_NAME        ECS
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/ecs/export.h
)

add_executable(ecs_tests
    ecs/tests/core_suitetests.cpp)

target_link_libraries(ecs_tests
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
)

add_executable(my_ecs_tests
    ecs/tests/core_autretest.cpp)

target_link_libraries(my_ecs_tests
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
)

add_library(menu SHARED
    Menu/Menu.cpp
)
add_library(game SHARED
    Game/Game.cpp
)

target_include_directories(menu PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_include_directories(game PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(menu 
    PUBLIC sfml::sfml
    game
)
target_link_libraries(game 
    PUBLIC sfml::sfml
    lib::ecs
)

set_target_properties(menu PROPERTIES DEFINE_SYMBOL MENU_WINDOW_EXPORTS)
generate_export_header(menu
    BASE_NAME        MENU
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/menu_export.h
)
set_target_properties(game PROPERTIES DEFINE_SYMBOL GAME_WINDOW_EXPORTS)
generate_export_header(game
    BASE_NAME        GAME
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/game_export.h
)

add_executable(AllSurvivor main.cpp)
target_link_libraries(AllSurvivor 
    PUBLIC sfml::sfml
    menu
    game
)