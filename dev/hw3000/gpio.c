#include "stm8s.h"
#include "gpio.h"
#include "stm8s_exti.h"


void MCU_gpio_init(void)
{
    GPIO_Init(HW3000_CSN_PORT, HW3000_CSN_IO,GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(HW3000_CLK_PORT, HW3000_CLK_IO,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(HW3000_SDO_PORT, HW3000_SDO_IO,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(HW3000_SDI_PORT, HW3000_SDI_IO,GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(HW3000_IRQ_PORT, HW3000_IRQ_IO,GPIO_MODE_IN_FL_IT);
    GPIO_Init(HW3000_PDN_PORT, HW3000_PDN_IO,GPIO_MODE_OUT_PP_LOW_FAST);
    
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);   
    
    GPIO_Init(LED_PORT, LED_IO,GPIO_MODE_OUT_PP_LOW_FAST);
    
}

void SPI_Pwr_ON()
{
    GPIO_WriteLow(HW3000_PDN_PORT,HW3000_PDN_IO);//PDN 0
}

void SPI_Pwr_OFF()
{
    GPIO_WriteHigh(HW3000_PDN_PORT,HW3000_PDN_IO);//PDN 1
}

void SPI_CS_Enable()
{
    GPIO_WriteLow(HW3000_CSN_PORT,HW3000_CSN_IO);
}

void SPI_CS_Disable()
{
    GPIO_WriteHigh(HW3000_CSN_PORT,HW3000_CSN_IO);
}


void SPI_Write(uint8_t data)
{
    for(uint8_t i=0;i<8;i++)
    {
        GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
        if(data&0x80)
        {
            GPIO_WriteHigh(HW3000_SDI_PORT,HW3000_SDI_IO);
        }
        else
        {
            GPIO_WriteLow(HW3000_SDI_PORT,HW3000_SDI_IO);
        }
        GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
        data<<=1;
    }    
}

uint8_t SPI_Read()
{
    uint8_t temp = 0;
    for(uint8_t i=0;i<8;i++)
    {
        temp<<=1;
        GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
        GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
        if((GPIO_ReadInputData(HW3000_SDO_PORT)&HW3000_SDO_IO) != 0){
            temp|=0x01;
        }
        else{
            temp&=0xfe;
        }
    }
    return temp;
}