#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

namespace vkapp
{

enum EngineError : int
{
    no_error                           = 0,
    glfw_init_failed                   = 1,
    vk_invalid_layer                   = 2,
    vk_instance_creation_failed        = 3,
    vk_debug_messenger_creation_failed = 4,
    vk_failed_to_find_physical_device  = 5,
    vk_failed_to_find_queue            = 6,
    vk_failed_to_create_logical_device = 7,
};

class Engine
{
  public:
    static std::vector<const char*> _vk_layers_names;

    void run();

  private:
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        bool                    is_complete();
        void                    log_unfound_queue();
    };

    void               init_window();
    void               init_vulkan();
    void               main_loop();
    void               cleanup();
    void               fetch_all_queues(QueueFamilyIndices& indices);
    VkResult           pick_up_physical_device();
    uint32_t           get_device_score(VkPhysicalDevice& phd);
    QueueFamilyIndices find_queue_families();
    VkResult           setup_debug_callback();
    VkResult           clear_debug_callback();
    VkResult           create_logical_device();

    GLFWwindow*              _window;
    VkInstance               _instance;
    VkPhysicalDevice         _physical_device;
    VkDevice                 _logical_device;
    VkQueue                  _grahics_queue;
    VkDebugUtilsMessengerEXT _debug_messenger;

  public:
#if defined(ENABLE_LOGGER)
    static std::shared_ptr<spdlog::logger> _logger;
#endif
};

}   // namespace vkapp

#endif