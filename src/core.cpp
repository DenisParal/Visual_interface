#include "core.hpp"
#include <iostream>
#include <math.h>

// ComplexShape

    ComplexShape::ComplexShape(std::vector<std::shared_ptr<sf::Shape>> shapes, std::vector<sf::Vector2f> positions, sf::Vector2f center_position):shapes(shapes),
                               positions(positions), center_position(center_position), complexity(shapes.size())
    {
        for(auto& shape : shapes)
        {
            shape->setOrigin(shape->getLocalBounds().width/2.0f, shape->getLocalBounds().height/2.0f);
        }
    }

    ComplexShape::ComplexShape(const ComplexShape& shape): shapes(shape.shapes), positions(shape.positions), center_position(shape.center_position),
                               scale(shape.scale), complexity(shape.complexity) {}

    void ComplexShape::change_size(float coef)
    {
        scale = coef;
        for(auto& shape : shapes)
        {
            shape->setScale(coef,coef);
        }
    }

    void ComplexShape::set_position(float x, float y)
    {
        center_position = sf::Vector2f(x, y);
    }

    sf::Color ComplexShape::get_color(std::size_t index) const
    {
        return shapes[index]->getFillColor();
    }
    void ComplexShape::set_color(std::size_t index, sf::Color color)
    {
        shapes[index]->setFillColor(color);
        shapes[index]->setOutlineColor(color);
    }
    void ComplexShape::rotate(float angle)
    {
        float rad = (angle*3.14f)/180.0f;
        float cs = std::cos(rad);
        float sn = std::sin(rad);
        for(std::size_t i = 0; i < complexity; i++)
        {
            shapes[i]->rotate(angle);
            sf::Vector2f vec = positions[i];
            positions[i]=sf::Vector2f(vec.x*cs - vec.y*sn, vec.x*sn + vec.y*cs);
        }
    }

    std::shared_ptr<sf::Shape> ComplexShape::get_shape(std::size_t index) const
    {
        return shapes[index];
    }

    sf::Vector2f ComplexShape::get_coordinates(std::size_t index) const
    {
        return positions[index];
    }

    std::size_t ComplexShape::get_complexity() const
    {
        return complexity;
    }

    sf::Vector2f ComplexShape::get_position() const
    {
        return center_position;
    }

    bool ComplexShape::contains(float x, float y) const
    {
        bool result = true;
        for(auto& shape : shapes)
        {
            sf::FloatRect tmp_rect = shape->getGlobalBounds();
            if(!tmp_rect.contains(x,y))
            {
                result = false;
                break;
            }
        }
        return result;
    }

//~ComplexShape

//DefaultDrawer

DefaultDrawer::DefaultDrawer(std::shared_ptr<sf::RenderTexture> picture) : picture(picture) {}

void Drawer::change_scale(float coef)
{
    scale = coef;
}

void DefaultDrawer::draw(const ComplexShape& shape)
{
    for(std::size_t i = 0; i < shape.get_complexity(); ++i)
    {
        std::shared_ptr<sf::Shape> tmp_shape = shape.get_shape(i);
        sf::Vector2f central_p = shape.get_position();
        sf::Vector2f shape_p = shape.get_coordinates(i);
        central_p.x += shape_p.x * scale;
        central_p.y += shape_p.y * scale;
        tmp_shape->setPosition(central_p);
        tmp_shape->scale(scale, scale);
        picture->draw(*tmp_shape);
    }
}
//~DefaultDrawer

// FunctionalObject

FunctionalObject::FunctionalObject(const ComplexShape& shape) : shape(shape) {}

void FunctionalObject::set_hover_action(std::shared_ptr<IFunctor> action)
{
    hover_func = action;
}
void FunctionalObject::set_mouse_click_action(std::shared_ptr<IFunctor> action)
{
    mouse_func = action;
}
void FunctionalObject::set_move_action(std::shared_ptr<IFunctor> action)
{
    move_func = action;
}
void FunctionalObject::set_break_hovering_action(std::shared_ptr<IFunctor> action)
{
    break_hover_func = action;
}

void FunctionalObject::act_on_hovering()
{
    hover_func->operator()();
}
void FunctionalObject::act_on_click()
{
    mouse_func->operator()();
}
void FunctionalObject::act_on_move()
{
    move_func->operator()();
}
void FunctionalObject::act_on_break_hovering()
{
    break_hover_func->operator()();
}
bool FunctionalObject::check_coordinate(float x, float y)
{
    return shape.contains(x,y);
}
// ~FunctionalObject