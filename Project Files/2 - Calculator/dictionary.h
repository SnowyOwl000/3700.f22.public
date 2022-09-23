//
// Created by rwkramer on 9/23/22.
//

#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include "fraction.h"

enum DictionaryStatus {
    UNUSED,
    IN_USE,
    DELETED
};

const uint32_t
    TABLE_SIZE = 127,
    MAX_ITEMS = 101;

class Dictionary {
public:
    Dictionary() { clear(); } // NOLINT(cppcoreguidelines-pro-type-member-init)
    ~Dictionary() = default;

    bool isEmpty() { return nItems == 0; }
    uint32_t size() { return nItems; }

    void clear();

    void add(const std::string &,const Fraction);

    Fraction search(const std::string &);

    void remove(const std::string &);

private:
    std::string
        keys[TABLE_SIZE];
    Fraction
        values[TABLE_SIZE];
    DictionaryStatus
        status[TABLE_SIZE];
    uint32_t
        nItems;

};


#endif //_DICTIONARY_H
