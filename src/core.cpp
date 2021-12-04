#include "core.hpp"

// ComplexShape

    ComplexShape::ComplexShape(std::vector<sf::Shape> shapes, std::vector<sf::Vector2f> positions, sf::Vector2f center_position):shapes(shapes),
                               positions(positions), center_position(center_position)
    {
        for(auto& shape : shapes)
        {
            shape.setOrigin(center_position);
        }
    }

    ComplexShape::ComplexShape(const ComplexShape& shape): shapes(shape.shapes), positions(shape.positions), center_position(shape.center_position), scale(shape.scale) {}

    void ComplexShape::change_size(float coef)
    {
        scale = coef;
        for(auto& shape : shapes)
        {
            shape.setScale(coef,coef);
        }
    }
    sf::Color ComplexShape::get_color(std::size_t index) const
    {
        return shapes[index].getFillColor();
    }
    void ComplexShape::set_color(std::size_t index, sf::Color color)
    {
        shapes[index].setFillColor(color);
        shapes[index].setOutlineColor(color);
    }
    void ComplexShape::rotate(float angle)
    {
        for(auto& shape : shapes)
        {
            shape.rotate(angle);
        }
    }

//~ComplexShape

//DefaultDrawer

DefaultDrawer::DefaultDrawer(std::shared_ptr<sf::RenderTexture> picture) : picture(picture) {}

void DefaultDrawer::draw(const ComplexShape& shape)
{
    
}
//~DefaultDrawer