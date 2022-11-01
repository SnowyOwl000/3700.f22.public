//
// Created by bob on 4/3/20.
//

#ifndef LINEARLIST_LINEARLIST_IMPLEMENTATION_H
#define LINEARLIST_LINEARLIST_IMPLEMENTATION_H

#include <linearlist.h>
#include <stdexcept>

//=============================================================================
//----------------------------- common operations -----------------------------
//=============================================================================

//=============================================================================
// LinearList<ListType>::LinearList()
//  constructor
//

template <typename ListType>
LinearList<ListType>::LinearList() {

    head = nullptr;     // as discussed in the slides
    count = 0;
}

template<typename ListType>
LinearList<ListType>::LinearList(const LinearList<ListType> &list) {
    ListNode<ListType>
        *src = list.head,
        **dest = &head;

    for (uint32_t i=0;i<list.count;i++) {
        *dest = new ListNode<ListType>;

        (*dest)->datum = src->datum;

        src = src->next;

        dest = &(*dest)->next;
    }

    count = list.count;
}

//=============================================================================
// LinearList<ListType>::~LinearList()
//  destructor
//

template <typename ListType>
LinearList<ListType>::~LinearList() { clear(); }

//=============================================================================
// uint32_t LinearList<ListType>::size()
//  return size of list (number of nodes)
//

template <typename ListType>
uint32_t LinearList<ListType>::size() { return count; }

//=============================================================================
// bool LinearList<ListType>::isEmpty()
//  return true if list is empty, false otherwise
//

template <typename ListType>
bool LinearList<ListType>::isEmpty() { return count == 0; }

//=============================================================================
// void clear()
//  remove all nodes from list, reset head and count
//
// Notes:
// - uses a counted loop instead of looking for the nullptr. that way, if this
//   is changed to a circular list (which has no null pointers), the code won't
//   be affected.
//

template <typename ListType>
void LinearList<ListType>::clear() {
    ListNode<ListType>
        *tmp;

    for (int i=0;i<count;i++) {
        tmp = head->next;           // remember second node in the list
        delete head;                // remove the first node
        head = tmp;                 // second node moves to front
    }

    head = nullptr;                 // all nodes removed, reset list
    count = 0;
}

//=============================================================================
//------------------------- list-specific operations --------------------------
//=============================================================================

//=============================================================================
// uint32_t LinearList<ListType>::search(const ListType &key)
//  search for key in the list
//
// Parameter:
//  key - what to look for
//
// Returns:
//  - index of first occurrence if key exists in the list
//  - throws a domain_error exception if key is not in the list
//

template <typename ListType>
uint32_t LinearList<ListType>::search(const ListType &key) {
    ListNode<ListType>
        *tmp = head;                // start at head of list

    for (int i=0;i<count;i++) {     // step through the list
        if (key == tmp->datum)      // if key found, stop and return index
            return i;
        tmp = tmp->next;            // walk to next node
    }

    // if we get here, the key was not in the list
    throw std::domain_error("key not found");
}

//=============================================================================
// ListType &LinearList<ListType>::operator[](int32_t pos)
//  return reference to element at given position, works like array []
//
// Parameter:
//  pos - position in the list to access
//
// Returns:
//  - reference to specified element in the list
//  - throws a domain_error exception if pos is invalid (< 0 or >= count)
//

template <typename ListType>
ListType &LinearList<ListType>::operator[](int32_t pos) {
    ListNode<ListType>
        *tmp = head;                // start at head of list

    if (pos < 0 || pos >= count)
        throw std::domain_error("Invalid index");

    for (int i=0;i<pos;i++)         // step through the list
        tmp = tmp->next;            // walk to next node

    return tmp->datum;              // return reference;
}

//=============================================================================
// void LinearList<ListType>::map(void (*fp)(ListType &))
//  apply given function to each element in the list
//
// Parameter:
//  fp - pointer to function to call
//

template <typename ListType>
void LinearList<ListType>::map(void (*fp)(ListType &)) {
    ListNode<ListType>
        *tmp = head;                // start at head of list

    for (int i=0;i<count;i++) {     // step through the list
        (*fp)(tmp->datum);          // call the function, pass the datum
        tmp = tmp->next;            // walk to next node
    }
}

//=============================================================================
// void LinearList<ListType>::insert(uint32_t pos,const ListType &d)
//  insert element into the list
//
// Parameters:
//  pos - position of new element; subsequent elements shifted
//    d - value to be inserted
//
// Notes:
//  - throws a domain_error exception if pos is invalid (> count)
//  - predecessor work is done by pointing to the pointers. this allows all
//    positions in the list to be treated identically, without regard to whether
//    it is the list object or a node pointing to the new node. conceptually
//    the walking process is the same.
//

template <typename ListType>
void LinearList<ListType>::insert(uint32_t pos, const ListType &d) {
    ListNode<ListType>
        *ptr,
        **pred;

    // step 0: validate pos
    if (pos > count)
        throw std::domain_error("Invalid index");

    // step 1: allocate new node and place datum
    ptr = new ListNode<ListType>;
    ptr->datum = d;

    // step 2: find the predecessor
    pred = &head;
    for (int i=0;i<pos;i++)
        pred = &((*pred)->next);

    // step 3: copy predecessor's next pointer to new node
    ptr->next = *pred;

    // step 4: predecessor points to new node
    *pred = ptr;

    // step 5: update count
    count++;
}

//=============================================================================
// void LinearList<ListType>::remove(uint32_t pos)
//  remove element from the list
//
// Parameter:
//  pos - position of element to be removed; subsequent elements shifted
//
// Notes:
//  - throws a domain_error exception if pos is invalid (>= count)
//  - predecessor work is done by pointing to the pointers. this allows all
//    positions in the list to be treated identically, without regard to whether
//    it is the list object or a node pointing to the new node. conceptually
//    the walking process is the same.
//

template <typename ListType>
void LinearList<ListType>::remove(uint32_t pos) {
    ListNode<ListType>
        *ptr,
        **pred;

    // step 0: validate pos
    if (pos >= count)
        throw std::domain_error("Invalid index");

    // step 1: find the predecessor
    pred = &head;
    for (int i=0;i<pos;i++)
        pred = &((*pred)->next);

    // step 2: remember node to be deleted
    ptr = *pred;

    // step 3: copy ntbd's pointer to predecessor... pointing around ntbd
    *pred = ptr->next;

    // step 4: delete the node
    delete ptr;

    // step 5: update count
    count--;
}

#endif //LINEARLIST_LINEARLIST_IMPLEMENTATION_H
