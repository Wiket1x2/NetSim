//
// Created by Aayli on 03.01.2020.
//

#include "gtest/gtest.h"
#include "nodes.hpp"
#include "package.hpp"
#include <iostream>


double pg() { return 0.6; }

TEST(ReciverTest, scaling_probability) {
    ReceiverPreferences receiver(pg);

    Storehouse S1(1);
    Storehouse S2(2);
    Storehouse S3(3);
    Storehouse S4(4);

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
    Worker W(1,1);
    Ramp R(1,1);

    R.receiver_preferences_.add_receiver(&W);
    R.deliver_goods(1);
    ASSERT_EQ(R.get_sending_buffer()->get_id(), 1);
    R.send_package();
    ASSERT_EQ(R.get_sending_buffer().has_value(), false);
}

TEST(RampTest, deliver_goods) {
    Storehouse S(1);
    Ramp R1(1,1);
    Ramp R2(2,2);
    R1.receiver_preferences_.add_receiver(&S);
    R2.receiver_preferences_.add_receiver(&S);
    ASSERT_EQ(R1.get_sending_buffer().has_value(), false);
    ASSERT_EQ(R2.get_sending_buffer().has_value(), false);

    R1.deliver_goods(1);
    R2.deliver_goods(1);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 1);
    ASSERT_EQ(R2.get_sending_buffer()->get_id(), 2);
    R1.send_package();
    R2.send_package();

    R1.deliver_goods(2);
    R2.deliver_goods(2);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 3);
    ASSERT_EQ(R2.get_sending_buffer().has_value(), false);
    R1.send_package();
    R2.send_package();

    R1.deliver_goods(3);
    R2.deliver_goods(3);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 4);
    ASSERT_EQ(R2.get_sending_buffer()->get_id(), 5);
}

TEST(ReciveTest, send_and_recive) {
    Storehouse S(1);

    Worker W(1,1);
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
    Storehouse S(1);
    Worker W(1,3);
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

TEST(NodesTest, small_simulation) {
    Ramp R1 (1,1);
    Ramp R2 (2,2);
    Worker W1 (1,2);
    Worker W2 (2, 1);
    Storehouse S(1);

    R1.receiver_preferences_.add_receiver(&W1);
    R2.receiver_preferences_.add_receiver(&W2);
    W1.receiver_preferences_.add_receiver(&W2);
    W2.receiver_preferences_.add_receiver(&S);

    //pierwsza tura
    R1.deliver_goods(1);
    R2.deliver_goods(1);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 1);
    ASSERT_EQ(R2.get_sending_buffer()->get_id(), 2);
    R1.send_package();
    R2.send_package();
    W1.send_package();
    W2.send_package();
    ASSERT_EQ(W1.cbegin()->get_id(),1);
    ASSERT_EQ(W2.cbegin()->get_id(), 2);
    W1.do_work(1);
    W2.do_work(1);
    ASSERT_EQ(W1.get_sending_buffer().has_value(), false);
    ASSERT_EQ(W2.get_sending_buffer()->get_id(), 2);

    R2.receiver_preferences_.remove_receiver(&W2);
    R2.receiver_preferences_.add_receiver(&W1);

    //druga tura
    R1.deliver_goods(2);
    R2.deliver_goods(2);
    ASSERT_EQ(R1.get_sending_buffer()->get_id(), 3);
    ASSERT_EQ(R2.get_sending_buffer().has_value(), false);
    R1.send_package();
    R2.send_package();
    W1.send_package();
    W2.send_package();
    ASSERT_EQ(W1.cbegin()->get_id(),3);
    ASSERT_EQ(W1.get_sending_buffer().has_value(), false);
    ASSERT_EQ(S.cbegin()->get_id(), 2);
    W1.do_work(2);
    W2.do_work(2);
    ASSERT_EQ(W1.get_sending_buffer().has_value(), true);
}