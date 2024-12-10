/**
 * @file cdd.c
 * @brief Cdd implementation.
 */


#include "autoconf.h"
#include "cdd.h"
#include "asic_api.h"
#include "asic_data.h"

static TRIGGER_STATE_t trigger_state = TRIGGER_STATE_000;
static ISR_STATE_t isr_state = ISR_STATE_000;

static error_t return_error;

static void * read_array;

ASIC_service_t service;
ASIC_service_status_t status;

// Define the ASIC data to be updated when communication with the ASIC is successful
ASIC_data_t asic_data;

// Define variable to synchronize the ISR and the state handling
// TODO: this semaphore is checked by the Interrupt Handler to call the ISR State Handling
uint8_t CDD_state_handling_semaphore = 0;
static uint8_t asic_communication_error = 0;

static void CDD_TRIGGER_StateHandling(void);

static void ISR_ReadErrorFlagsAndDetermineNextState(uint8_t* state_ptr, uint8_t next_state, uint8_t next_trigger_state_in_case_of_error, error_t error) {
    ASIC_ErrorFlags error_flags;
    error_t ret_err;
    ret_err = ASIC_ReadErrorFlags(&error_flags);
    if (error == 0 || ret_err == 0) {
        if (error_flags.data_error == 0 && error_flags.cmd_error == 0) {
            // In case of no error continue with the next state. This handles both ISR and TRIGGER states
            *state_ptr = next_state;
            trigger_state = next_state;
        } else {
            // In case of an error, clear the error flags and go back to the initial state
            CDD_state_handling_semaphore = 0;
            asic_communication_error = 1;
            trigger_state = TRIGGER_STATE_000;
        }
    } else {
        // If there is an error in the communication with the ASIC, handle back control to the TRIGER state handling
        CDD_state_handling_semaphore = 0;
        asic_communication_error = 1;
        trigger_state = next_trigger_state_in_case_of_error;
    }
}

extern TRIGGER_STATE_t Get_CDD_STATE() {
    return trigger_state;
}

extern ISR_STATE_t Get_CDD_ISR_STATE() {
    return isr_state;
}

void CDD_Init() {

}

void CDD_Main() {
    CDD_TRIGGER_StateHandling();
}

static void CDD_TRIGGER_StateHandling(void) {
    // If the ISR is currently handling the state, return
    if (CDD_state_handling_semaphore == 1) {
        return;
    }
    // Handle the semaphore to the ISR
    CDD_state_handling_semaphore = 1;
    switch (trigger_state)
    {
    case TRIGGER_STATE_000:
        if (asic_communication_error != 0) {
            // Prepare and start the ISR first sequence
            isr_state = ISR_STATE_000;
            return_error = ASIC_ClearErrorFlags();
        } else {
            isr_state = ISR_STATE_001;
            return_error = ASIC_SendCmd(ASIC_CMD_READ);
        }
        break;
    case TRIGGER_STATE_001:
        if (asic_communication_error != 0) {
            // Prepare and start the ISR second sequence
            isr_state = ISR_STATE_003;
            return_error = ASIC_ClearErrorFlags();
        } else {
            isr_state = ISR_STATE_004;
            return_error = ASIC_SendCmd(ASIC_CMD_REG);
        }

        break;
    default:
        break;
    }
}

void CDD_ISR_StateHandling(void) {

    switch (isr_state)
    {
    // Handling error case for the first sequence
    case ISR_STATE_000:
        // Get Command status
        return_error |= ASIC_GetCmdStatus(&service, &status);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&isr_state), ISR_STATE_001, TRIGGER_STATE_000, return_error);
        return_error = 0;
        if (asic_communication_error == 0) {
            return_error = ASIC_SendCmd(ASIC_CMD_READ);
        } else {
            // No action
        }
        break;
    case ISR_STATE_001:
        read_array = (void*)(asic_data.sensor_data.Voltage);
        return_error = ASIC_ReadData(read_array);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&isr_state), ISR_STATE_002, TRIGGER_STATE_000, return_error);
        return_error = 0;
        if (asic_communication_error == 0) {
            return_error = ASIC_SendCmd(ASIC_CMD_CLEAR);
        } else {
            // No action
        }
        break;
    case ISR_STATE_002:
        return_error |= ASIC_GetCmdStatus(&service, &status);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&trigger_state), TRIGGER_STATE_001, TRIGGER_STATE_000, return_error);
        CDD_state_handling_semaphore = 0;
        return_error = 0;
        break;
    // Handling error case for the second sequence
    case ISR_STATE_003:
         // Get Command status
        return_error |= ASIC_GetCmdStatus(&service, &status);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&isr_state), ISR_STATE_004, TRIGGER_STATE_001, return_error);
        return_error = 0;
        if (asic_communication_error == 0) {
            return_error = ASIC_SendCmd(ASIC_CMD_READ);
        } else {
            // No action
        }
        break;
    case ISR_STATE_004:
        read_array = (void*)(asic_data.sensor_data.Register);
        return_error = ASIC_ReadData(read_array);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&isr_state), ISR_STATE_005, TRIGGER_STATE_001, return_error);
        return_error = 0;
        if (asic_communication_error == 0) {
            return_error = ASIC_SendCmd(ASIC_CMD_CLEAR_REG);
        } else {
            // No action
        }
        break;
    case ISR_STATE_005:
        return_error |= ASIC_GetCmdStatus(&service, &status);
        ISR_ReadErrorFlagsAndDetermineNextState((uint8_t*)(&trigger_state), TRIGGER_STATE_000, TRIGGER_STATE_001, return_error);
        CDD_state_handling_semaphore = 0;
        return_error = 0;
        break;
    default:
        break;
    }
}

