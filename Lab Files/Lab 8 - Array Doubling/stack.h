#ifndef _STACK_H
#define _STACK_H

#include <stdexcept>

using namespace std;

const uint32_t
    STACK_SIZE = 16;
  
template <typename StackType>
class Stack {
public:
    Stack(uint32_t sSize=STACK_SIZE) {
  
        data = new StackType[sSize];
        capacity = sSize;
    
        top = 0;
    }

    ~Stack() { delete[] data; }

    void clear() { top = 0; }

    uint32_t size() { return top; }

    bool isEmpty() { return !top; }
  

    void push(StackType d) {
  
        if (top == capacity)
            throw overflow_error("Stack is full");
      
        data[top++] = d;
    }
  
    StackType pop() {

        if (top == 0)
            throw underflow_error("Stack is empty");
      
        return data[--top];
    }
    
    StackType peek() {
  
        if (top == 0)
            throw underflow_error("Stack is empty");
      
        return data[top-1];
    }
  
private:
    StackType
        *data;
    
    uint32_t
        top,
        capacity;
};

#endif

