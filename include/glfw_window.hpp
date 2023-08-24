#ifndef GLFW_WINDOW_HEADER
#define GLFW_WINDOW_HEADER
#include "graphic.hpp"
#include "logger.hpp"

#include <glfw/glfw3.h>

namespace vkapp
{

class GlfwWindow
{
    GLFWwindow* _window;
    Graphic&    _graphic;
#if defined(ENABLE_LOGGER)
    static std::shared_ptr<spdlog::logger> _logger;
#endif

  public:
    GlfwWindow(Graphic& graphic);

    void init();
    void run();
    void clean();
};

}   // namespace vkapp

#endif