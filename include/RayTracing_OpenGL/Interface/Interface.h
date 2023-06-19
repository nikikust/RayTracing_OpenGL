#pragma once
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Interface
{
public:
	Interface(DataStorage& data_storage);
	Interface() = delete;
	Interface(Interface&) = delete;
	Interface(Interface&&) = delete;

	// --- //

	void init();
	void shutdown();

	void update();
	void show();

private:
	DataStorage& data_storage_;

};
