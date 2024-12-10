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
}
