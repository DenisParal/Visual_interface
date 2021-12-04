#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class Drawer;

class ComplexShape
{
friend class Drawer;
private:
    std::vector<sf::Shape> shapes;
    std::vector<sf::Vector2f> positions;
    sf::Vector2f center_position;
    float scale = 1;
public:
    ComplexShape(std::vector<sf::Shape> shapes, std::vector<sf::Vector2f> positions, sf::Vector2f center_position);
    ComplexShape(const ComplexShape& shape);
    void change_size(float coef);
    sf::Color get_color(std::size_t index) const;
    void set_color(std::size_t index, sf::Color color);
    void rotate(float angle);
};

class Drawer
{
public:
    virtual void draw(const ComplexShape& shape) = 0;
};

class DefaultDrawer : public Drawer
{
private:
    std::shared_ptr<sf::RenderTexture> picture;
public:
    DefaultDrawer(std::shared_ptr<sf::RenderTexture> picture);
    void draw(const ComplexShape& shape) override;
};

