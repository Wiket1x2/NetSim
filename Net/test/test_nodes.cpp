//
// Created by Aayli on 03.01.2020.
//

#include "gtest/gtest.h"

#include "nodes.hpp"

double pg() { return 0.6; }


TEST(ReciverTest, sendingPackage) {
    ReceiverPreferences receiver(pg);

    std::unique_ptr<IPackageStockpile> s1(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S1(1,std::move(s1));
    std::unique_ptr<IPackageStockpile> s2(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S2(2,std::move(s2));
    std::unique_ptr<IPackageStockpile> s3(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S3(3,std::move(s3));
    std::unique_ptr<IPackageStockpile> s4(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S4(4,std::move(s4));

    //Dodawanie
    receiver.add_receiver(&S1);
    ASSERT_EQ(receiver.choose_receiver(), &S1);
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0);
    receiver.add_receiver(&S2);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0/2);
    receiver.add_receiver(&S3);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0/3);
    receiver.add_receiver(&S4);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
   ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0/4);
    //Usuwanie
    receiver.remove_receiver(&S1);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0/3);
    receiver.remove_receiver(&S2);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0/2);
    receiver.remove_receiver(&S3);
    ASSERT_EQ(receiver.choose_receiver(), receiver.choose_receiver());
    ASSERT_EQ(receiver.get_preferences().begin()->second, 1.0);
}

TEST(PackageSenderTest, bufor) {
    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,1,std::move(a));

    Ramp R(1,1);

    R.receiver_preferences_.add_receiver(&W);
    R.deliver_goods(1);
    ASSERT_EQ(R.get_sending_buffer()->get_id(), 1);
    R.send_package();
    ASSERT_EQ(R.get_sending_buffer().has_value(), false);
}

TEST(RampTest, deliver_goods) {
    Ramp R1(1,1);
    Ramp R2(2,2);
    ASSERT_EQ(R1.get_sending_buffer().has_value(), false);
    ASSERT_EQ(R2.get_sending_buffer().has_value(), false);

    R1.deliver_goods(1);
    R2.deliver_goods(1);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 1);
    ASSERT_EQ(R2.get_sending_buffer().has_value(), false);

    R2.deliver_goods(2);
    ASSERT_EQ(R2.get_sending_buffer()->get_id(), 2);
}

TEST(ReciveTest, send_and_recive) {
    std::unique_ptr<IPackageStockpile> s(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S(1,std::move(s));

    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,1,std::move(a));

    W.receiver_preferences_.add_receiver(&S);
    ASSERT_EQ(W.begin(), W.end());

    Ramp R(1,1);
    R.receiver_preferences_.add_receiver(&W);
    ASSERT_EQ(S.begin(), S.end());


    R.deliver_goods(1);
    R.send_package();
    ASSERT_EQ(++W.begin(), W.end());

    W.do_work(1);
    W.send_package();
    ASSERT_EQ(W.begin(), W.end());
    ASSERT_EQ(++S.begin(), S.end());
}

TEST(WorkerTest, do_work) {
    std::unique_ptr<IPackageStockpile> s(new PackageQueue(PackageQueueType::LIFO));
    Storehouse S(1,std::move(s));

    std::unique_ptr<PackageQueue> a(new PackageQueue(PackageQueueType::LIFO));
    Worker W(1,3,std::move(a));
    W.receiver_preferences_.add_receiver(&S);

    //pierwsza tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(1);
    ASSERT_EQ(W.begin(), W.end());

    //druga tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(2);
    ASSERT_EQ(++W.begin(), W.end());

    //trzecia tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(3);
    ASSERT_EQ(++W.begin(), --W.end());

    //czwarta tura
    W.receive_package(Package());
    W.send_package();
    W.do_work(4);
    ASSERT_EQ(++W.begin(), --W.end());
}
