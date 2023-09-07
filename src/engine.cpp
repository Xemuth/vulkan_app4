#include "engine.hpp"
#include "error.hpp"

#include <vulkan/vk_enum_string_helper.h>

#if defined(ENABLE_LOGGER)
#define LOGGER(action) _logger->action
#else
#define LOGGER(action)
#endif

namespace
{

VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data)
{
    static std::shared_ptr<spdlog::logger> logger = vkapp::MasterLogger::get_logger("Validation layer callback");
    switch (message_severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        logger->debug("{}", p_callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        logger->warn("{}", p_callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        logger->error("{}", p_callback_data->pMessage);
        break;
    };

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT populate_debug_messenger_create_info()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = vulkan_debug_callback;
    create_info.pUserData       = nullptr;   // Optional
    return create_info;
}

std::vector<const char*> get_required_extensions()
{
    uint32_t     glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
#ifdef ENABLE_VALIDATION_LAYER
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return extensions;
}

VkResult check_validation_layer_support(std::vector<const char*>& layers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layer_found = false;
    for (const char* layer : layers)
    {
        layer_found = false;
        for (const auto& available_layer : availableLayers)
        {

            if (strcmp(layer, available_layer.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }
        if (!layer_found)
        {
            vkapp::Engine::_logger->error("Vulkan layer not found {}", layer);
            return VkResult::VK_ERROR_LAYER_NOT_PRESENT;
        }
    }
    return VkResult::VK_SUCCESS;
}

VkResult create_vulkan_instance(VkInstance& instance)
{
    VkApplicationInfo app_info{};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "No Engine";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    auto vulkan_extensions              = get_required_extensions();
    create_info.enabledExtensionCount   = vulkan_extensions.size();
    create_info.ppEnabledExtensionNames = vulkan_extensions.data();
#if defined(ENABLE_VALIDATION_LAYER) && defined(ENABLE_LOGGER)
    create_info.enabledLayerCount   = vkapp::Engine::_vk_layers_names.size();
    create_info.ppEnabledLayerNames = vkapp::Engine::_vk_layers_names.data();
    auto create_info_messenger      = populate_debug_messenger_create_info();
    create_info.pNext               = (VkDebugUtilsMessengerCreateInfoEXT*) &create_info_messenger;
#else
    create_info.enabledLayerCount   = 0;
    create_info.ppEnabledLayerNames = nullptr;
#endif
    return vkCreateInstance(&create_info, nullptr, &instance);
}

}   // namespace

namespace vkapp
{

std::vector<const char*> Engine::_vk_layers_names = {};
#if defined(ENABLE_LOGGER)
std::shared_ptr<spdlog::logger> Engine::_logger = MasterLogger::get_logger("Engine");
#endif

void Engine::run()
{
    init_window();
    init_vulkan();
    main_loop();
    cleanup();
}
void Engine::init_window()
{
    LOGGER(debug("Create GLFW Window"));
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr);
    if (_window == nullptr)
    {
        LOGGER(error("Failed to create GLFW Window"));
        ErrorHandler::raise_error(EngineError::glfw_init_failed);
    }
}
void Engine::init_vulkan()
{
    if (ErrorHandler::is_app_error())
        return;
    LOGGER(debug("Create Vulkan instance"));
    VkResult result = VK_SUCCESS;
#if defined(ENABLE_VALIDATION_LAYER) && defined(ENABLE_LOGGER)
    LOGGER(debug("Vulkan validation layer enable"));
    result = check_validation_layer_support(_vk_layers_names);
    if (result != VK_SUCCESS)
    {
        ErrorHandler::raise_error(EngineError::vk_invalid_layer);
        return;
    }
#endif

    result = create_vulkan_instance(_instance);
    if (result != VK_SUCCESS)
    {
        LOGGER(error("Failed to create Vulkan instance: {}", string_VkResult(result)));
        ErrorHandler::raise_error(EngineError::vk_instance_creation_failed);
        return;
    }
#if defined(ENABLE_VALIDATION_LAYER) && defined(ENABLE_LOGGER)
    result = setup_debug_callback();
    if (result != VK_SUCCESS)
    {
        LOGGER(error("Failed to create Vulkan debug messenger: {}", string_VkResult(result)));
        ErrorHandler::raise_error(EngineError::vk_debug_messenger_creation_failed);
        return;
    }
#endif

    result = pick_up_physical_device();
    if (result == EngineError::vk_failed_to_find_physical_device)
    {
        ErrorHandler::raise_error(EngineError::vk_failed_to_find_physical_device);
        return;
    }
    if (result != VK_SUCCESS)
    {
        LOGGER(error("Failed to enumerate VkPhysicalDevice: {}", string_VkResult(result)));
        ErrorHandler::raise_error(EngineError::vk_failed_to_find_physical_device);
        return;
    }

    result = create_logical_device();
    if (result != VK_SUCCESS)
    {
        if (result == EngineError::vk_failed_to_find_queue)
            ErrorHandler::raise_error(EngineError::vk_failed_to_find_queue);
        else
        {
            ErrorHandler::raise_error(EngineError::vk_failed_to_create_logical_device);
            LOGGER(error("Failed to create logical device: {}", string_VkResult(result)));
        }
        return;
    }
}
VkResult Engine::create_logical_device()
{

    auto queues = find_queue_families();
    if (!queues.is_complete())
    {
        LOGGER(error("Failed to find all required queues"));
        queues.log_unfound_queue();
        return static_cast<VkResult>(EngineError::vk_failed_to_find_queue);
    }

    LOGGER(debug("Creating logical device"));
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queues.graphics_family.value();
    queue_create_info.queueCount       = 1;
    auto queue_priority                = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.enabledExtensionCount = 0;

#if defined(ENABLE_VALIDATION_LAYER)
    create_info.enabledLayerCount   = static_cast<uint32_t>(_vk_layers_names.size());
    create_info.ppEnabledLayerNames = _vk_layers_names.data();
#else
    create_info.enabledLayerCount   = 0;
#endif
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos    = &queue_create_info;

    auto result = vkCreateDevice(_physical_device, &create_info, nullptr, &_logical_device);
    if (result != VK_SUCCESS)
        return result;

    // Fetching all queue
    vkGetDeviceQueue(_logical_device, queues.graphics_family.value(), 0, &_grahics_queue);

    return result;
}
Engine::QueueFamilyIndices Engine::find_queue_families()
{
    LOGGER(debug("Looking up for all required queue"));
    Engine::QueueFamilyIndices indices;
    uint32_t                   queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_family_count, queue_families.data());
    int i = 0;
    for (uint32_t i = 0; i < queue_families.size(); i++)
    {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            LOGGER(debug("graphics_family queue found"));
            indices.graphics_family = i;
        }
        if (indices.is_complete())
        {
            LOGGER(debug("All required queue found"));
            break;
        }
    }
    return indices;
}
VkResult Engine::pick_up_physical_device()
{
    LOGGER(debug("Looking up for a physical device"));
    uint32_t device_count = 0;
    auto     result       = vkEnumeratePhysicalDevices(_instance, &device_count, nullptr);
    if (result != VK_SUCCESS)
        return result;
    if (device_count == 0)
        return static_cast<VkResult>(EngineError::vk_failed_to_find_physical_device);
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(_instance, &device_count, devices.data());

    uint32_t         best_score = 0;
    VkPhysicalDevice best_device;

    for (auto& phd : devices)
    {
        auto score = get_device_score(phd);
        if (score > best_score)
        {
            best_device = phd;
            best_score  = score;
        }
    }
    if (best_score == 0)
    {
        LOGGER(error("No suitable physical device available"));
        return static_cast<VkResult>(EngineError::vk_failed_to_find_physical_device);
    }
    _physical_device = best_device;
#if defined(ENABLE_LOGGER)
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(_physical_device, &device_properties);
    LOGGER(debug("{} selected", device_properties.deviceName));
#endif
    return VK_SUCCESS;
}
uint32_t Engine::get_device_score(VkPhysicalDevice& phd)
{
    uint32_t                   score = 0;
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(phd, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(phd, &device_features);

    // Discrete GPUs have a significant performance advantage
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += device_properties.limits.maxImageDimension2D;
    // Application can't function without geometry shaders
    if (!device_features.geometryShader)
    {
        LOGGER(debug("{}: {} score", device_properties.deviceName, 0));
        return 0;
    }
    LOGGER(debug("{}: {} score", device_properties.deviceName, score));
    return score;
}
VkResult Engine::setup_debug_callback()
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        auto create_info = populate_debug_messenger_create_info();
        return func(_instance, &create_info, nullptr, &_debug_messenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult Engine::clear_debug_callback()
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(_instance, _debug_messenger, nullptr);
        return VK_SUCCESS;
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Engine::main_loop()
{
    while (!glfwWindowShouldClose(_window) && !ErrorHandler::is_app_error())
    {
        glfwPollEvents();
    }
}
void Engine::cleanup()
{
    LOGGER(debug("Cleaning Engine..."));
    VkResult result = VK_SUCCESS;

    vkDestroyDevice(_logical_device, nullptr);
#if defined(ENABLE_VALIDATION_LAYER) && defined(ENABLE_LOGGER)
    result = clear_debug_callback();
    if (result != VK_SUCCESS)
    {
        LOGGER(error("Failed to clear Vulkan debug messenger: {}", string_VkResult(result)));
    }
#endif

    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}
bool Engine::QueueFamilyIndices::is_complete()
{
    return graphics_family.has_value();
}
void Engine::QueueFamilyIndices::log_unfound_queue()
{
    if (graphics_family.has_value())
        LOGGER(error("graphics_family queue not found"));
}
}   // namespace vkapp
