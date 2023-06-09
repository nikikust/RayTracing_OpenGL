cmake_minimum_required(VERSION 3.5)
project(RayTracing_SFML_OpenGL LANGUAGES CXX)


set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)


if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


include_directories(include)


find_package(Threads REQUIRED)

find_package(imgui CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)


add_library(gears STATIC
    src/RayTracing_SFML_OpenGL/gears/Functions.cpp
)
target_link_libraries(gears
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)


add_library(WindowStorage STATIC
    src/RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.cpp
)
target_link_libraries(WindowStorage
    gears
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)


add_executable(${PROJECT_NAME}
    src/RayTracing_SFML_OpenGL/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    WindowStorage
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
