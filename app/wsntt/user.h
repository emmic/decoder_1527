#ifndef _USER_H_
#define _USER_H_

typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
    
    RFLR_STATE_RX_ACK_INIT,
    RFLR_STATE_RX_ACK_DONE,
    RFLR_STATE_RX_TIMEOUT,
   
    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    
    RFLR_STATE_TX_ACK_INIT,
    RFLR_STATE_TX_ACK_DONE,
    
    RFLR_STATE_SLEEP,
}tRFLRStates;

#include <stdbool.h>
#include "gpio.h"
#include "hw3000.h"
#include "_reg_hw3000.h"
#include "delay.h"





#endif