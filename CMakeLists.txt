cmake_minimum_required(VERSION 3.5.0)
project(ASS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PROJECT_LINK_LIBS -lm -lGL -lGLEW -lSDL2 -lSDL2_mixer)

#-lBulletDynamics -lBulletCollision -lLinearMath -llua5.3

link_directories(/usr/local/lib)

#-Wfatal-errors -fno-exceptions
add_definitions(-g -O3 -std=c++14 -Wfatal-errors -Wall)

include_directories(src)
include_directories(src/Tower)
include_directories(src/bullet)
include_directories(src/bullet/LinearMath)

file(GLOB src0 "src/Tower/*.cpp")
file(GLOB src1 "src/Tower/*/*.cpp")
file(GLOB src2 "src/Tower/*/*/*.cpp")

add_executable(out
    ${src0}
    ${src1}
    ${src2}
)
target_link_libraries(out ${PROJECT_LINK_LIBS})
