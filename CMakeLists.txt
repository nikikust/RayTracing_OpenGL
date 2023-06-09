cmake_minimum_required(VERSION 3.5)
project(RayTracing_SFML_OpenGL LANGUAGES CXX)


set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)


include_directories(include)


find_package(Threads REQUIRED)

find_package(imgui CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)


add_library(functions STATIC
    src/RayTracing_SFML_OpenGL/Functions.cpp
)


add_executable(${PROJECT_NAME}
    src/RayTracing_SFML_OpenGL/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    functions
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
