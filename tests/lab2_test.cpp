#include <gtest/gtest.h>
#include "lab2.h"

// --- Basic functionality tests ---
TEST(VectorTest, DefaultConstructor) {
    Vector<double> v1;
    EXPECT_EQ(v1.Size(), 0);
    EXPECT_TRUE(v1.OwnsMemory());
}

TEST(VectorTest, SizedConstructor) {
    Vector<double> v2(5);
    EXPECT_EQ(v2.Size(), 5);
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(v2.GetElement(i), 0);
    }
}

TEST(VectorTest, FillConstructor) {
    Vector<double> v3(4, 7);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(v3.GetElement(i), 7);
    }
}

TEST(VectorTest, ExternalBuffer) {
    double data[3] = {1, 2, 3};
    Vector<double> v4(data, 3);
    EXPECT_FALSE(v4.OwnsMemory());
    EXPECT_EQ(v4.GetElement(1), 2);
}

TEST(VectorTest, DotProduct) {
    Vector<double> v5(3, 2); // [2,2,2]
    Vector<double> v6(3, 3); // [3,3,3]
    EXPECT_EQ(v5.Dot(v6), 18); // 2*3+2*3+2*3
}

TEST(VectorTest, LinkAndUnlink) {
    double buf[2] = {10, 20};
    Vector<double> v5(3, 2);
    v5.Link(buf, 2);
    EXPECT_FALSE(v5.OwnsMemory());
    EXPECT_EQ(v5.GetElement(0), 10);

    v5.Unlink();
    EXPECT_TRUE(v5.OwnsMemory());
    EXPECT_EQ(v5.Size(), 0);
}
