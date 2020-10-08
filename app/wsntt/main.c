
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
//#include "ev1527.h"
#include "rx10.h"
#include "stm8s_gpio.h"
#include "delay.h"
//
#define LED_GPIO_PORT  (GPIOB)
#define LED_GPIO_PINS  (GPIO_PIN_5)

void main(void)
{

    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    
    GPIO_Init(LED_GPIO_PORT,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);
    
/*
    GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);
    while(1){
        GPIO_WriteReverse(GPIOA,GPIO_PIN_3);
        delay(1);
    }
*/
    enableInterrupts();
    RX10_init();

    static uint32_t data;
    while(1){
        data = RX10_readData();
        if(data == 0x00706570){
            GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
        }
    }
   
}

