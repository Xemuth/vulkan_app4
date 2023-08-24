#include "vulkan_graphic.hpp"

#if defined(ENABLE_LOGGER)
#define LOGGER(action) _logger->action
#else
#define LOGGER(action)
#endif

namespace vkapp
{

#if defined(ENABLE_LOGGER)
std::shared_ptr<spdlog::logger> VulkanGraphic::_logger = MasterLogger::get_logger("VulkanGraphic");
#endif

VulkanGraphic::VulkanGraphic(const std::vector<const char*>& extensions, const std::vector<const char*>& validation_layers) : _extensions(extensions), _layers(validation_layers)
{
    LOGGER(debug("Testing vulkan logger"));
}

void VulkanGraphic::init()
{
}

void VulkanGraphic::run()
{
}

void VulkanGraphic::clean()
{
}

};   // namespace vkapp