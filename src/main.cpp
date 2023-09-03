#include "engine.hpp"
#include "error.hpp"

#include <iostream>

int main()
{

    vkapp::Engine engine;
    engine._vk_layers_names.push_back("VK_LAYER_KHRONOS_validation");
    engine.run();
    if (vkapp::ErrorHandler::is_app_error())
    {
        std::cerr << "Code " << vkapp::ErrorHandler::get_last_error_code() << "\n";
        return -1;
    }
    return 0;
}