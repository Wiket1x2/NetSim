#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"


int main() {


    std::cout << "Hello, World!" << std::endl;
    return 0;
}




//Test package:
//    Package A;
//    ElementID id_ = A.get_id();
//    {
//        Package B;
//        ElementID id1 = B.get_id();
//        std::cout << id1 << std::endl;
//    }
//    Package C;
//    ElementID id2 = C.get_id();
//    std::cout << id_ << id2 << std::endl;

//Test lifo:
//    Package A;
//    Package B;
//    PackageQueue lifo(PackageQueueType::LIFO);
//    lifo.push((Package&&) A);
//    lifo.push((Package&&) B);
//    Package D = lifo.pop();
//    if (D.get_id()==2)
//    std::cout<<"Lifo True"<<std::endl;

//Test fifo:
//    Package A;
//    Package B;
//    PackageQueue lifo(PackageQueueType::LIFO);
//    lifo.push((Package&&) A);
//    lifo.push((Package&&) B);
//    Package D = lifo.pop();
//    if (D.get_id()==1)
//    std::cout<<"Fifo True"<<std::endl;