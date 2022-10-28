//
// Created by rwkramer on 10/21/22.
//

#ifndef _SAMPLER_H
#define _SAMPLER_H


#include <cstdint>
#include <random>

class Sampler {
public:
    Sampler(uint32_t _nElements);
    ~Sampler();

    uint32_t getSample();
private:
    uint32_t
        nElements,
        *elements;

    std::random_device
        *rd;
    std::mt19937
        *mt;
};


#endif //_SAMPLER_H
