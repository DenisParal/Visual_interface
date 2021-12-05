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
    std::vector<std::shared_ptr<sf::Shape>> shapes;
    std::vector<sf::Vector2f> positions;
    sf::Vector2f center_position;
    float scale = 1;
    std::size_t complexity;
public:
    ComplexShape(std::vector<std::shared_ptr<sf::Shape>> shapes, std::vector<sf::Vector2f> positions, sf::Vector2f center_position);
    ComplexShape(const ComplexShape& shape);
    void change_size(float coef);
    sf::Color get_color(std::size_t index) const;
    void set_color(std::size_t index, sf::Color color);
    void rotate(float angle);
    std::shared_ptr<sf::Shape> get_shape(std::size_t index) const;
    sf::Vector2f get_coordinates(std::size_t index) const;
    std::size_t get_complexity() const;
    sf::Vector2f get_position() const;
};

class Drawer
{
protected:
    float scale = 1;
public:
    virtual void draw(const ComplexShape& shape) = 0;
    void change_scale(float coef);
};

class DefaultDrawer : public Drawer
{
private:
    std::shared_ptr<sf::RenderTexture> picture;
public:
    DefaultDrawer(std::shared_ptr<sf::RenderTexture> picture);
    void draw(const ComplexShape& shape) override;
};



template<typename Func>
class button
{
private:
    float x;
    float y;
    float size_x;
    float size_y;
    Func action;
    sf::Color color;
    std::string text;
public:
    button(float x, float y, float size_x, float size_y, Func action, sf::Color color, std::string text): x(x), y(y), size_x(size_x), size_y(size_y), action(action), color(color), text(text) {}
    
    bool check_coordinate(float x, float y)
    {
        return x >= this->x && x <= this->x+this->size_x && y >= this->y && y <= this->y+this->size_y;
    }

    void display(sf::RenderWindow* window)
    {
        sf::Font font;
        font.loadFromFile("/home/dparanic/Study/Design-Patterns-labs/Lab2.2-GeometryVisualizer/src/arial.ttf");

        sf::RectangleShape rectangle(sf::Vector2f(size_x, size_y));
        sf::Text button_text(text, font);
        button_text.setFillColor(sf::Color::Black);
        button_text.setOutlineThickness(1.0f);
        button_text.setCharacterSize(25);
        button_text.setPosition(x+20.0f,y+20.0f);
        rectangle.setFillColor(color);
        rectangle.setOutlineThickness(1.0f);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition(x,y);
        window->draw(rectangle);
        window->draw(button_text);
    }
    void act()
    {
        action();
    }
};
