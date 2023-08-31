
#include <memory>

namespace vkapp
{

class Engine
{
    std::unique_ptr<Window>  _window;
    std::unique_ptr<Graphic> _graphic;

    Engine()                         = delete;
    Engine(const Engine&)            = delete;
    Engine(Engine&&)                 = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&)      = delete;

  public:
    Engine(std::unique_ptr<Window>&&, std::unique_ptr<Graphic>&&);
};

}   // namespace vkapp