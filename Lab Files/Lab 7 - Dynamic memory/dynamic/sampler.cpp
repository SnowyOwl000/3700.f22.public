//
// Created by rwkramer on 10/21/22.
//

#include "sampler.h"

Sampler::Sampler(uint32_t _nElements) {

    // create space for elements
    elements = new uint32_t[_nElements];
    nElements = _nElements;

    // fill in the array
    for (uint32_t i=0;i<nElements;i++)
        elements[i] = i;

    // remember element count

    // create random number objects
}

Sampler::~Sampler() {

    // delete array

    // delete random number objects

}

uint32_t Sampler::getSample() {

    // if no elements, throw an exception

    // get random number 'r' in range [0,nElements-1]

    // decrement nElements

    // remember element in chosen position 'r'

    // copy last element to position 'r'

    // return chosen element
}
