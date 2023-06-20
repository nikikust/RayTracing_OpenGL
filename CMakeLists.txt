cmake_minimum_required(VERSION 3.5)
project(RayTracing_OpenGL LANGUAGES CXX)


set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)


file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY misc DESTINATION ${CMAKE_BINARY_DIR})


if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


include_directories(include ${CMAKE_BINARY_DIR}/_deps/imgui-src ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends ${CMAKE_BINARY_DIR}/_deps/imgui-src/misc/cpp/ ${CMAKE_BINARY_DIR}/_deps/glm-src)


add_subdirectory(third_party)


find_package(Threads REQUIRED)


add_library(gears STATIC
    src/RayTracing_OpenGL/gears/Functions.cpp
)
target_link_libraries(gears
    imgui
    stb
    glfw
    glad
    glm
)

add_library(data_storage STATIC
    src/RayTracing_OpenGL/DataStorage/DataStorage.cpp>
)
target_link_libraries(data_storage
    gears
)


add_library(window STATIC
    src/RayTracing_OpenGL/Window/Window.cpp
)
target_link_libraries(window
    gears
    data_storage
)


add_library(renderer STATIC
    src/RayTracing_OpenGL/Renderer/Renderer.cpp>
    src/RayTracing_OpenGL/Renderer/Tracer/Tracer.cpp
    src/RayTracing_OpenGL/Renderer/Tracer/Objects.cpp
)
target_link_libraries(renderer
    gears
    data_storage
)


add_library(interface STATIC
    src/RayTracing_OpenGL/Interface/Interface.cpp
)
target_link_libraries(interface
    gears
    data_storage
)


add_library(Core STATIC
    src/RayTracing_OpenGL/WindowStorage/WindowStorage.cpp

)
target_link_libraries(Core
    gears
    window
    renderer
    interface
    data_storage
)


add_executable(${PROJECT_NAME}
    src/RayTracing_OpenGL/Main.cpp
)
target_link_libraries(${PROJECT_NAME}
    Core
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
