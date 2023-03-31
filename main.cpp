#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"

int main() {

    {
        Package p1;
    }
    Package p2;
    std::cout << p2.get_id();

    return EXIT_SUCCESS;
}


