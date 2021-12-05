#include "core.hpp"
#include "visualizer.hpp"
#include <iostream>

int main()
{
    auto lambda = [](sf::RenderWindow& window)
    {
        auto act1 = [&window](){window.close();};
        button<decltype(act1)> b1(600,0,200,50,act1,sf::Color::Green,"scale plus");
        button<decltype(act1)> b2(600,50,200,50,act1,sf::Color::Blue,"scale minus");
        button<decltype(act1)> b3(600,100,200,50,act1,sf::Color::Blue,"refresh");
        button<decltype(act1)> b4(600,150,200,50,act1,sf::Color::Blue,"rotate");

        std::shared_ptr<sf::RenderTexture> picture = std::make_shared<sf::RenderTexture>();
        picture->create(600,600);
        float scale = 1;
        std::shared_ptr<sf::Shape> circle1 = std::make_shared<sf::CircleShape>(20.0f,10);
        std::shared_ptr<sf::Shape> circle2 = std::make_shared<sf::CircleShape>(30.0f,10);
        std::shared_ptr<sf::Shape> rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(100, 20));
        circle1->setFillColor(sf::Color::Green);
        circle2->setFillColor(sf::Color::Yellow);
        rect->setFillColor(sf::Color::Yellow);
        ComplexShape c1{std::vector<std::shared_ptr<sf::Shape>>{rect, circle2, circle1}, std::vector<sf::Vector2f>{{-20.0f,0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, sf::Vector2f{300,300}};
        std::shared_ptr<Drawer> drawer = std::make_shared<DefaultDrawer>(std::shared_ptr<sf::RenderTexture>(picture));


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
                    if (b1.check_coordinate(localPosition.x, localPosition.y))
                    {
                        scale = 1.1f;
                        std::cout << scale << std::endl;
                        drawer->change_scale(scale);
                        scale = 1.0f;
                    }
                    else if (b2.check_coordinate(localPosition.x, localPosition.y))
                    {
                        scale = 0.9f;
                        std::cout << scale << std::endl;
                        drawer->change_scale(scale);
                        scale = 1.0f;
                    }
                    else if (b3.check_coordinate(localPosition.x, localPosition.y))
                    {
                        picture->clear(sf::Color::White);
                        drawer->draw(c1);
                    }
                    else if (b4.check_coordinate(localPosition.x, localPosition.y))
                    {
                        picture->clear(sf::Color::White);
                        c1.rotate(-10);
                        drawer->draw(c1);
                    }
                }
            }

            window.clear(sf::Color::White);
            b1.display(&window);
            b2.display(&window);
            b3.display(&window);
            b4.display(&window);
            sf::Sprite sprite1(picture->getTexture());

            window.draw(sprite1);
            window.display();
        }
    };
    gr_interface<decltype(lambda)> interface(800,600,"start",lambda);
    interface.init();
}
