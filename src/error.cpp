#include "error.hpp"

namespace vkapp
{
uint32_t ErrorHandler::_error_code = ErrorHandler::NO_ERROR;

bool ErrorHandler::is_app_error()
{
    return _error_code != ErrorHandler::NO_ERROR;
}
uint32_t ErrorHandler::get_last_error_code()
{
    return _error_code;
}
void ErrorHandler::raise_error(uint32_t error_code)
{
    _error_code = error_code;
}
}   // namespace vkapp