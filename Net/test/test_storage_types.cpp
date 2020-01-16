//
// Created by Aayli on 18.12.2019.
//

#include "gtest/gtest.h"
#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"

using ::std::cout;
using ::std::endl;

TEST(NetStorage_TypesTest, Lifo) {
    Package A;
    Package B;
    PackageQueue lifo(PackageQueueType::LIFO);
    lifo.push((Package&&) A);
    lifo.push((Package&&) B);
    Package D = lifo.pop();
    ASSERT_EQ(D.get_id(),2);
}

TEST(NetStorage_TypesTest, Fifo) {
    Package A;
    Package B;
    PackageQueue lifo(PackageQueueType::FIFO);
    lifo.push((Package&&) A);
    lifo.push((Package&&) B);
    Package D = lifo.pop();
    ASSERT_EQ(D.get_id(),1);
}

// Kueczek

TEST(PackageQueueTest, IsFifoCorrect) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(PackageQueueTest, IsLifoCorrect) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}