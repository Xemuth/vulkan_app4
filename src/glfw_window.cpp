

#include "glfw_window.hpp"

namespace vkapp
{

#if defined(ENABLE_LOGGER)
std::shared_ptr<spdlog::logger> GlfwWindow::_logger = MasterLogger::get_logger("VulkanGraphic");
#endif

GlfwWindow::GlfwWindow(Graphic& graphic) : _graphic(graphic)
{
}

void GlfwWindow::init()
{
}
void GlfwWindow::run()
{
}
void GlfwWindow::clean()
{
}

}   // namespace vkapp