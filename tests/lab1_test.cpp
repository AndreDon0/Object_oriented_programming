#include <gtest/gtest.h>
#include "lab1.h"

// --- RGB -> HSV tests ---
TEST(ColorTest, BlackRGBtoHSV) {
    Color c((unsigned char)0, (unsigned char)0, (unsigned char)0);
    auto [h, s, v] = c.GetHSV();

    EXPECT_FLOAT_EQ(h, 0.0f);
    EXPECT_FLOAT_EQ(s, 0.0f);
    EXPECT_FLOAT_EQ(v, 0.0f);
}

TEST(ColorTest, WhiteRGBtoHSV) {
    Color c((unsigned char)255, (unsigned char)255, (unsigned char)255);
    auto [h, s, v] = c.GetHSV();

    EXPECT_FLOAT_EQ(h, 0.0f);
    EXPECT_FLOAT_EQ(s, 0.0f);
    EXPECT_FLOAT_EQ(v, 1.0f);
}

TEST(ColorTest, RedRGBtoHSV) {
    Color c((unsigned char)255, (unsigned char)0, (unsigned char)0);
    auto [h, s, v] = c.GetHSV();

    EXPECT_FLOAT_EQ(h, 0.0f);
    EXPECT_FLOAT_EQ(s, 1.0f);
    EXPECT_FLOAT_EQ(v, 1.0f);
}

TEST(ColorTest, GreenRGBtoHSV) {
    Color c((unsigned char)0, (unsigned char)255, (unsigned char)0);
    auto [h, s, v] = c.GetHSV();

    EXPECT_FLOAT_EQ(h, 120.0f);
    EXPECT_FLOAT_EQ(s, 1.0f);
    EXPECT_FLOAT_EQ(v, 1.0f);
}

TEST(ColorTest, BlueRGBtoHSV) {
    Color c((unsigned char)0, (unsigned char)0, (unsigned char)255);
    auto [h, s, v] = c.GetHSV();

    EXPECT_FLOAT_EQ(h, 240.0f);
    EXPECT_FLOAT_EQ(s, 1.0f);
    EXPECT_FLOAT_EQ(v, 1.0f);
}

// --- HSV -> RGB tests ---
TEST(ColorTest, HSVtoRed) {
    Color c(0.0f, 1.0f, 1.0f);
    auto [r, g, b] = c.GetRGB();

    EXPECT_EQ(r, 255);
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, 0);
}

TEST(ColorTest, HSVtoGreen) {
    Color c(120.0f, 1.0f, 1.0f);
    auto [r, g, b] = c.GetRGB();

    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, 255);
    EXPECT_EQ(b, 0);
}

TEST(ColorTest, HSVtoBlue) {
    Color c(240.0f, 1.0f, 1.0f);
    auto [r, g, b] = c.GetRGB();

    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, 255);
}

TEST(ColorTest, HSVtoWhite) {
    Color c(0.0f, 0.0f, 1.0f);
    auto [r, g, b] = c.GetRGB();

    EXPECT_EQ(r, 255);
    EXPECT_EQ(g, 255);
    EXPECT_EQ(b, 255);
}

// --- Setters & Consistency ---
TEST(ColorTest, SetterConsistency) {
    Color c((unsigned char)255, (unsigned char)0, (unsigned char)0); // red
    c.SetHue(120.0f);   // should turn green
    auto [r, g, b] = c.GetRGB();

    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, 255);
    EXPECT_EQ(b, 0);
}
