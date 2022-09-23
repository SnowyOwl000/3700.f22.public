#include <iostream>
#include "dictionary.h"

int main() {
    Dictionary
        vars;
    Fraction
        f(2,3);

    try {
        vars.add("foo", f);
    }
    catch (const std::overflow_error &e) {
        std::cout << "Caught an overflow" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
    }
    catch (const std::domain_error &e) {
        std::cout << "Caught a domain error, which shouldn't happen"
            << std::endl;
    }
    catch (...) {
        std::cout << "Caught some generic exception" << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}