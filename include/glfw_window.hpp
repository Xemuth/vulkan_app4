#ifndef GLFW_WINDOW_HEADER
#define GLFW_WINDOW_HEADER
#include "logger.hpp"
#include "window.hpp"

#include <glfw/glfw3.h>

namespace vkapp
{

class GlfwWindow : public Window
{
    GLFWwindow* _window;

  public:
    GlfwWindow();
    virtual Window::Error render() override;
    virtual const char*   get_error_str(GlfwWindow::Error error) override;
};

}   // namespace vkapp

#endif