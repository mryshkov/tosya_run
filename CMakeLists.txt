cmake_minimum_required(VERSION 3.15)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyGame src/main.cpp)

# Подключаем raylib
target_include_directories(MyGame PRIVATE "C:/raylib/include")
target_link_directories(MyGame PRIVATE "C:/raylib/lib")

target_link_libraries(MyGame
    raylib
    opengl32
    gdi32
    winmm
)

# Чтобы exe появлялся в корне проекта
set_target_properties(MyGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)