//
// Created by rwkramer on 10/21/22.
//

#include <stdexcept>
#include "sampler.h"

Sampler::Sampler(uint32_t _nElements) {

    // create space for elements
    elements = new uint32_t[_nElements];

    // fill in the array
    for (uint32_t i=0;i<_nElements;i++)
        elements[i] = i;

    // remember element count
    nElements = _nElements;

    // create random number objects
    rd = new std::random_device;

    mt = new std::mt19937((*rd)());
}

Sampler::~Sampler() {

    // delete array
    delete[] elements;

    // delete random number objects
    delete mt;

    delete rd;
}

uint32_t Sampler::getSample() {
    std::uniform_int_distribution<>
        dis(0,nElements-1);
    uint32_t
        r,e;

    // if no elements, throw an exception
    if (nElements == 0)
        throw std::underflow_error("Sampler is empty");

    // get random number 'r' in range [0,nElements-1]
    r = dis(mt);

    // decrement nElements
    nElements--;

    // remember element in chosen position 'r'
    e = elements[r];

    // copy last element to position 'r'
    elements[r] = elements[nElements];

    // return chosen element
    return e;
}
