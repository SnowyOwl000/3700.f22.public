//
// Created by bob on 4/23/22.
//
// Note: portions of this code are adapted from Kevin Wayne and Robert Sedgewick's Algorithms 4th ed. book and
//       web site
//

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <cstdint>
#include <stdexcept>
#include <cmath>

#define GET_COUNT(n) (((n) == NULL_INDEX) ? 0 : counts[n])
#define GET_HEIGHT(n) (((n) == NULL_INDEX) ? 0 : heights[n])
#define IS_RED(n) (((n) == NULL_INDEX) ? false : (colors[n] == NODE_RED))
#define REPI(ctr,start,limit) for (uint32_t ctr=(start);(ctr)<(limit);ctr++)

static const uint32_t
    NODE_BLACK = 0,
    NODE_RED = 1,
    NULL_INDEX = 0xffffffff,
    DEFAULT_INIT_CAPACITY = 16;

template <typename KeyType,typename ValueType>
class RedBlackTree {
public:
    explicit RedBlackTree(uint32_t _cap=DEFAULT_INIT_CAPACITY) {

        if (nTrees == 0) {
            left = new uint32_t[_cap];
            right = new uint32_t[_cap];
            counts = new uint32_t[_cap];
            heights = new uint32_t[_cap];

            colors = new uint8_t[_cap];

            keys = new KeyType[_cap];
            values = new ValueType[_cap];

            capacity = _cap;

            REPI(i,0,capacity-1)
                left[i] = i + 1;
            left[capacity-1] = NULL_INDEX;

            freeListHead = 0;
        }

        nTrees++;

        root = NULL_INDEX;
    }

    ~RedBlackTree() {

        nTrees--;

        if (nTrees == 0) {
            delete[] values;
            delete[] keys;
            delete[] colors;
            delete[] heights;
            delete[] counts;
            delete[] right;
            delete[] left;
        } else
            prvClear(root);
    }

    void clear() { prvClear(root); root = NULL_INDEX; }

    uint32_t size() { return GET_COUNT(root); }

    uint32_t height() { return GET_HEIGHT(root); }

    bool isEmpty() { return root == NULL_INDEX; }

    ValueType &search(const KeyType &k) {

        for (uint32_t r=root;r!=NULL_INDEX;) {
            if (k == keys[r])
                return values[r];
            if (k < keys[r])
                r = left[r];
            else
                r = right[r];
        }

        throw std::domain_error("Search: Key not found");
    }

    ValueType &operator[](const KeyType &k) {

        root = prvInsert(root,k);

        colors[root] = NODE_BLACK;

        for (uint32_t r=root;r!=NULL_INDEX;) {
            if (k == keys[r])
                return values[r];
            if (k < keys[r])
                r = left[r];
            else
                r = right[r];
        }

        throw std::domain_error("Search: Key not found");
    }

    void map(void (*fp)(const KeyType &,ValueType &)) {

        prvMap(root,fp);
    }

    void remove(const KeyType &k) {
        uint32_t
            ntbd;

        try {
            search(k);
        } catch (const std::domain_error &e) {
            throw std::domain_error("Remove: Key not found");
        }

        if (!IS_RED(left[root]) && !IS_RED(right[root]))
            colors[root] = NODE_RED;

        root = prvRemove(root,ntbd,k);

        prvFree(ntbd);

        if (root != NULL_INDEX)
            colors[root] = NODE_BLACK;
    }

