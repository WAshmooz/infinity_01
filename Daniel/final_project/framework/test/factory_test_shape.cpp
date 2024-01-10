#include <iostream>
#include <memory>
#include <functional>

#include "factory.hpp"

/*******************  Base class   *******************/
class Shape 
{
public:
    virtual void Draw() const = 0;
    virtual ~Shape() = default;
};

/*******************  Derived class 1   *******************/ 
class Circle : public Shape 
{
public:
    explicit Circle(int x = 0) : m(x) 
    {
        std::cout << "Circle Ctor" << std::endl;
    };

    ~Circle() 
    {
        std::cout << "Circle Dtor" << std::endl;
    };
    
    Circle(const Circle& other_) = default;
    Circle& operator=(const Circle& other_) = default;

    void Draw() const override 
    {
        std::cout << "Drawing Circle" << std::endl;
    }

    // Factory method to create Circle
    static std::shared_ptr<Circle> CreateCircle(int m)
    {
        return std::make_shared<Circle>(m);
    }

private:
    int m;

};

/*******************  Derived class 2   *******************/
class Square : public Shape 
{
public:
    explicit Square(int x = 0) : m(x) 
    {
        std::cout << "Square Ctor" << std::endl;
    };
    
    ~Square() 
    {
        std::cout << "Square Dtor" << std::endl;
    };

    Square(const Square& other_) = default;
    Square& operator=(const Square& other_) = default;

    void Draw() const override 
    {
        std::cout << "Drawing Square" << std::endl;
    }

    // Factory method to create Square
    static std::shared_ptr<Square> CreateSquare(int m)
    {
        return std::make_shared<Square>(m);
    }

private:
    int m;
    
};

/****************************   Main    ***************************************/

int main() 
{
    using namespace hrd31;

    // Create a Factory for Shape objects with string keys
    using factory_t = Factory<Shape, std::string, int>;
    factory_t *factory_instance = Singleton<factory_t>::GetInstance();

    // Add functions to create Circle + Square instances
    factory_instance->Add("circle", Circle::CreateCircle);
    factory_instance->Add("square", Square::CreateSquare);

    // Use the Factory to create instances based on keys
    std::shared_ptr<Shape> circle = factory_instance->Create("circle", 1);
    std::shared_ptr<Shape> square = factory_instance->Create("square", 2);

    // Test the created objects
    if (circle)
        circle->Draw(); // Output: Drawing Circle
    if (square)
        square->Draw(); // Output: Drawing Square

    return 0;
}
