/*************************************************************************
 * \File:ev1527.c
 * \Summary:
 * \Version:1.0
 * \Created:2020/9/15 by CaoKang
 *************************************************************************/
#include "ev1527.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_exti.h"
#include "delay.h"


/*!
 * \Name  : EV1527_init
 * \Brief : set 1527 data pin to extern interrupt
 * \Created: 2020/9/15 by CaoKang
 */
void EV1527_init()
{
    GPIO_Init(EV1527_DATA_GPIO, EV1527_DATA_PIN, GPIO_MODE_IN_PU_NO_IT);
//    EXTI_SetExtIntSensitivity(EV1527_EXTI_PORT, EXTI_SENSITIVITY_RISE_FALL);
    
     GPIO_Init(GPIOD, EV1527_DATA_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
    
}

/*!
 * \Name  : EV1527_getPinStatus
 * \Brief : -
 * \Created: 2020/9/15 by CaoKang
 */
uint8_t EV1527_getPinStatus()
{
    static uint8_t tmp=0;
    tmp = GPIO_ReadInputData(EV1527_DATA_GPIO) & EV1527_DATA_PIN;
    if(tmp == 0){
        return 0;
    }else{
        return 1;
    }
    
}

/*!
 * \Name  : EV1527_IRQ
 * \Brief : Called when pin status changed
 * \Created: 2020/9/15 by CaoKang
 */
void EV1527_IRQ()
{
    
}

/*!
 * \Name  : readBit
 * \Brief : 
 * \reVal : --
 * \Created: 2020/9/15 by CaoKang
 */
static uint8_t readBit()
{
    uint16_t high = 0; // high status duration
    uint16_t low = 0;  // low  status duration
    uint16_t duration = 0;
    while(EV1527_getPinStatus() && duration < 10000){
        high ++;
        duration ++;
    }
    while(!EV1527_getPinStatus()){
        low ++;        
    }    
}

#define  PERMITTED_ERROR 0.2
/*!
 * \Name  : EV1527_decode
 * \Brief : -
 * \Param :  -> 
 * \Created: 2020/9/15 by CaoKang
 */
uint32_t EV1527_decode()
{
    static uint16_t syn_high = 0; // high status duration
    static uint16_t syn_low = 0;  // low  status duration
    static uint16_t duration = 0;
    syn_high = 0;
    syn_low = 0;
    duration = 0;

    
    
    while(EV1527_getPinStatus()){
        syn_high ++;
        delay(1);
        
    }
    while(!EV1527_getPinStatus()){
        syn_low ++;  
        delay(1);
    }
    static uint32_t data=0;
    data=0;
    
    if(syn_high <= 5 || syn_low <= 50){
        return 0;
    }

    if(syn_low <= syn_high*20 )
        return 0;
    uint16_t t1 = syn_high >> 1;
    if(syn_low >= syn_high*35 ){
        duration = 0;
        return 0;
    }
        
    
    if(1){

        static uint8_t i;
        for(i=0; i<24; i++){
            duration = 0;
            while(EV1527_getPinStatus()){
                delay(1);
                duration ++;
            }
            if(t1 < duration && duration < (syn_high<<1)){
                 data |= 1;
            }else if(t1 > duration || duration > (syn_high<<2)){
                duration = 0;
                return 0;
            }
            data <<= 1;
            while(!EV1527_getPinStatus());
        }
        if(data == 0x01DAAA1E || data == 0x12DC25E || data == 0x018F9A8E){
            GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
        }
        
        return data;
    }
    GPIO_WriteLow(GPIOD,GPIO_PIN_3);
    return 0;
}



