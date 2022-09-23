//
// Created by rwkramer on 9/23/22.
//

#include "dictionary.h"

static uint32_t hash(const std::string &s) {
    uint32_t
        sum = 0;

    for (uint32_t i=0;i<s.length();i++)
        sum = sum * i + s[i];

    return sum;
}

void Dictionary::clear() {

    for (uint32_t i=0;i<TABLE_SIZE;i++)
        status[i] = UNUSED;

    nItems = 0;
}

void Dictionary::add(const std::string &key,const Fraction value) {
    // hash key to get a position
    uint32_t
        pos = hash(key) % TABLE_SIZE;

    // sequential search for key
    while (status[pos] != UNUSED) {

        // if key found, update value and return
        if (status[pos] == IN_USE && keys[pos] == key) {
            values[pos] = value;
            return;
        }

        pos = (pos + 1) % TABLE_SIZE;
    }

    // not found... is table too full? If so, exception
    if (nItems == MAX_ITEMS)
        throw std::overflow_error("Dictionary is full");

    // not too full... search again stopping at first open spot

    // put key here, increment nItems and done
}

Fraction Dictionary::search(const std::string &key) {
    // hash key to get a position

    // sequential search for key; stop at unused

        // if key found, return value

    // if we get here, key isn't here, throw exception
    throw std::domain_error("Key not found");
}

void Dictionary::remove(const std::string &) {
    // hash key to get a position

    // sequential search for key; stop at unused

        // if key found, set status to deleted and return

    // if we get here, key isn't here, throw exception
    throw std::domain_error("Key not found");

}
