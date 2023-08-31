#ifndef VULKAN_GRAPHIC_HEADER
#define VULKAN_GRAPHIC_HEADER

#include "graphic.hpp"
#include "logger.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace vkapp
{
class VulkanGraphic : public Graphic
{
    VulkanGraphic(const VulkanGraphic& v)            = delete;
    VulkanGraphic(VulkanGraphic&& v)                 = delete;
    VulkanGraphic& operator=(const VulkanGraphic& v) = delete;
    VulkanGraphic& operator=(VulkanGraphic&& v)      = delete;

    void init();
    void clean();

    VkInstance               _instance        = VK_NULL_HANDLE;
    VkPhysicalDevice         _physical_device = VK_NULL_HANDLE;
    VkDevice                 _device          = VK_NULL_HANDLE;
    std::vector<const char*> _extensions;
    std::vector<const char*> _layers;

  public:
    VulkanGraphic(const std::vector<const char*>& extensions, const std::vector<const char*>& validation_layers);

    void run() override;
};

}   // namespace vkapp

#endif