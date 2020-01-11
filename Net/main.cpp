#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"
#include "factory.hpp"


int main() {
    std::list a {1,2,3,4};
    NodeCollection<Ramp> nc;
    nc.find_by_id(5);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