    void isValidRBTree() {
        uint32_t
            leafDepth = NULL_INDEX;

        if (root == NULL_INDEX)
            return;

        prvIsValid(root,leafDepth,0);

        if (GET_HEIGHT(root) > 2 * ceil(log2(GET_COUNT(root)+1)))
            throw std::logic_error("tree too tall");
    }

private:
    uint32_t prvAllocate() {

        if (freeListHead == NULL_INDEX) {
            auto
                tmpLeft = new uint32_t[2*capacity];
            auto
                tmpRight = new uint32_t[2*capacity];
            auto
                tmpCounts = new uint32_t[2*capacity];
            auto
                tmpHeights = new uint32_t[2*capacity];
            auto
                tmpColors = new uint8_t[2*capacity];
            auto
                tmpKeys = new KeyType[2*capacity];
            auto
                tmpValues = new ValueType[2*capacity];

            REPI(i,0,capacity) {
                tmpLeft[i] = left[i];
                tmpRight[i] = right[i];
                tmpCounts[i] = counts[i];
                tmpHeights[i] = heights[i];
                tmpColors[i] = colors[i];
                tmpKeys[i] = keys[i];
                tmpValues[i] = values[i];
            }

            delete[] values;
            delete[] keys;
            delete[] colors;
            delete[] heights;
            delete[] counts;
            delete[] right;
            delete[] left;

            left = tmpLeft;
            right = tmpRight;
            counts = tmpCounts;
            heights = tmpHeights;
            colors = tmpColors;
            keys = tmpKeys;
            values = tmpValues;

            REPI(i,capacity,2*capacity-1)
                left[i] = i + 1;
            left[2*capacity-1] = NULL_INDEX;

            freeListHead = capacity;

            capacity *= 2;
        }

        uint32_t
            tmp = freeListHead;

        freeListHead = left[freeListHead];

        left[tmp] = right[tmp] = NULL_INDEX;
        counts[tmp] = heights[tmp] = 1;
        colors[tmp] = NODE_RED;

        return tmp;
    }
    void prvFree(uint32_t r) {

        left[r] = freeListHead;
        freeListHead = r;
    }

    void prvClear(uint32_t r) {

        if (r != NULL_INDEX) {
            prvClear(left[r]);
            prvClear(right[r]);

            prvFree(r);
        }
    }

    void prvMap(uint32_t r,void (*fp)(const KeyType &,ValueType &)) {

        if (r != NULL_INDEX) {
            prvMap(left[r],fp);

            (*fp)(keys[r],values[r]);

            prvMap(right[r],fp);
        }
    }

    void prvAdjust(uint32_t r) {
        uint32_t
            lc = GET_COUNT(left[r]),
            rc = GET_COUNT(right[r]),
            lh = GET_HEIGHT(left[r]),
            rh = GET_HEIGHT(right[r]);

        counts[r] = 1 + lc + rc;
        heights[r] = 1 + ((lh > rh) ? lh : rh);
    }

    uint32_t prvRotateLeft(uint32_t r) {
        uint32_t
            s = right[r];

        right[r] = left[s];
        left[s] = r;

        colors[s] = colors[r];
        colors[r] = NODE_RED;

        prvAdjust(r);
        prvAdjust(s);

        return s;
    }

    uint32_t prvRotateRight(uint32_t r) {
        uint32_t
            q = left[r];

        left[r] = right[q];
        right[q] = r;

        colors[q] = colors[r];
        colors[r] = NODE_RED;

        prvAdjust(r);
        prvAdjust(q);

        return q;
    }

    void prvFlipColors(uint32_t r) {

        colors[r] = !colors[r];
        colors[left[r]] = !colors[left[r]];
        colors[right[r]] = !colors[right[r]];
    }

    uint32_t prvBalance(uint32_t r) {

        if (IS_RED(right[r]) && !IS_RED(left[r]))
            r = prvRotateLeft(r);
        if (IS_RED(left[r]) && IS_RED(left[left[r]]))
            r = prvRotateRight(r);
        if (IS_RED(left[r]) && IS_RED(right[r]))
            prvFlipColors(r);

        prvAdjust(r);

        return r;
    }

    uint32_t prvMoveRedLeft(uint32_t r) {

        prvFlipColors(r);
        if (IS_RED(left[right[r]])) {
            right[r] = prvRotateRight(right[r]);
            r = prvRotateLeft(r);
            prvFlipColors(r);
        }

        return r;
    }

    uint32_t prvMoveRedRight(uint32_t r) {

        prvFlipColors(r);
        if (IS_RED(left[left[r]])) {
            r = prvRotateRight(r);
            prvFlipColors(r);
        }

        return r;
    }

    uint32_t prvInsert(uint32_t r,const KeyType &k) {
        uint32_t
            tmp;

        if (r == NULL_INDEX) {
            tmp = prvAllocate();

            keys[tmp] = k;

            return tmp;
        }

        if (k == keys[r])
            return r;

        if (k < keys[r]) {
            // why split these? because left might change inside prvInsert
            // so must guarantee proper order
            tmp = prvInsert(left[r],k);
            left[r] = tmp;
        } else {
            tmp = prvInsert(right[r],k);
            right[r] = tmp;
        }

        return prvBalance(r);
    }

