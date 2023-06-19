cmake_minimum_required(VERSION 3.5)
project(RayTracing_OpenGL LANGUAGES CXX)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY misc DESTINATION ${CMAKE_BINARY_DIR})

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


include_directories(include)


find_package(Threads REQUIRED)

find_package(imgui CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


add_library(gears STATIC
    src/RayTracing_OpenGL/gears/Functions.cpp
)
target_link_libraries(gears
    imgui::imgui
    glfw
    glad::glad
)


add_library(Core STATIC
    src/RayTracing_OpenGL/WindowStorage/WindowStorage.cpp
    src/RayTracing_OpenGL/Renderer/Tracer/Tracer.cpp
    src/RayTracing_OpenGL/Renderer/Tracer/Objects.cpp
)
target_link_libraries(Core
    gears
    imgui::imgui
    glfw
    glad::glad
)


add_executable(${PROJECT_NAME}
    src/RayTracing_OpenGL/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    Core
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
