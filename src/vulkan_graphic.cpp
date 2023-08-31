#include "vulkan_graphic.hpp"

#include <array>

#if defined(ENABLE_LOGGER)
#define LOGGER(action) _logger->action
#else
#define LOGGER(action)
#endif

namespace
{
const std::array<const char*, 1> error_str = {"invalid_error"};

}   // namespace

namespace vkapp
{

#if defined(ENABLE_LOGGER)
std::shared_ptr<spdlog::logger> Graphic::_logger = MasterLogger::get_logger("VulkanGraphic");
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