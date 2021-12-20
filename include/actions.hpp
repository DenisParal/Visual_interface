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
public:
    SimpleColorChangeHoverEvent(sf::Color color) : color(color) {}
    void operator()(FunctionalObject* obj)
    {
        obj->shape.set_color(1, color);
    }
};

class SwapColorChangeMouseClickEvent : public IFunctor
{
private:
    sf::Color painting_color;
public:
    SwapColorChangeMouseClickEvent(sf::Color color) : painting_color(color) {}
    void operator()(FunctionalObject* obj)
    {
        obj->shape.set_color(1, painting_color);
        obj->is_moving = !obj->is_moving;
    }
};

class SimpleMoveEvent : public IFunctor
{
private:
    sf::Color painting_color;
    sf::Window& window;
    sf::Vector2f offset;
public:
    SimpleMoveEvent(sf::Window& window, sf::Vector2f offset, sf::Color painting_color) : window(window), offset(offset), painting_color(painting_color) {}
    void operator()(FunctionalObject* obj)
    {
        obj->shape.set_color(1, painting_color);
        obj->shape.set_position(sf::Mouse::getPosition(window).x + offset.x, 600 - sf::Mouse::getPosition(window).y + offset.y);
    }
};
}