    uint32_t prvRemoveMin(uint32_t r,uint32_t &ntbd) {

        if (left[r] == NULL_INDEX) {
            ntbd = r;

            return NULL_INDEX;
        }

        if (!IS_RED(left[r]) && !IS_RED(left[left[r]]))
            r = prvMoveRedLeft(r);

        left[r] = prvRemoveMin(left[r],ntbd);

        return prvBalance(r);
    }

    uint32_t prvRemove(uint32_t r,uint32_t &ntbd,const KeyType &k) {

        if (k < keys[r]) {
            if (!IS_RED(left[r]) && !IS_RED(left[left[r]]))
                r = prvMoveRedLeft(r);
            left[r] = prvRemove(left[r],ntbd,k);
        } else {
            if (IS_RED(left[r]))
                r = prvRotateRight(r);
            if (k == keys[r] && right[r] == NULL_INDEX) {
                ntbd = r;
                return NULL_INDEX;
            }
            if (!IS_RED(right[r]) && !IS_RED(left[right[r]]))
                r = prvMoveRedRight(r);
            if (k == keys[r]) {
                uint32_t
                    tmp = right[r];

                while (left[tmp] != NULL_INDEX)
                    tmp = left[tmp];

                keys[r] = keys[tmp];
                values[r] = values[tmp];

                right[r] = prvRemoveMin(right[r],ntbd);
            } else
                right[r] = prvRemove(right[r],ntbd,k);
        }

        return prvBalance(r);
    }

    void prvIsValid(uint32_t r,uint32_t &leafDepth,uint32_t curDepth) {

        if (r == NULL_INDEX) {
            if (leafDepth == NULL_INDEX)
                leafDepth = curDepth;
            if (leafDepth != curDepth)
                throw std::logic_error("leaves at different levels " + std::to_string(leafDepth)
                    + " and " + std::to_string(curDepth));

            return;
        }

        if (IS_RED(r) && (IS_RED(left[r])) || IS_RED(right[r]))
            throw std::logic_error("red rule violation");

        if (left[r] != NULL_INDEX && keys[left[r]] >= keys[r])
            throw std::logic_error("left child not less");

        if (right[r] != NULL_INDEX && keys[right[r]] <= keys[r])
            throw std::logic_error("right child not larger");

        prvIsValid(left[r],leafDepth,curDepth+(IS_RED(r) ? 0 : 1));
        prvIsValid(right[r],leafDepth,curDepth+(IS_RED(r) ? 0 : 1));
    }

    uint32_t
        root;

    [[maybe_unused]] static uint32_t
        *left,
        *right,
        *counts,
        *heights,
        nTrees,
        freeListHead,
        capacity;

    static uint8_t
        *colors;

    [[maybe_unused]] static KeyType
        *keys;

    [[maybe_unused]] static ValueType
        *values;
};

template <typename KeyType,typename ValueType>
uint32_t RedBlackTree<KeyType,ValueType>::capacity = 0;

template <typename KeyType,typename ValueType>
uint32_t RedBlackTree<KeyType,ValueType>::freeListHead = 0;

template <typename KeyType,typename ValueType>
uint32_t RedBlackTree<KeyType,ValueType>::nTrees = 0;

template <typename KeyType,typename ValueType>
uint32_t *RedBlackTree<KeyType,ValueType>::left = nullptr;

template <typename KeyType,typename ValueType>
uint32_t *RedBlackTree<KeyType,ValueType>::right = nullptr;

template <typename KeyType,typename ValueType>
uint32_t *RedBlackTree<KeyType,ValueType>::counts = nullptr;

template <typename KeyType,typename ValueType>
uint32_t *RedBlackTree<KeyType,ValueType>::heights = nullptr;

template <typename KeyType,typename ValueType>
uint8_t *RedBlackTree<KeyType,ValueType>::colors = nullptr;

template <typename KeyType,typename ValueType>
KeyType *RedBlackTree<KeyType,ValueType>::keys = nullptr;

template <typename KeyType,typename ValueType>
ValueType *RedBlackTree<KeyType,ValueType>::values = nullptr;

#endif //REDBLACKTREE_H
