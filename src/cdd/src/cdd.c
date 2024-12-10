/**
 * @file cdd.c
 * @brief Cdd implementation.
 */


#include "cdd.h"
#include "asic_api.h"
#include "autoconf.h"

static TRIGGER_STATE_t TRIGGER_STATE = TRIGGER_STATE_000;
static ISR_STATE_t ISR_STATE = ISR_STATE_000;

static void CDD_StateHandling(void);

static void ISR_DetermineNextState(ISR_STATE_t current_isr_state, ISR_STATE_t next_isr_state, TRIGGER_STATE_t next_state) {
    ASIC_ErrorFlags error_flags;
    error_t ret_err;
    ret_err = ASIC_ReadErrorFlags(&error_flags);
    if (ret_err == 0) {
        if (error_flags.data_error == 0 && error_flags.cmd_error == 0) {
            ISR_STATE = next_isr_state;
            TRIGGER_STATE = next_state;
        } else {
            CDD_state_handling_semaphore = 0;
            asic_communication_error = 1;
            TRIGGER_STATE = TRIGGER_STATE_000;
        }
    } else {
        /* If there is an error in the communication */
        CDD_state_handling_semaphore = 0;
        asic_communication_error = 1;
        TRIGGER_STATE = next_state;
    }
}

extern TRIGGER_STATE_t Get_CDD_STATE() {
    return TRIGGER_STATE;
}

extern ISR_STATE_t Get_CDD_ISR_STATE() {
    return ISR_STATE;
}

void CDD_Init() {

}

void CDD_Main() {
    CDD_StateHandling();
}

// Define variable to synchronize the ISR and the state handling
// TODO: this semaphore is checked by the Interrupt Handler to call the ISR State Handling
extern uint8_t CDD_state_handling_semaphore = 0;
static uint8_t asic_communication_error = 0;

static void CDD_StateHandling(void) {
    // If the ISR is currently handling the state, return
    if (CDD_state_handling_semaphore == 1) {
        return;
    }
    // Take hold of the semaphore
    CDD_state_handling_semaphore = 1;
    switch (TRIGGER_STATE)
    {
    case TRIGGER_STATE_000:
        if (asic_communication_error != 0) {
            ISR_STATE = ISR_STATE_000;
            ASIC_ClearErrorFlags();
        } else {
            ISR_STATE = ISR_STATE_001;
            ASIC_SendCmd(ASIC_CMD_READ);
        }
        break;
    case TRIGGER_STATE_001:

        break;
    default:
        break;
    }
}

void CDD_ISR_StateHandling(void) {

    switch (ISR_STATE)
    {
    case ISR_STATE_000:
        break;
    case ISR_STATE_001:
        break;
    case ISR_STATE_002:
        break;
    case ISR_STATE_003:
        break;
    default:
        break;
    }
}

