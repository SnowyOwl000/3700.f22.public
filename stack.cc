#include <stdexcept>
#include "stack.h"

Stack::Stack() { top = 0; }

void push(StackType d) {

  if (top == STACK_SIZE)
    throw std::overflow_error("Stack is full");
    
  // next two lines can be written as data[top++] = d
  data[top] = d;
  
  top++;
}

StackType peek() {

  if (top == 0)
    throw std::underflow_error("Stack is empty");
 
  return data[top-1];
}

