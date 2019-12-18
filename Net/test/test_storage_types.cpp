//
// Created by Aayli on 18.12.2019.
//

#include "gtest/gtest.h"

#include "storage_types.hpp"

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