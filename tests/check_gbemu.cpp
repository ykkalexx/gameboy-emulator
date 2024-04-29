#include <gtest/gtest.h>
#include <emu.h>
#include <cpu.h>

/*
    Unit that test the cpu_step using the Google Test framework
*/
TEST(GBEmuTest, TestNothing) {
    bool b = cpu_step();
    ASSERT_EQ(b, false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}