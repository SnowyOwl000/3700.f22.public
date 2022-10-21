#include <iostream>
#include "mrs.h"
#include "sampler.h"

using namespace std;

const uint32_t
    RADICES[] = {7,24,60,60};
const uint32_t
    N_RADICES = sizeof(RADICES) / sizeof(RADICES[0]),
    N_SECONDS = 7 * 24 * 60 * 60;

#define OPF(f) (f ? "pass" : "fail")

int main() {
    MRSCodec
        codec(N_RADICES,RADICES);
    Sampler
        samples(N_SECONDS);
    bool
        flags[7][24][60][60] = {false},
        okay;
    uint32_t
        decoded[N_RADICES],
        s;

    // loop over every second in the week
    for (uint32_t i=0;i<N_SECONDS;i++) {
        // get a sample
        s = samples.getSample();

        // convert s to mixed radix form
        codec.decode(s,decoded);

        // set flag in array
        flags[decoded[0]][decoded[1]][decoded[2]][decoded[3]] = true;
    }

    // test... sampler should be empty here
    okay = false;
    try {
        s = samples.getSample();
    } catch (std::underflow_error &e) {
        okay = true;
    }

    cout << "Sampler empty: " << OPF(okay) << endl;

    // test to see that all flags are set
    okay = true;
    for (uint32_t d=0;d<7;d++)
        for (uint32_t h=0;h<24;h++)
            for (uint32_t m=0;m<60;m++)
                for (s=0;s<60;s++)
                    okay = okay && flags[d][h][m][s];

    cout << "Sampler and codec: " << OPF(okay) << endl;

    // create codec
    return 0;
}