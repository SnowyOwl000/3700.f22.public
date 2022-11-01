//
// Created by bob on 4/3/20.
//

#ifndef LINEARLIST_H
#define LINEARLIST_H

#include <cstdint>

template <typename ListType>
struct ListNode {
    ListType
        datum;
    ListNode<ListType>
        *next;
};

template <typename ListType>
class LinearList {
public:
    LinearList();
    LinearList(const LinearList<ListType> &);
    ~LinearList();

    uint32_t size();
    bool isEmpty();

    void clear();

    uint32_t search(const ListType &);
    ListType &operator[](int32_t);

    void map(void (*)(ListType &));

    void insert(uint32_t,const ListType &);
    void remove(uint32_t);
private:
    ListNode<ListType>
        *head;
    uint32_t
        count;
};

#include <linearlist-implementation.h>

#endif //LINEARLIST_H
