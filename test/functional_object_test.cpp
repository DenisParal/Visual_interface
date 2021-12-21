#include "core.hpp"
#include "visualizer.hpp"
#include "actions.hpp"
#include <iostream>

std::shared_ptr<FunctionalObject> chose_obj(sf::RenderWindow& window, const std::vector<std::shared_ptr<FunctionalObject>>& obj_v)
{
    std::shared_ptr<FunctionalObject> chosen = nullptr;
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
    for(auto& ptr : obj_v)
    {
        if(ptr->shape.contains(mouse_position.x, 600 - mouse_position.y))
        {
            chosen = ptr;
        }
    }
    return chosen;
}

std::vector<std::shared_ptr<sf::Shape>> get_custom_circle()
{
    std::shared_ptr<sf::Shape> circle1 = std::make_shared<sf::CircleShape>(20.0f,10);
    std::shared_ptr<sf::Shape> circle2 = std::make_shared<sf::CircleShape>(30.0f,10);
    circle1->setFillColor(sf::Color::White);
    circle2->setFillColor(sf::Color::Black);
    std::vector<std::shared_ptr<sf::Shape>> res{circle2, circle1};
    return res;
}

int main()
{
    auto lambda = [](sf::RenderWindow& window)
    {
        auto act1 = [&window](){window.close();};
        IMemento* save = nullptr;
        bool save_status = false; // workaround for double free error
        button<decltype(act1)> b1(600,0,200,50,act1,sf::Color::Green,"save/load");

        std::shared_ptr<sf::RenderTexture> picture = std::make_shared<sf::RenderTexture>();
        picture->create(800,600);
        std::shared_ptr<Drawer> drawer = std::make_shared<DefaultDrawer>(std::shared_ptr<sf::RenderTexture>(picture));

        std::shared_ptr<IFunctor> hover_action = std::make_shared<Actions::SimpleColorChangeHoverEvent>(sf::Color::Green);
        std::shared_ptr<IFunctor> mouse_click = std::make_shared<Actions::SwapColorChangeMouseClickEvent>(sf::Color::Red);
        std::shared_ptr<IFunctor> hover_break = std::make_shared<Actions::SimpleColorChangeHoverEvent>(sf::Color::White);
        std::shared_ptr<IFunctor> move_action = std::make_shared<Actions::SimpleMoveEvent> (window, sf::Vector2f{0, 0}, sf::Color::Red);

        
        ComplexShape c1{get_custom_circle(), std::vector<sf::Vector2f>{{0.0f, 0.0f}, {0.0f, 0.0f}}, sf::Vector2f{300,300}};

        std::vector<std::shared_ptr<FunctionalObject>> obj_v;
        
        std::shared_ptr<FunctionalObject> chosen = nullptr;

        while(window.isOpen())
        {
            sf::Event event;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    chosen = chose_obj(window, obj_v);
                    if(chosen != nullptr)
                    {
                        chosen->is_moving=true;
                        while(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        {
                            chosen->act_on_move();
                            window.clear(sf::Color::White);
                            picture->clear(sf::Color::White);
                            for(auto& ptr : obj_v)
                            {
                                drawer->draw(ptr->shape);
                            }
                            sf::Sprite sprite1(picture->getTexture());

                            window.draw(sprite1);
                            window.display();
                        }
                        chosen->act_on_click();
                        chosen->is_moving=false;
                    }
                    else if(b1.check_coordinate(localPosition.x, localPosition.y))
                    {
                        if(!save_status)
                        {
                            save = MementoManager::get_instance()->save();
                            save_status = true;
                        }
                        else
                        {
                            save->restore();
                            save_status = false;
                            save = nullptr; //this line produces double free error, try to fix in future
                        }
                    }
                    else
                    {
                        
                        std::shared_ptr<FunctionalObject> obj = std::make_shared<FunctionalObject>(ComplexShape{get_custom_circle(), std::vector<sf::Vector2f>{{0.0f, 0.0f}, {0.0f, 0.0f}}, sf::Vector2f{300,300}});
                        obj->shape.set_position(sf::Mouse::getPosition(window).x, 600 - sf::Mouse::getPosition(window).y);
                        obj->set_hover_action(hover_action);
                        obj->set_mouse_click_action(mouse_click);
                        obj->set_move_action(move_action);
                        obj->set_break_hovering_action(hover_break);
                        obj_v.push_back(obj);
                        chosen = obj_v[obj_v.size()-1];
                        chosen->act_on_hovering();
                    }
                }

                if (event.type == sf::Event::MouseMoved)
                {
                    std::shared_ptr<FunctionalObject> chosen_tmp = chose_obj(window, obj_v);
                    if(chosen_tmp != nullptr)
                    {
                        chosen = chosen_tmp;
                        chosen->act_on_hovering();
                    }
                    else if(chosen!= nullptr && !chosen->is_moving)
                    {
                        chosen->is_hovering = false;
                        chosen->act_on_break_hovering();
                        chosen.reset();
                    }
                }

                
            }

            window.clear(sf::Color::White);
            picture->clear(sf::Color::White);
            for(auto& ptr : obj_v)
            {
                drawer->draw(ptr->shape);
            }
            sf::Sprite sprite1(picture->getTexture());

            window.draw(sprite1);
            b1.display(&window);
            window.display();
        }
    };
    gr_interface<decltype(lambda)> interface(800,600,"start",lambda);
    interface.init();
}