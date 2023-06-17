include(FetchContent)


FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 530134d1253c97bd33268b5932b6c67d0ad47d09
)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/ValtoLibraries/GLAD
        GIT_TAG master
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG master
)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG master
)

message("Installing imgui")
FetchContent_MakeAvailable(imgui)
message("imgui installed")

message("Installing glfw")
FetchContent_MakeAvailable(glfw)
message("glfw installed")

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

message("Installing glad")
FetchContent_MakeAvailable(glad)
message("glad installed")

message("Installing glm")
FetchContent_MakeAvailable(glm)
message("glm installed")
