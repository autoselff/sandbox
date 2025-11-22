cmake_minimum_required(VERSION 3.15)
project(sandbox C)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.c
)

target_link_libraries(${PROJECT_NAME} raylib)

# download dependencies
# sudo apt-get update
# sudo apt-get install build-essential cmake libx11-dev libxinerama-dev libxcursor-dev libxrandr-dev libxi-dev libglu1-mesa-dev

# compile raylib
# git clone https://github.com/raysan5/raylib.git
# cd raylib
# mkdir build
# cd build
# cmake ..
# make
# sudo make install
