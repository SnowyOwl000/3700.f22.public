#ifndef _QUEUE_H
#define _QUEUE_H

typedef char QueueType;

const uint32_t
  QUEUE_SIZE = 16;
  
class Queue {
public:
  Queue() { clear(); }
  ~Queue() = default;
  
  bool isEmpty() { return count == 0; }
  uint32_t size() { return count; }
  
  void clear();
  
  void enqueue(QueueType d);
  QueueType dequeue();
  
private:
  QueueType
    data[QUEUE_SIZE];
  uint32_t
    head,tail,
    count;
};

#endif

