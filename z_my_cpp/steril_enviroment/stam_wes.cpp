/*******************************************************************************
 * Author: Daniel
 * Date: 12/09/2023
 * Version: 1
 * Description: C intro 32
 *******************************************************************************/
#include <iostream>
#include <cstdio>

// namespace ilrd
using namespace std;

namespace useless
{
    void foo(){
        printf("foo\n");
    }

    namespace useless2
    {
        void foo(){
            printf("foo in\n");
        }
        void foo2(){
            printf("foo2 in\n");
        }
    }
}

int main()
{

    useless::useless2::foo();

    return 0;
}

Exercise "Member Functions": (cpp/ws/point/)

1. create a struct Point that represents a point space in (integer) 
cartesian coordinates(x,y).
The point should support the following functionality:
-AdjusBy - Receives another struct Point and adjusts the current 
point's (x,y) by addig the point's (x,y) to itself.
-Lenght - Return the linght of the Point's vector: the distance of the 
point from (0,0) (use Pitagoras)
Print - Print the point to stdout in the following format: "(x,y)". 
The type of the two parentheses ('(','[','<' etc.) is passed as two 
parameters to the function (a char for opening and a char for closing)
Put struct Point and its related code in files point.h and point.cpp

2. write a global function Add that receives 2 points as arguments 
and returns a new point whose (x,y) is the sum of the (x,y) of both
points given as arguments


****Exercise "Function overloading": (cpp/ws/point/)******
2. add an overloading to Print that receives a singal parameter that is an
enum of parentheses type.
enum ParenthesesType { ROUND, SQUARE, CURLY, ANGULAR }
Define the enum within the struct so it is part of the struct's scope. 
The same parameter detrmaines both the opening and closing panetheses type.

3.Add calls to the new function from your test program
4. How do we use value from the enum defined within the struct? 


****Exercise "References": (cpp/ws/point/)****
1. change previous Point's interface so the function AdjusBy that
receives a Point as argument will receive the point by const reference.
2.change both AdjusBy overloa


****Exercise "Default Parameters": (cpp/ws/point/)****
1. Add an IsEqual function to Point that receives another point as
parameter and return true if the given point is equal to the current
point.
2. Add a default value ROUND as the value of the enum argument to Print
that receives an enum parameter (so the default is to print with round 
panetheses). Change your test code to call this Print function at least once
with an explicit parameter value and once without (so that the default is used).
3. try to define a similar default to the Print function that recrives two chars.
Change your test so sometimes you use the default values ans somthimes not.
Document why this is problematic to use
4. Look at the implementation of the Length() function of Point. Since the 
calculation is quite heavy (sqrt is a heavy function) we would like to calculate 
the value as few times as possible and keep and reuse the calculation's result
in futher calls (this is called caching).

-when would you save the cached value? (note that we can have multiple points
that we call Length() on)
-how would you know if the cached values is already calculated or not? (if
this is the first time)
-what should the added caching variabels be initialized to? 
how many changes does this require to thr existing code in your test program?
does the compiler help you find the places requiring changes?
could you write the code in a way that the compiler tells you where the changes
to your code are required?
-what should happen to the cached value if the user changes the pointer's location
(i.e, by calling AdjusBy or by changing x,y another way)?
implement the changes needed so this works correctly.


****Exercise "new / delete": (cpp/ws/polygon/)****

write a Polygon struct that defines a polygon as a collection of points
implement a stack by using a linked list of integers.
implementation should handle memory allocations in a way that memory 
leaks are impossible.

use the definition below:

typedef int DataType

struct Stack
{
public:
    explicit Stack();
    ~Stack();

    voidPush(DataType data);
    void Pop();
    DataType Top() const;
    size_t Count() const;
    bool IsEqal() const;

    struct Node
    {
        explicit Node();
        explicit Node(DataType data, Node *Next);

        DataType GetData() const;
        bool HasNext() const;
        const Node &GetNext() const;

        const DataType m_data;
        const Node* m_next;
    };

    Node m_dummy;
};

what horrible mistake exists in the above definition of Stack?
Give an example usage that will case the program to crash.






use a dynamically allocated array for holding Points 
-the size of the array should be exactly the size required for holding
the polygons points (no need to reserve space for future points as you
did when implementing a dynamic vector).
-there is no need to remove or overwrite duplicate points or the points 
initially set to zero (yes, this is not very practiacl or even logical..
it is inly for practice reasons)
-IsEqual cheecks if the other polygon is exactly the same (the sme 
number of points, each point value at the same index as the other polygon)

support copping and asiigning polygons
use the definition below:

struct Polygon{
    //numPoints - initial number of points, all of them with value (0.0)
    explicit Poligon(size_t numPoints = 0);
    //Also define dtor, cctor, op=
    .
    .
    .
    void Add(const Point &p);
    bool IsEqal(const Polygon& o_) const;
    void Print() const;

    size_t m_numPoints;
    Point *m_points;
};


***************STANDART CONTAINERS*********************

1. wtite a program called histro that lets the user enter lines
of text as standard input.
2. the user terminates the program by enteiring . (a dot) on an
empty line, and then pressing Enter
3. after the user terminates the program, each line the user
entered as input is then printed in the same order that the user 
had intup the lines, however with duplicates removed (following 
the first occurrence).
the number of times a line was duplicated is then printed next 
to the line of text
4. the program then exits

implementation requiremrnts:
1. use the STL conrainers and try write thr program as short
as possible.
2. duplicates are also detrmined by thier case (case-sensitive).




