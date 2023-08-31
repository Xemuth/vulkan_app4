#include "engine.hpp"
#include "glfw_window.hpp"
#include "vulkan_graphic.hpp"

#include <memory>

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

    auto vulkan = std::make_unique<VulkanGraphic>(vec, std::vector<const char*>{});
    auto window = std::make_unique<GlfwWindow>();

    Engine engine(std::move(vulkan), std::move(window));

    for (;;)
    {
        auto result = engine.run();
        if (result != Engine::no_error)
            break;
    }

    return 0;
}