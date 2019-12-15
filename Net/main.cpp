#include <iostream>
#include "package.hpp"


int main() {


    std::cout << "Hello, World!" << std::endl;
    return 0;
}




//Test:
//    Package A;
//    ElementID id = A.get_id();
//    {
//        Package B;
//        ElementID id1 = B.get_id();
//        std::cout << id1 << std::endl;
//    }
//    Package C;
//    ElementID id2 = C.get_id();
//    std::cout << id << id2 << std::endl;