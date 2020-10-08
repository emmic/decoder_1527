/*************************************************************************
 * \File:rx10.c
 * \Summary:
 * \Version:1.0
 * \Created:2020/10/8 by CaoKang
 *************************************************************************/

#include "rx10.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_exti.h"
#include "delay.h"

const static uint8_t NOISE_HIGH = (uint8_t)NOISE_THRESHOLD;
const static uint8_t NOISE_LOW  = (uint8_t)(NOISE_THRESHOLD * RATIO_SYNC);
const static uint8_t SYN_MIN_RATIO = (uint8_t)(RATIO_SYNC * (1-ACCEPTABLE_ERROR));
const static uint8_t SYN_MAX_RATIO = (uint8_t)(RATIO_SYNC * (1+ACCEPTABLE_ERROR));
/*!
 * \Name  : RX10_init
 * \Param : void -> 
 * \Created: 2020/10/8 by CaoKang
 */
void RX10_init()
{
    GPIO_Init(RX10_DATA_GPIO, RX10_DATA_PIN, GPIO_MODE_IN_PU_NO_IT);
}

/*!
 * \Name  : getPinStatus
 * \Created: 2020/9/15 by CaoKang
 */
static uint8_t getPinStatus()
{
    return GPIO_ReadInputData(RX10_DATA_GPIO) & RX10_DATA_PIN ? 1 : 0;
}

/*!
 * \Name  : RX10_readData
 * \Brief : receive data
 * \Param :  -> 
 * \reVal : --
 * \Created: 2020/10/8 by CaoKang
 */
uint32_t RX10_readData()
{
    uint16_t syn_high = 0;  // high status duration
    uint16_t syn_low  = 0;  // low  status duration
    uint16_t duration = 0;  // duration counter
    static uint16_t d0_min_high = 0; // min duration of high level for data0
    static uint16_t judge_thre = 0;  // judge threshold
    static uint16_t d1_max_high = 0; // max duration of high level for data1
    
    while(getPinStatus()) { // high level
        syn_high ++;
        delay(1);
    }
    
    while(!getPinStatus()){ // low level
        if(syn_low == 0){
            // multiply calculation costs a lot of time, use it as a short delay
            d0_min_high = syn_high - (uint16_t)(syn_high * ACCEPTABLE_ERROR);
            judge_thre = (uint16_t)(syn_high * ((RATIO_DATA1-1)/2+1));
            d1_max_high = (uint16_t)(syn_high * RATIO_DATA1 * (1+ACCEPTABLE_ERROR));
            syn_low += 15;
        }else{
            delay(1);
        }
        syn_low ++;
    }


    
    if(syn_high > NOISE_HIGH && syn_low > NOISE_LOW // noise filtering
       && syn_high*SYN_MIN_RATIO < syn_low && syn_low < syn_high*SYN_MAX_RATIO) // sync head check
    {
        /////////////// read data ////////////////
        uint32_t data=0;
        for(uint8_t i=0; i<24; i++){
            duration = 0;
            while(getPinStatus()){
                delay(1);
                duration ++;
            }
            if(duration < d0_min_high  || d1_max_high < duration){ 
                return 0; // error duty cycle, give up
            }
            if(duration > judge_thre){ 
                data |= 1; // data 1
            }
            data <<= 1;
            while(!getPinStatus());
        }
        return data;
    }
    return 0;
}


