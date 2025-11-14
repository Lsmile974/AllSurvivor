add_executable(AllSurvivor main.cpp "Menu/Menu.h" "Menu/Menu.cpp" "Game/Game.h" "Game/Game.cpp")
target_link_libraries(AllSurvivor PUBLIC sfml::sfml
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)