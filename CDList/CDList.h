//
// Created by Robert Kramer on 4/16/21.
//

#ifndef _CDLIST_H
#define _CDLIST_H

#include <cstdint>
#include <stdexcept>

//============================================================================
//-------------------------------- constants ---------------------------------
//============================================================================

const double
    LIST_DEFAULT_M = 2.0;       // default multiplier when creating more space
const uint32_t
    LIST_DEFAULT_B = 0,         // default count added to capacity
    LIST_DEFAULT_CAP = 64;      // default initial capacity

//============================================================================
// CDList<ListType>
//  Circular doubly-linked list using a shared data pool
//

template<typename ListType>
class CDList {
public:
    //------------------------------------------------------------------------
    // explicit CDList(uint32_t _cap=LIST_DEFAULT_CAP,double _m=LIST_DEFAULT_M,
    //                 uint32_t _b=LIST_DEFAULT_B)
    //  Constructor
    //
    // Parameters:
    // _cap - initial capacity of shared data pool
    // _m   - multiplier used when creating additional space
    // _b   - additional space added when creating more space
    //

    explicit CDList(uint32_t _cap=LIST_DEFAULT_CAP,double _m=LIST_DEFAULT_M,
                    uint32_t _b=LIST_DEFAULT_B) {

        // if this is the first list, set up space
        if (nLists == 0) {
            // allocate space
            data = new ListType[_cap];
            nextLinks = new uint32_t[_cap];
            prevLinks = new uint32_t[_cap];

            // set up free list
            freeListHead = 0;                   // free list head "points" to
                                                // first node
            for (uint32_t i=0;i<_cap-1;i++)     // nodes point to next node
                nextLinks[i] = i + 1;
            nextLinks[_cap-1] = 0xffffffff;     // last node points to "null"

            // remember parameters
            capacity = _cap;
            m = _m;
            b = _b;
        }

        // one more list
        nLists++;

        // normal list initialization
        head = curNode = 0xffffffff;
        count = 0;
    }

    //------------------------------------------------------------------------
    // ~CDList()
    //  Destructor
    //

    ~CDList() {

        // call clear to add nodes back to free list
        clear();

        // one less list
        nLists--;

        // if this was last list, deallocate space
        if (nLists == 0) {
            delete[] prevLinks;
            delete[] nextLinks;
            delete[] data;
        }
    }

    //------------------------------------------------------------------------
    // void clear()
    //  Clear the list, reset it for future use
    //

    void clear() {

        // only do this if the list exists. if list is empty, finding last
        // node will segfault
        if (count > 0) {
            uint32_t
                last = prevLinks[head];

            // connect end of list to front of free list
            nextLinks[last] = freeListHead;

            // adjust front of free list
            freeListHead = head;

            // reset head and count
            head = curNode = 0xffffffff;
            count = 0;
        }
    }

    //------------------------------------------------------------------------
    // uint32_t size()
    //  return size of list
    //

    uint32_t size() { return count; }

    //------------------------------------------------------------------------
    // bool isEmpty()
    //  returns true if list is empty, false if list is not empty
    //

    bool isEmpty() { return count == 0; }

    //------------------------------------------------------------------------
    // uint32_t search(const ListType &key)
    //  Search for key
    //
    // Parameter:
    // key - what to search for in list
    //
    // Returns:
    // position (within the list) of first occurrence of key
    //
    // Throws:
    // domain_error if key not found
    //

    uint32_t search(const ListType &key) {
        uint32_t
            pos = head;

        // standard sequential search
        for (uint32_t i=0;i<count;i++) {
            if (data[pos] == key)
                return i;

            // same as "pos = pos->next" in array form
            pos = nextLinks[pos];
        }

        throw std::domain_error("CDList: Key not found");
    }

    //------------------------------------------------------------------------
    // ListType &operator[](int32_t index)
    //  array indexing operator
    //
    // Parameter:
    // index - position within list to access
    //
    // Returns:
    // reference to data at the given position; allows use on LHS
    //
    // Notes:
    // - Negative indexing is supported
    // - Bounds must be -count <= index < count
    // - domain_error thrown if index not in bounds
    //

    ListType &operator[](int32_t index) {
        uint32_t
            pos = head;

        // make sure index is in bounds
        if ((index < 0 && -index > count) || index >= count)
            throw std::domain_error("CDList: Invalid index");

        // move in the proper direction
        if (index < 0)
            for (uint32_t i=0;i<-index;i++)
                pos = prevLinks[pos];
        else
            for (uint32_t i=0;i<index;i++)
                pos = nextLinks[pos];

        return data[pos];
    }

