/**
 * @file
 */
#include <gtest/gtest.h>

using namespace testing;

extern "C" {
    #include "cdd.h"
    #include "autoconf.h"
}

#include "mockup_src_cdd.h"

TEST(cdd, test_init) {
    CDD_Init();
}

TEST(cdd, test_main) {
    CREATE_MOCK(mock);

    EXPECT_CALL(mock, ASIC_SendCmd(ASIC_CMD_READ)).Times(1).WillOnce(Return(0));
    CDD_Main();

    EXPECT_CALL(mock, ASIC_ReadData(_)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(mock, ASIC_SendCmd(ASIC_CMD_CLEAR)).Times(1).WillOnce(Return(0));
    //TODO: EXPECT_CALL(mock, ASIC_ASIC_ReadErrorFlags(_)).Times(1).WillOnce(Return(0));
    CDD_ISR_StateHandling();
    
    EXPECT_CALL(mock, ASIC_GetCmdStatus(_, _)).Times(1).WillOnce(Return(0));
    //TODO: EXPECT_CALL(mock, ASIC_ASIC_ReadErrorFlags(_)).Times(1).WillOnce(Return(0));
    CDD_ISR_StateHandling();

    EXPECT_CALL(mock, ASIC_SendCmd(ASIC_CMD_REG)).Times(1).WillOnce(Return(0));
    CDD_Main();

    EXPECT_CALL(mock, ASIC_ReadData(_)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(mock, ASIC_SendCmd(ASIC_CMD_CLEAR_REG)).Times(1).WillOnce(Return(0));
    //TODO: EXPECT_CALL(mock, ASIC_ASIC_ReadErrorFlags(_)).Times(1).WillOnce(Return(0));
    CDD_ISR_StateHandling();

    EXPECT_CALL(mock, ASIC_GetCmdStatus(_, _)).Times(1).WillOnce(Return(0));
    //TODO: EXPECT_CALL(mock, ASIC_ASIC_ReadErrorFlags(_)).Times(1).WillOnce(Return(0));
    CDD_ISR_StateHandling();

    EXPECT_CALL(mock, ASIC_SendCmd(ASIC_CMD_READ)).Times(1).WillOnce(Return(0));
    CDD_Main();
    
    EXPECT_CALL(mock, ASIC_SendCmd).Times(0).WillOnce(Return(0));
    CDD_Main();
    CDD_Main();
}
