#pragma once
#include <SFML/Graphics.hpp>
#include <string>

template<typename Func>
class gr_interface
{
private:
    sf::RenderWindow window;
    Func run_function;
    sf::Thread thread;
    std::size_t size_x;
    std::size_t size_y;
    std::string window_name;

    class thread_handler
    {
        Func func;
        std::size_t size_x;
        std::size_t size_y;
        std::string window_name;
    public:
        thread_handler(std::size_t size_x, std::size_t size_y, std::string window_name, Func run_function): size_x(size_x), size_y(size_y), window_name(window_name), func(run_function) {}
        void operator()(sf::RenderWindow* window)
        {
            window->create(sf::VideoMode(size_x, size_y), window_name);
            window->setActive(true);
            func(*window);
        }
    };

public:
    gr_interface(std::size_t size_x, std::size_t size_y, std::string window_name, Func run_function): size_x(size_x), size_y(size_y), window_name(window_name), run_function(run_function), thread(thread_handler{size_x, size_y, window_name, run_function}, &window) {}
    void init()
    {
        if(!window.isOpen())
        {
            thread.launch();
        }
    }
    ~gr_interface()
    {
        thread.wait();
    }
};