    //------------------------------------------------------------------------
    // void map(void (*fp)(ListType &))
    //  Apply function to each element in list
    //
    // Parameter:
    // fp - pointer to function that takes a reference to a ListType, returns nothing
    //
    // Note:
    // Function is passed element by reference to allow function to modify list data
    //

    void map(void (*fp)(ListType &)) {
        uint32_t
            pos = head;

        // step through the list
        for (uint32_t i=0;i<count;i++) {
            // call the function, pass it one value from the list
            (*fp)(data[pos]);

            // same as "pos = pos->next" in array form
            pos = nextLinks[pos];
        }
    }

    //------------------------------------------------------------------------
    // void insert(uint32_t pos,const ListType &val)
    //  Insert element into list
    //
    // Parameters:
    // pos - position in which to insert element
    // val - datum to be inserted
    //
    // Notes:
    // - 0 <= pos <= count; domain_error thrown if not
    // - If out of space and can't get more, runtime_error is thrown
    //

    void insert(uint32_t pos,const ListType &val) {
        uint32_t
            tmp;

        // make sure pos is valid
        if (pos > count)
            throw std::domain_error("CDList: Invalid index");

        // if no space is left, get some more
        if (freeListHead == 0xffffffff) {
            // calculate new capacity
            uint32_t
                tmpCap = capacity * m + b;

            // if we can't expand the list, thrown an exception
            if (tmpCap <= capacity)
                throw std::runtime_error("CDList: No more space");

            // get more space
            auto
                tmpData = new ListType[tmpCap];
            auto
                tmpNext = new uint32_t[tmpCap];
            auto
                tmpPrev = new uint32_t[tmpCap];

            // copy old space into new
            for (uint32_t i=0;i<capacity;i++) {
                tmpData[i] = data[i];
                tmpNext[i] = nextLinks[i];
                tmpPrev[i] = prevLinks[i];
            }

            // regenerate free list
            freeListHead = capacity;
            for (uint32_t i=capacity;i<tmpCap-1;i++)
                tmpNext[i] = i + 1;
            tmpNext[tmpCap-1] = 0xffffffff;

            // delete old space
            delete[] prevLinks;
            delete[] nextLinks;
            delete[] data;

            // point to new space
            data = tmpData;
            nextLinks = tmpNext;
            prevLinks = tmpPrev;

            // remember new capacity;
            capacity = tmpCap;
        }

        // get node from free list
        tmp = freeListHead;
        freeListHead = nextLinks[tmp];

        data[tmp] = val;

        // special case... if list is empty, result is just this node
        if (count == 0) {
            // node points to itself... twice
            nextLinks[tmp] = prevLinks[tmp] = tmp;

            // head points to new node
            head = tmp;
        } else {
            uint32_t
                pred=prevLinks[head],
                succ;

            // find predecessor
            for (uint32_t i=0;i<pos;i++)
                pred = nextLinks[pred];

            // remember successor
            succ = nextLinks[pred];

            // copy pointers into new node
            nextLinks[tmp] = succ;
            prevLinks[tmp] = pred;

            // pred and succ point to new node
            nextLinks[pred] = prevLinks[succ] = tmp;
        }

        // update count
        count++;
    }

    //------------------------------------------------------------------------
    // void remove(uint32_t pos)
    //  Remove element from list
    //
    // Parameter
    // pos - position of element to be removed
    //
    // Notes:
    // - 0 <= pos < count; otherwise, domain_error is thrown
    // - resets current node to 0xffffffff if current node is deleted
    //

    void remove(uint32_t pos) {
        uint32_t
            ntbd;

        // make sure pos is valid
        if (pos >= count)
            throw std::domain_error("CDList: Invalid index");

        // special case... if list will be empty
        if (count == 1) {
            // remember ntbd
            ntbd = head;

            head = curNode = 0xffffffff;
        } else {
            uint32_t
                pred=prevLinks[head],
                succ;

            // find predecessor
            for (uint32_t i=0;i<pos;i++)
                pred = nextLinks[pred];

            // remember node to be deleted
            ntbd = nextLinks[pred];

            // if ntbd is current node, reset curNode
            if (ntbd == curNode)
                curNode = 0xffffffff;

            // if ntbd is head node, move head to next position
            if (ntbd == head)
                head = nextLinks[head];

            // remember successor
            succ = nextLinks[ntbd];

            // pred and succ point to each other
            nextLinks[pred] = succ;
            prevLinks[succ] = pred;
        }

        // add ntbd back to free list
        nextLinks[ntbd] = freeListHead;
        freeListHead = ntbd;

        // update count
        count--;
    }

