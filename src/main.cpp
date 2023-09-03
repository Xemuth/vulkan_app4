#include "engine.hpp"
#include "error.hpp"

#include <iostream>

int main()
{

    vkapp::Engine engine;
    engine.run();
    if (vkapp::ErrorHandler::is_app_error())
    {
        std::cerr << "Code " << vkapp::ErrorHandler::get_last_error_code() << "\n";
        return -1;
    }
    return 0;
}