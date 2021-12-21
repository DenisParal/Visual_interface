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

    ComplexShape::Memento::Memento(ComplexShape* owner, const std::vector<std::shared_ptr<sf::Shape>>& shapes, const std::vector<sf::Vector2f>& positions,
                sf::Vector2f center_position, float scale, std::size_t complexity): owner(owner), shapes(shapes), positions(positions), center_position(center_position),
                                                                                    scale(scale), complexity(complexity) {}

    void ComplexShape::Memento::restore()
    {
        owner->shapes = shapes;
        owner->positions = positions;
        owner->center_position = center_position;
        owner->scale = scale;
        owner->complexity = complexity;
    }

    IMemento* ComplexShape::save()
    {
        IMemento* memento = new Memento(this, shapes, positions, center_position, scale, complexity);
        return memento;
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

FunctionalObject::FunctionalObject(const ComplexShape& shape) : shape(shape) 
{
    std::shared_ptr<FunctionalObject> ptr;
    MementoManager::get_instance()->registrate(this);//probably there will be error with shared_ptr
}

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
    if(hover_func)
    {
        hover_func->operator()(this);
    }
}
void FunctionalObject::act_on_click()
{
    if(mouse_func)
    {
        mouse_func->operator()(this);
    }
}
void FunctionalObject::act_on_move()
{
    if(move_func)
    {
        move_func->operator()(this);
    }
}
void FunctionalObject::act_on_break_hovering()
{
    if(break_hover_func)
    {
        break_hover_func->operator()(this);
    }
}
bool FunctionalObject::check_coordinate(float x, float y)
{
    return shape.contains(x,y);
}

FunctionalObject::Memento::Memento(FunctionalObject* owner, std::shared_ptr<IFunctor> hover_func, std::shared_ptr<IFunctor> mouse_func, std::shared_ptr<IFunctor> move_func,
                std::shared_ptr<IFunctor> break_hover_func, IMemento* shape_save): owner(owner), hover_func(hover_func), mouse_func(mouse_func),
                                                                                                   move_func(move_func), break_hover_func(break_hover_func), shape_save(shape_save) {}
void FunctionalObject::Memento::restore()
{
    owner->hover_func = hover_func;
    owner->mouse_func = mouse_func;
    owner->move_func = move_func;
    owner->break_hover_func = break_hover_func;
    shape_save->restore();
}

IMemento* FunctionalObject::save()
{
    IMemento* shape_save = shape.save();
    IMemento* memento = new Memento(this, hover_func, mouse_func, move_func, break_hover_func, shape_save);
    return memento;
}

// ~FunctionalObject

// Memento

    MementoManager* MementoManager::instance;
    MementoManager::Memento::Memento(MementoManager* owner, const std::vector<IMementable*>& mementable_list,
                                     const std::vector<IMemento*>& memory): owner(owner), mementable_list(mementable_list), memory(memory) {}

    void MementoManager::Memento::restore()
    {
        owner->mementable_list = mementable_list;
        for(auto& item : memory)
        {
            item->restore();
        }
    }
    MementoManager* MementoManager::get_instance()
    {
        if(instance == nullptr)
        {
            instance = new MementoManager{};
        }
        return instance;
    }

    MementoManager::MementoManager(){}

    void MementoManager::registrate(IMementable* obj)
    {
        mementable_list.push_back(obj);
    }

    IMemento* MementoManager::save()
    {
        std::vector<IMemento*> memory(mementable_list.size());
        for(std::size_t i = 0; i < memory.size(); i++)
        {
            memory[i]=mementable_list[i]->save();
        }
        IMemento* memento = new Memento(instance, mementable_list, memory);
        return memento;
    }

// ~Memento