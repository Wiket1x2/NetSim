#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"


int main() {

    //Przykładowa funkcjonalność
    std::unique_ptr<PackageQueue> q(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,2,std::move(q));


    Ramp R(1,1);
    R.receiver_preferences_.add_receiver(&W);

    R.deliver_goods(1);
    R.send_package();



    std::cout << "Hello, World!" << std::endl;
    return 0;
}