/*
write an API with the implementaion of a generic stack in c++ 
clarifications:
required APIs - Ctor Dtor Push() Peek() Pop()
no use of STL 
the stack should grow automatically
the stack never shrink
space complexity o(n)
time complexity pf Push()/Pop() O(1)
*/

#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& elem) : data(elem), next(nullptr) {}
    };

    Node* top_node;

public:
    Stack() : top_node(nullptr) {}

    ~Stack() {
        while (top_node != nullptr) {
            Node* temp = top_node;
            top_node = top_node->next;
            delete temp;
        }
    }

    void Push(const T& elem) {
        Node* newNode = new Node(elem);
        newNode->next = top_node;
        top_node = newNode;
    }

    void Pop() {
        if (top_node == nullptr) {
            cout << "Stack underflow! Cannot pop from an empty stack." << endl;
            return;
        }
        Node* temp = top_node;
        top_node = top_node->next;
        delete temp;
    }

    T Peek() const {
        if (top_node == nullptr) {
            cerr << "Stack is empty. No elements to peek." << endl;
            throw out_of_range("Stack is empty. No elements to peek.");
        }
        return top_node->data;
    }
};

























