#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"

//double pg() { return 0.6; }

int main() {


    std::cout << "Hello, World!" << std::endl;
    return 0;
}

//ReceiverPreferences pref(pg);
//
//std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
//Worker A(1,1,std::move(a), pref);
//std::unique_ptr<PackageQueue> b(new PackageQueue(PackageQueueType::LIFO));
//Worker B(2,2,std::move(a), pref);
//std::unique_ptr<PackageQueue> c(new PackageQueue(PackageQueueType::LIFO));
//Worker C(3,3,std::move(a), pref);
//std::unique_ptr<PackageQueue> d(new PackageQueue(PackageQueueType::LIFO));
//Worker D(4,4,std::move(a), pref);
//
//Ramp R(1,1, pref);
//
// //Dodawanie
//R.receiver_preferences_.add_receiver(&A);
//IPackageReceiver* reciever1 = R.receiver_preferences_.choose_receiver();
//std::cout << reciever1->get_id() << std::endl;
//R.receiver_preferences_.add_receiver(&B);
//IPackageReceiver* reciever2 = R.receiver_preferences_.choose_receiver();
//std::cout << reciever2->get_id() << std::endl;;
//R.receiver_preferences_.add_receiver(&C);
//IPackageReceiver* reciever3 = R.receiver_preferences_.choose_receiver();
//std::cout << reciever3->get_id()  << std::endl;;
//R.receiver_preferences_.add_receiver(&D);
//IPackageReceiver* reciever4 = R.receiver_preferences_.choose_receiver();
//std::cout << reciever4->get_id()  << std::endl;;
// //Usuwanie
//    R.receiver_preferences_.remove_receiver(&B);
//    std::cout << R.receiver_preferences_.choose_receiver()->get_id() << std::endl;
//    R.receiver_preferences_.remove_receiver(&C);
//    std::cout << R.receiver_preferences_.choose_receiver()->get_id() << std::endl;
//    R.receiver_preferences_.remove_receiver(&D);
//    std::cout << R.receiver_preferences_.choose_receiver()->get_id() << std::endl;