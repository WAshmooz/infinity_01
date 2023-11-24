#include <iostream>
using namespace std; //prefer not to use namespace

class Base
{
public:
    Base(int i): m_i(i), m_iptr(&m_i) {}

    virtual void foo() {cout << "Base foo: " << *(m_iptr) << endl;}
    virtual void bar() {cout << "Base bar: " << m_i << endl;}
    //virtual ~Base()  = 0;
    Base(const Base& other) : m_i(other.m_i), m_iptr(&m_i) {}

protected:

private:
    virtual ~Base() = 0;
    int m_i;
    int *m_iptr;
};

inline Base::~Base() {}

class Derived: public Base
{
public:
    Derived(int i, char c): m_c(c), Base(i), m_c2(c) {}
    void bar() {foo(); Base::foo();}

 // Copy constructor for Derived
    Derived(const Derived& other) : Base(other), m_c(other.m_c), m_c2(other.m_c2) {}

 
private:
    void foo() {cout << "Derived foo: " <<  m_c << endl; }
    char m_c, m_c2;
};

class OtherDerived: public Base
{
};

int main(int argc, char **argv)
{
    Derived d1(5, 'x');
    Derived d2(3, 'w');

    Base *b1 = &d1;
    b1->foo();
    b1->bar();

    Base *b2 = &d2;
    *b2 = *b1;
    b2->foo();
    d2.bar();
    
    return 0;
}



