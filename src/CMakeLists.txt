include(GenerateExportHeader)

add_library(menu SHARED
    Menu/Menu.cpp
)

target_include_directories(menu PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(menu 
    PUBLIC sfml::sfml
)

set_target_properties(menu PROPERTIES DEFINE_SYMBOL MENU_WINDOW_EXPORTS)
generate_export_header(menu
    BASE_NAME        MENU
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/menu_export.h
)

add_executable(AllSurvivor main.cpp)
target_link_libraries(AllSurvivor 
    PUBLIC sfml::sfml
    menu
)