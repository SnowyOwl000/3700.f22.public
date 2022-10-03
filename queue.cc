#include "queue.h"

void Queue::clear() {

  tail = count = 0;
  head = QUEUE_SIZE - 1;
}
  
void Queue::enqueue(QueueType d) {

  if (count == QUEUE_SIZE)
    throw std::overflow_error("Queue is full");
    
  data[tail] = d;
  
  tail = ++tail % QUEUE_SIZE;
  
  count++;
}

QueueType Queue::dequeue() {

  if (count == 0)
    throw std::underflow_error("Queue is empty");
    
  head = ++head % QUEUE_SIZE;
  
  count--;
  
  return data[head];
}

