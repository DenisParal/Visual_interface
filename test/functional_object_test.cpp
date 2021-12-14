#include "core.hpp"
#include "visualizer.hpp"
#include "actions.hpp"
#include <iostream>

int main()
{
    auto lambda = [](sf::RenderWindow& window)
    {
        auto act1 = [&window](){window.close();};

        std::shared_ptr<sf::RenderTexture> picture = std::make_shared<sf::RenderTexture>();
        picture->create(800,600);
        std::shared_ptr<sf::Shape> circle1 = std::make_shared<sf::CircleShape>(20.0f,10);
        std::shared_ptr<sf::Shape> circle2 = std::make_shared<sf::CircleShape>(30.0f,10);
        circle1->setFillColor(sf::Color::White);
        circle2->setFillColor(sf::Color::Black);
        ComplexShape c1{std::vector<std::shared_ptr<sf::Shape>>{circle2, circle1}, std::vector<sf::Vector2f>{{0.0f, 0.0f}, {0.0f, 0.0f}}, sf::Vector2f{300,300}};
        std::shared_ptr<Drawer> drawer = std::make_shared<DefaultDrawer>(std::shared_ptr<sf::RenderTexture>(picture));

        std::shared_ptr<FunctionalObject> obj = std::make_shared<FunctionalObject>(c1);

        std::shared_ptr<IFunctor> hover_action = std::make_shared<Actions::SimpleColorChangeHoverEvent>(sf::Color::Green, obj);
        std::shared_ptr<IFunctor> mouse_click = std::make_shared<Actions::SwapColorChangeMouseClickEvent>(sf::Color::Red, obj);
        std::shared_ptr<IFunctor> hover_break = std::make_shared<Actions::SimpleColorChangeHoverEvent>(sf::Color::White, obj);
        std::shared_ptr<IFunctor> move_action = std::make_shared<Actions::SimpleMoveEvent>(obj, window, sf::Vector2f{0, 0});

        obj->set_hover_action(hover_action);
        obj->set_mouse_click_action(mouse_click);
        obj->set_move_action(move_action);
        obj->set_break_hovering_action(hover_break);
        
        while(window.isOpen())
        {
            sf::Event event;
            while(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                obj->act_on_move();
                window.clear(sf::Color::White);
                picture->clear(sf::Color::White);
                drawer->draw(obj->shape);
                sf::Sprite sprite1(picture->getTexture());

                window.draw(sprite1);
                window.display();
            }
            
            if(obj->is_moving)
            {
                std::cout <<"UNPRESSED\n";
                obj->act_on_click();
                obj->is_moving=false;
            }

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                    if(obj->shape.contains(mouse_position.x, 600 - mouse_position.y))
                    {
                        obj->act_on_click();
                        obj->is_moving = true;
                        break;
                    }
                }

                if (event.type == sf::Event::MouseMoved)
                {
                    // std::cout << sf::Mouse::getPosition().x - obj->shape.get_position().x << " " << sf::Mouse::getPosition().y - obj->shape.get_position().y << "\n";
                    // std::cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << "\n";
                    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                    if(!obj->is_moving)
                    {
                        if(obj->shape.contains(mouse_position.x, 600 - mouse_position.y))
                        {
                            obj->act_on_hovering();
                        }
                        else
                        {
                            obj->is_hovering = false;
                            obj->act_on_break_hovering();
                        }
                    }
                    // if(obj->is_moving)
                    // {
                    //     obj->act_on_move();
                    // }
                }

                
            }

            window.clear(sf::Color::White);
            picture->clear(sf::Color::White);
            drawer->draw(obj->shape);
            sf::Sprite sprite1(picture->getTexture());

            window.draw(sprite1);
            window.display();
        }
    };
    gr_interface<decltype(lambda)> interface(800,600,"start",lambda);
    interface.init();
}