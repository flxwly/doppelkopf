cmake_minimum_required(VERSION 3.20)
project(doppelkopf)

set(CMAKE_CXX_STANDARD 23)
set(SFML_DIR "C:\\GCC_Compiler\\SFML-2.5.1x32\\lib\\cmake\\SFML")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(doppelkopf main.cpp GameWindow.cpp GameWindow.hpp Player.cpp Player.hpp Card.cpp Card.hpp WindowSettings.hpp AI.cpp AI.hpp)

target_link_libraries(doppelkopf sfml-graphics)
