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

//Kueczek

TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}