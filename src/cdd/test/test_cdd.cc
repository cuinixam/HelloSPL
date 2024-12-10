/**
 * @file
 */
#include <gtest/gtest.h>

extern "C" {
    #include "cdd.h"
    #include "autoconf.h"
}

TEST(cdd, test_init) {
    CDD_Init();
}

TEST(cdd, test_main) {
    CDD_Main();
    EXPECT_EQ(Get_CDD_STATE(), TRIGGER_STATE_000);
    EXPECT_EQ(Get_CDD_ISR_STATE(), ISR_STATE_000);
}
