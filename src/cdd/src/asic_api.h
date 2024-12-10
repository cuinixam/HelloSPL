#ifndef ASIC_API_H_
#define ASIC_API_H_

#include "rte.h"

// Commands for the ASIC
typedef enum {
    ASIC_CMD_INIT,
    ASIC_CMD_CLEAR,
    ASIC_CMD_CLEAR_REG,
    ASIC_CMD_READ,
    ASIC_CMD_REG
} ASIC_command_t;


typedef struct {
    uint8_t data_error;
    uint8_t cmd_error;
} ASIC_ErrorFlags;

// ASIC service status
typedef enum {
    ASIC_CMD_STATUS_OK,
    ASIC_CMD_STATUS_PENDING,
    ASIC_CMD_STATUS_ERROR
} ASIC_service_status_t;

// ASIC service
typedef enum {
    ASIC_SERVICE_INIT,
    ASIC_SERVICE_READ_ADC,
    ASIC_SERVICE_WRITE_PIN
} ASIC_service_t;

/* Clear all ASIC error flags*/
extern error_t ASIC_ClearErrorFlags();

/* Read ASIC error flags */
extern error_t ASIC_ReadErrorFlags(ASIC_ErrorFlags * error_flags);

/* Send a command to the ASIC */
extern error_t ASIC_SendCmd(ASIC_command_t command);

/* Send a command to the ASIC */
extern error_t ASIC_GetCmdStatus(ASIC_service_t *service, ASIC_service_status_t * status);

/* Read 6 bytes from the ASIC and store the data in the read_array */
extern error_t ASIC_ReadData(void * read_array);

#endif
