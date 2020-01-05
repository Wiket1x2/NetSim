//
// Created by Aayli on 03.01.2020.
//

#include "gtest/gtest.h"

#include "nodes.hpp"

double pg() { return 0.6; }

//test nie działa właściwie na mapie.
/*TEST(ReciverTest, sendingPackage) {
    ReceiverPreferences receiver(pg);

    std::unique_ptr<IPackageStockpile> s1(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S1(1,std::move(s1));
    std::unique_ptr<IPackageStockpile> s2(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S2(1,std::move(s1));
    std::unique_ptr<IPackageStockpile> s3(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S3(1,std::move(s1));
    std::unique_ptr<IPackageStockpile> s4(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S4(1,std::move(s1));

    //Dodawanie
    receiver.add_receiver(&S1);
    ASSERT_EQ(receiver.choose_receiver(), &S1);
    receiver.add_receiver(&S2);
    ASSERT_EQ(receiver.choose_receiver(), &S2);
    receiver.add_receiver(&S3);
    ASSERT_EQ(receiver.choose_receiver(), &S2);
    receiver.add_receiver(&S4);
    ASSERT_EQ(receiver.choose_receiver(), &S3);
    //Usuwanie
    receiver.remove_receiver(&S1);
    ASSERT_EQ(receiver.choose_receiver(), &S3);
    receiver.remove_receiver(&S2);
    ASSERT_EQ(receiver.choose_receiver(), &S4);
    receiver.remove_receiver(&S3);
    ASSERT_EQ(receiver.choose_receiver(), &S4);
}*/

TEST(PackageSenderTest, bufor) {
    ReceiverPreferences pref(pg);
    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,1,std::move(a), pref);

    Ramp R(1,1, pref);

    R.receiver_preferences_.add_receiver(&W);
    R.deliver_goods(1);
    ASSERT_EQ(R.is_sending_buffer(), true);
    R.send_package();
    ASSERT_EQ(R.is_sending_buffer(), false);
}

TEST(RampTest, deliver_goods) {
    ReceiverPreferences pref(pg);

    Ramp R1(1,1, pref);
    Ramp R2(2,2, pref);
    ASSERT_EQ(R1.is_sending_buffer(), false);
    ASSERT_EQ(R2.is_sending_buffer(), false);

    R1.deliver_goods(1);
    R2.deliver_goods(1);
    ASSERT_EQ(R1.is_sending_buffer(), true);
    ASSERT_EQ(R2.is_sending_buffer(), false);

    R2.deliver_goods(2);
    ASSERT_EQ(R2.is_sending_buffer(), true);
}

TEST(ReciveTest, send_and_recive) {
    ReceiverPreferences pref(pg);

    std::unique_ptr<IPackageStockpile> s(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S(1,std::move(s));

    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,1,std::move(a), pref);
    W.receiver_preferences_.add_receiver(&S);
    ASSERT_EQ(W.get_queue_size(), 0);

    Ramp R(1,1, pref);
    R.receiver_preferences_.add_receiver(&W);
    ASSERT_EQ(S.get_queue_size(), 0);


    R.deliver_goods(1);
    R.send_package();
    ASSERT_EQ(W.get_queue_size(), 1);

    W.do_work(1);
    W.send_package();
    ASSERT_EQ(W.get_queue_size(), 0);
    ASSERT_EQ(S.get_queue_size(), 1);
}

TEST(WorkerTest, do_work) {
    ReceiverPreferences pref(pg);

    std::unique_ptr<IPackageStockpile> s(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S(1,std::move(s));

    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,3,std::move(a), pref);
    W.receiver_preferences_.add_receiver(&S);

    //pierwsza tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(1);
    ASSERT_EQ(W.get_queue_size(), 0);

    //druga tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(2);
    ASSERT_EQ(W.get_queue_size(), 1);

    //trzecia tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(3);
    ASSERT_EQ(W.get_queue_size(), 2);

    //czwarta tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(4);
    ASSERT_EQ(W.get_queue_size(), 2);
}
