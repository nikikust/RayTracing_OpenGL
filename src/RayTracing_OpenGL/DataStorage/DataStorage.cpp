#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


DataStorage::DataStorage()
{
    lastTime = glfwGetTime();
    deltaTime = 0;
}
