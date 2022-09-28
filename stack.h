#ifndef _STACK_H
#define _STACK_H

typedef int32_t StackType;

const uint32_t
  STACK_SIZE = 16;
  
class Stack {
public:
  Stack();
  ~Stack() = default;
  
  void clear() { top = 0; }
  bool isEmpty() { return top == 0; }
  uint32_t size() { return top; }
  
  void push(StackType d);
  StackType pop();
  StackType peek();

private:
  StackType
    data[STACK_SIZE];
  uint32_t
    top;
};

#endif