    //------------------------------------------------------------------------
    // ListType &cur()
    //  Return current node
    //
    // Notes:
    // - returns reference to allow changing the value in the list
    // - throws runtime_error if no current node set
    //

    ListType &cur() {

        if (curNode == 0xffffffff)
            throw std::runtime_error("CDList: No current node");

        return data[curNode];
    }

    //------------------------------------------------------------------------
    // ListType &next()
    //  Moves current node to next node in list and returns it
    //
    // Notes:
    // - returns reference to allow changing the value in the list
    // - throws runtime_error if no current node set
    //

    ListType &next() {

        if (curNode == 0xffffffff)
            throw std::runtime_error("CDList: No current node");

        curNode = nextLinks[curNode];

        return data[curNode];
    }

    //------------------------------------------------------------------------
    // ListType &prev()
    //  Moves current node to previous node in list and returns it
    //
    // Notes:
    // - returns reference to allow changing the value in the list
    // - throws runtime_error if no current node set
    //

    ListType &prev() {

        if (curNode == 0xffffffff)
            throw std::runtime_error("CDList: No current node");

        curNode = prevLinks[curNode];

        return data[curNode];
    }

    //------------------------------------------------------------------------
    // ListType &first()
    //  Moves current node to first node in list and returns it
    //
    // Notes:
    // - returns reference to allow changing the value in the list
    // - throws runtime_error if list is empty
    //

    ListType &first() {

        if (head == 0xffffffff)
            throw std::runtime_error("CDList: Empty list");

        curNode = head;

        return data[curNode];
    }

    //------------------------------------------------------------------------
    // ListType &last()
    //  Moves current node to last node in list and returns it
    //
    // Notes:
    // - returns reference to allow changing the value in the list
    // - throws runtime_error if list is empty
    //

    ListType &last() {

        if (head == 0xffffffff)
            throw std::runtime_error("CDList: Empty list");

        curNode = prevLinks[head];

        return data[curNode];
    }

    //------------------------------------------------------------------------
    // bool isFirst()
    //  Returns true if current node is first node in list, false otherwise
    //
    // Notes:
    // - returns false if list is empty
    //

    bool isFirst() {

        return head != 0xffffffff && curNode == head;
    }

    //------------------------------------------------------------------------
    // bool isLast()
    //  Returns true if current node is last node in list, false otherwise
    //
    // Notes:
    // - returns false if list is empty
    //

    bool isLast() {

        return head != 0xffffffff && curNode == prevLinks[head];
    }

private:
    // per-list stuff
    uint32_t
        head,               // index of first node within data pool
        count,              // number of items in this list
        curNode;            // index of current node within data pool

    // shared variables
    [[maybe_unused]] static ListType
        *data;                          // pointer to data array
    [[maybe_unused]] static uint32_t
        *nextLinks,                     // pointer to next index array
        *prevLinks,                     // pointer to previous index array
        nLists,                         // number of lists
        capacity,                       // size of shared arrays
        b,                              // value added to capacity
        freeListHead;                   // index of first node in free list
    [[maybe_unused]] static double
        m;                              // capacity multiplier
};

//============================================================================
//------------------------------ initial values ------------------------------
//============================================================================

template <typename ListType>
[[maybe_unused]] ListType *CDList<ListType>::data = nullptr;

template <typename ListType>
[[maybe_unused]] uint32_t *CDList<ListType>::nextLinks = nullptr;

template <typename ListType>
[[maybe_unused]] uint32_t *CDList<ListType>::prevLinks = nullptr;

template <typename ListType>
[[maybe_unused]] uint32_t CDList<ListType>::nLists = 0;

template <typename ListType>
[[maybe_unused]] uint32_t CDList<ListType>::capacity = 0;

template <typename ListType>
[[maybe_unused]] uint32_t CDList<ListType>::freeListHead = 0;

template <typename ListType>
[[maybe_unused]] uint32_t CDList<ListType>::b = 0;

template <typename ListType>
[[maybe_unused]] double CDList<ListType>::m = 0.0;

#endif //_CDLIST_H
