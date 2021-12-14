#pragma once
#include <SFML/Graphics.hpp>
#include "core.hpp"
#include <memory>
#include <iostream>

namespace Actions
{
    
class SimpleColorChangeHoverEvent : public IFunctor
{
private:
    sf::Color color;
    std::shared_ptr<FunctionalObject> func_obj;
public:
    SimpleColorChangeHoverEvent(sf::Color color, std::shared_ptr<FunctionalObject> func_obj) : color(color), func_obj(func_obj) {}
    void operator()()
    {
        func_obj->shape.set_color(1, color);
    }
};

class SwapColorChangeMouseClickEvent : public IFunctor
{
private:
    sf::Color painting_color;
    sf::Color saved_color;
    std::shared_ptr<FunctionalObject> func_obj;
public:
    SwapColorChangeMouseClickEvent(sf::Color color, std::shared_ptr<FunctionalObject> func_obj) : saved_color(color), func_obj(func_obj) {}
    void operator()()
    {
        saved_color = func_obj->shape.get_color(1);
        func_obj->shape.set_color(1, painting_color);
        func_obj->is_moving = !func_obj->is_moving;
    }
};

class SimpleMoveEvent : public IFunctor
{
private:
    std::shared_ptr<FunctionalObject> func_obj;
    sf::Window& window;
    sf::Vector2f offset;
public:
    SimpleMoveEvent(std::shared_ptr<FunctionalObject> func_obj, sf::Window& window, sf::Vector2f offset) : func_obj(func_obj), window(window), offset(offset) {}
    void operator()()
    {
        func_obj->shape.set_position(sf::Mouse::getPosition(window).x + offset.x, 600 - sf::Mouse::getPosition(window).y + offset.y);
    }
};
}


