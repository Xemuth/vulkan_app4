

#include "glfw_window.hpp"

#include <array>

namespace
{
const std::array<const char*, 1> error_str = {"invalid_error"};

}   // namespace

namespace vkapp
{

#if defined(ENABLE_LOGGER)
std::shared_ptr<spdlog::logger> Window::_logger = MasterLogger::get_logger("GLFWWindow");
#endif

GlfwWindow::GlfwWindow()
{
}

GlfwWindow::Error GlfwWindow::render()
{
}

const char* GlfwWindow::get_error_str(GlfwWindow::Error error)
{
    auto removed_class_id = error & 0x0FFF;
    if (removed_class_id >= error_str.size())
        return error_str[0];
    return error_str[removed_class_id]
}

}   // namespace vkapp