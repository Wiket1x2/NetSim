//
// Created by Aayli on 03.01.2020.
//
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "nodes.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"

#include "nodes_mocks.hpp"
#include "global_functions_mock.hpp"

#include <iostream>

using ::std::cout;
using ::std::endl;


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

// Kueczek
// -----------------

TEST(WorkerTest, HasBuffer) {
    // Test scenariusza opisanego na stronie:
    // http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes#bufor_aktualnie_przetwarzanego_polproduktu

    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    // FIXME: poprawić w docelowej wersji (dodać konstruktor z ID półproduktu)
//    w.receive_package(Package(1));
//    w.do_work(t);
//    ++t;
//    w.receive_package(Package(2));
//    w.do_work(t);
//    auto& buffer = w.get_sending_buffer();
    //
    w.receive_package(Package());
    w.do_work(t);
    ++t;
    w.receive_package(Package());
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();


    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

// -----------------

TEST(RampTest, IsDeliveryOnTime) {

    Ramp r(1, 2);
    // FIXME: poprawić w docelowej wersji (konstruktor powinien posiadać argument domyślny)
//    auto recv = std::make_unique<Storehouse>(1);
    auto recv = std::make_unique<Storehouse>(1, std::make_unique<PackageQueue>(PackageQueueType::LIFO));

    r.receiver_preferences_.add_receiver(recv.get());

    r.deliver_goods(1);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
    r.send_package();

    r.deliver_goods(2);
    ASSERT_FALSE(r.get_sending_buffer().has_value());

    r.deliver_goods(3);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
}

// -----------------

TEST(ReceiverPreferencesTest, AddReceiversRescalesProbability) {
    // Upewnij się, że dodanie odbiorcy spowoduje przeskalowanie prawdopodobieństw.
    ReceiverPreferences rp;

    MockReceiver r1;
    rp.add_receiver(&r1);
    ASSERT_NE(rp.get_preferences().find(&r1), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);

    MockReceiver r2;
    rp.add_receiver(&r2);
    EXPECT_EQ(rp.get_preferences().at(&r1), 0.5);
    ASSERT_NE(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r2), 0.5);
}

TEST(ReceiverPreferencesTest, RemoveReceiversRescalesProbability) {
    // Upewnij się, że usunięcie odbiorcy spowoduje przeskalowanie pozostałych prawdopodobieństw.
    ReceiverPreferences rp;

    MockReceiver r1, r2;
    rp.add_receiver(&r1);
    rp.add_receiver(&r2);

    rp.remove_receiver(&r2);
    ASSERT_EQ(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);
}

// Przydatny alias, żeby zamiast pisać `::testing::Return(...)` móc pisać
// samo `Return(...)`.
using ::testing::Return;


// FIXME: odkomentować po wyjaśnieniu sytuacji z domyślnym generatorem prawdopodobieństwa
//TEST_F(ReceiverPreferencesChoosingTest, ChooseReceiver) {
//    // Upewnij się, że odbiorcy wybierani są z właściwym prawdopodobieństwem.
//
//    EXPECT_CALL(global_functions_mock, generate_canonical()).WillOnce(Return(0.3)).WillOnce(Return(0.7));
//
//    ReceiverPreferences rp;
//
//    MockReceiver r1, r2;
//    rp.add_receiver(&r1);
//    rp.add_receiver(&r2);
//
//    if (rp.begin()->first == &r1) {
//        EXPECT_EQ(rp.choose_receiver(), &r1);
//        EXPECT_EQ(rp.choose_receiver(), &r2);
//    } else {
//        EXPECT_EQ(rp.choose_receiver(), &r2);
//        EXPECT_EQ(rp.choose_receiver(), &r1);
//    }
//}

// -----------------

using ::testing::Return;
using ::testing::_;

// Ponieważ `IPackageStockpile::const_iterator` to iterator na (niestandardowy)
// typ Package, który nie przeciąża operatora <<, Google Mock nie ma pojęcia
// w jaki sposób wypisać iterator w postaci tekstowej (w przypadku nie
// spełnienia asercji - w ramach komunikatu diagnostycznego).
// Aby rozwiązać ten problem, możemy ręcznie zdefiniować funkcję wywoływaną
// w sytuacji, gdy zachodzi potrzeba wypisania obiektu niestandardowego typu
// - w tym przypadku `IPackageStockpile::const_iterator`.
// zob. https://github.com/google/googlemock/blob/master/googlemock/docs/v1_5/CookBook.md#teaching-google-mock-how-to-print-your-values
void PrintTo(const IPackageStockpile::const_iterator& it, ::std::ostream* os) {
    *os << it->get_id();
}

class PackageSenderFixture : public PackageSender {
    // Nie sposób w teście wykorzystać prywetnej metody `PackageSender::push_package()`,
    // dlatego do celów testowych stworzona została implementacja zawierająca
    // metodę `push_package()` w sekcji publicznej.
public:
    void push_package(Package&& package) { PackageSender::push_package(std::move(package)); }
};


TEST(PackageSenderTest, SendPackage) {
    MockReceiver mock_receiver;
    // Oczekujemy, że metoda `receive_package()` obiektu `mock_receiver` zostanie
    // wywołana dwukrotnie, z dowolnym argumentem (symbol `_`).
    EXPECT_CALL(mock_receiver, receive_package(_)).Times(1);

    PackageSenderFixture sender;
    sender.receiver_preferences_.add_receiver(&mock_receiver);
    // Zwróć uwagę, że poniższa instrukcja korzysta z semantyki referencji do r-wartości.
    sender.push_package(Package());

    sender.send_package();

    EXPECT_FALSE(sender.get_sending_buffer());

    // Upewnij się, że proces wysyłania zachodzi tylko wówczas, gdy w bufor jest pełny.
    sender.send_package();
}
