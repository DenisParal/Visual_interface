#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class Drawer;
class IFunctor;

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
    void set_color(std::size_t index, sf::Color color);
    void set_position(float x, float y);
    void rotate(float angle);

    sf::Color get_color(std::size_t index) const;
    std::shared_ptr<sf::Shape> get_shape(std::size_t index) const;
    sf::Vector2f get_coordinates(std::size_t index) const;
    std::size_t get_complexity() const;
    sf::Vector2f get_position() const;
    bool contains(float x, float y) const;
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


class FunctionalObject
{
private:
    std::shared_ptr<IFunctor> hover_func = nullptr;
    std::shared_ptr<IFunctor> mouse_func = nullptr;
    std::shared_ptr<IFunctor> move_func = nullptr;
    std::shared_ptr<IFunctor> break_hover_func = nullptr;
public:
    ComplexShape shape;
    FunctionalObject(const ComplexShape& shape);
    void set_hover_action(std::shared_ptr<IFunctor> action);
    void set_mouse_click_action(std::shared_ptr<IFunctor> action);
    void set_move_action(std::shared_ptr<IFunctor> action);
    void set_break_hovering_action(std::shared_ptr<IFunctor> action);
    void act_on_hovering();
    void act_on_click();
    void act_on_move();
    void act_on_break_hovering();
    std::shared_ptr<FunctionalObject> get_entity();
    bool check_coordinate(float x, float y);

    bool is_hovering = false;
    bool is_moving = false;
};

class IFunctor
{
public:
    virtual void operator()(FunctionalObject* obj) = 0;
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
