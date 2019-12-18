//
// Created by Aayli on 18.12.2019.
//

#include "gtest/gtest.h"

#include "package.hpp"

TEST(NetPackageTest, createPackage) {
    Package A;
    Package B;
    ASSERT_EQ(A.get_id(), 1);
    ASSERT_EQ(B.get_id(), 2);
    {
        Package C;
        ASSERT_EQ(C.get_id(), 3);
    }
    Package C;
    ASSERT_EQ(C.get_id(), 3);
}

