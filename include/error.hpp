#ifndef ERROR_HEADER
#define ERROR_HEADER

#include <string>

namespace vkapp
{
class ErrorHandler
{
    static uint32_t _error_code;

  public:
    static const uint32_t NO_ERROR = 0;

    static bool     is_app_error();
    static uint32_t get_last_error_code();
    static void     raise_error(uint32_t error_code);
};
}   // namespace vkapp

#endif