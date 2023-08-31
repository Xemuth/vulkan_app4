#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <cstdint>

namespace vkapp
{

class Window
{
  public:
    using Error                   = unsigned int;
    const unsigned int   no_error = 0;
    static const uint8_t ERROR_ID = 1;

    virtual const char* get_error_str(Error code) = 0;
    virtual Error       render()                  = 0;

  protected:
#if defined(ENABLE_LOGGER)
    static std::shared_ptr<spdlog::logger> _logger;
#endif
};

}   // namespace vkapp

#endif