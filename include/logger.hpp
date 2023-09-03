#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace vkapp
{
#if defined(ENABLE_LOGGER)
class MasterLogger
{
  public:
    static auto get_logger(const char* logger_name)
    {
        static std::shared_ptr<spdlog::logger> master_logger = nullptr;
        if (!master_logger)
        {
            std::vector<spdlog::sink_ptr> log_output;
            log_output.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
            log_output.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("vulkan_app4.txt"));
            master_logger = std::make_shared<spdlog::logger>("master_logger", begin(log_output), end(log_output));
            master_logger->set_level(spdlog::level::debug);
        }
        return master_logger->clone(logger_name);
    }
};
#endif
}   // namespace vkapp

#endif