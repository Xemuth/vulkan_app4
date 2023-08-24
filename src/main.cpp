#include "glfw_window.hpp"
#include "vulkan_graphic.hpp"

using namespace vkapp;

std::vector<const char*> get_glfw_vk_required_extension()
{
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> vec;
    for (int i = 0; i < glfwExtensionCount; i++)
        vec.push_back(glfwExtensions[i]);

    return vec;
}

int main()
{
    auto vec = get_glfw_vk_required_extension();

    VulkanGraphic v{vec, std::vector<const char*>{}};

    GlfwWindow window(v);

    window.init();
    window.run();
    window.clean();

    return 0;
}