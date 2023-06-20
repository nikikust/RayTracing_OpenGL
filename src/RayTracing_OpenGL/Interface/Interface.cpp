#include <RayTracing_OpenGL/Interface/Interface.h>


Interface::Interface(DataStorage& data_storage)
    : data_storage_(data_storage)
{
    init();
}
Interface::~Interface()
{
    shutdown();
}


void Interface::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --- //

    ImGui::Begin("Information");

    ImGui::Text("Screen resolution: %ix%i", data_storage_.screen_size.x, data_storage_.screen_size.y);
    ImGui::Separator();
    ImGui::Text("Camera X: %.1f", data_storage_.camera.origin.x);
    ImGui::Text("Camera Y: %.1f", data_storage_.camera.origin.y);
    ImGui::Text("Camera Z: %.1f", data_storage_.camera.origin.z);
    ImGui::Separator();
    ImGui::Text("Camera U: %.2f", data_storage_.camera.angles.x);
    ImGui::Text("Camera V: %.2f", data_storage_.camera.angles.y);
    ImGui::Separator();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::End();
}
void Interface::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Interface::init()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}
void Interface::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
