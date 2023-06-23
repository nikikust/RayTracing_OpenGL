#pragma once
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Interface
{
public:
    Interface(DataStorage& data_storage);
    Interface() = delete;
    Interface(Interface&) = delete;
    Interface(Interface&&) = delete;

    ~Interface();

    // --- //

    void update();
    void draw();

private:
    void init();
    void shutdown();

    DataStorage& data_storage_;

};
