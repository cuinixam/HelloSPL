#ifndef CDD_H
#define CDD_H

#include "rte.h"

// Type definition for the STATE
typedef enum {
    TRIGGER_STATE_000,
    TRIGGER_STATE_001
} TRIGGER_STATE_t;

// Type definition for the ISR_STATE
typedef enum {
    ISR_STATE_000,
    ISR_STATE_001,
    ISR_STATE_002,
    ISR_STATE_003
} ISR_STATE_t;

extern uint8_t CDD_state_handling_semaphore;

extern void CDD_Init();
extern void CDD_Main();

extern void CDD_ISR_StateHandling(void);

extern TRIGGER_STATE_t Get_CDD_STATE();
extern ISR_STATE_t Get_CDD_ISR_STATE();

#endif // CDD_H
