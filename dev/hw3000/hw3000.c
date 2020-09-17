/******************************************************************************
* @file     hw3000.c
* @brief    hw3000 driver
*           
*
* @version  1.0
* @date     2016-01-14
*
* @author   Sundy
*
* @note
* @Copyright (C) 2016 Shanghai Eastsoft Microelectronics C0., Ltd.
******************************************************************************/
#include "stm8s.h"
#include "gpio.h"
#include "hw3000.h"
#include "_reg_hw3000.h"
#include "delay.h"
#include <string.h>

volatile hw3000_state_t _hw3000_state; 		
volatile uint8_t 		_hw3000_irq_request;	//request for irq

struct {
    uint8_t len;
    uint8_t data[HW3000_BUF_LEN];
} packet;
//static uint8_t databuf[HW3000_BUF_LEN];
static void (* f)(uint8_t *data, uint8_t len);

const uint8_t addr[] = {
    0x4C ,0x1B ,0x1C ,0x1D ,0x1E ,0x1F ,0x20 ,0x21 ,0x22 ,0x23 ,
    0x24 ,0x25 ,0x26 ,0x27 ,0x28 ,0x29 ,0x2A ,0x03 ,0x11 ,0x14 ,
    0x40 ,0x41 ,0x42 ,0x43 ,0x17 ,0x51 ,0x55 ,0x56 ,0x62 ,0x4C ,
    0x11 ,0x26 ,0x1C ,0x25 ,0x35 ,0x30 ,0x31 ,0x2F ,0x32 ,0x33 ,
    0x2E ,0x2C ,0x40 ,0x02 ,0x03 ,0x24 ,0x0E ,0x4E ,0x13 ,0x14 ,
    0x15 ,0x01 ,0x0E
};

const uint16_t value[] = {
    0x55AA ,0x1371 ,0x1351 ,0x0B51 ,0x2F71 ,0x2F51 ,0x2E51 ,0x2E31 ,0x4B31 ,0x4731 ,
    0x4631 ,0x4531 ,0x4431 ,0x6131 ,0x6031 ,0x6011 ,0x6009 ,0x0508 ,0xC630 ,0x1935 ,
    0x0008 ,0x0010 ,0x82D8 ,0x3D38 ,0xF6C2 ,0x001B ,0x8003 ,0x4155 ,0x70ED ,0x5555 ,
    0x8000 ,0x2E35 ,0x100F ,0x5201 ,0x3312 ,0x8000 ,0x0000 ,0x2056 ,0x0009 ,0x00D4 ,
    0x0025 ,0x0007 ,0xFF3F ,0x4000 ,0x8010 ,0x0206 ,0x0000 ,0x0000 ,0x0001 ,0x7E7E ,
    0x7E7E ,0x0080 ,0x0200
};

void hw3000_setRcvdCallback(void (* cb)(uint8_t *data, uint8_t len))
{
    f = cb;
}
void hw3000_power_up(void)
{
    SPI_Pwr_ON();
}

void hw3000_power_down(void)
{
    SPI_Pwr_OFF();
}

static void hw3000_write_reg(uint8_t addr, uint16_t data)
{   
    SPI_CS_Enable();
    
    SPI_Write(addr|0x80);
    SPI_Write(data >> 8);
    SPI_Write(data & 0x00FF);
    
    SPI_CS_Disable(); 
}

static uint16_t hw3000_read_reg(uint8_t addr)
{
    uint16_t  temp=0;
    SPI_CS_Enable();
    
    SPI_Write(addr&0x7F);
    temp = (SPI_Read()<<8) + SPI_Read();
    
    SPI_CS_Disable();
    
    return temp;
}

static void hw3000_write_fifo(uint8_t addr, uint8_t *data, uint8_t length)
{  
    SPI_CS_Enable();
    
    SPI_Write(addr|0x80);
    
    for(uint8_t i=0; i<length; i++){
        SPI_Write(data[i]);
    }
    
   SPI_CS_Disable();
}

static void hw3000_read_fifo(uint8_t addr, uint8_t *data, uint8_t length)
{
    SPI_CS_Enable();
    SPI_Write(addr&0x7F);
    
    for(uint8_t i=0; i<length; i++){
        data[i] = SPI_Read();
    }
    
   SPI_CS_Disable();
}

static void hw3000_rx_enable(void)
{
    hw3000_write_reg(TRCTRL, 0x0080); //enable rx
    hw3000_write_reg(FIFOSTA, 0x0200); //flush fifo
    _hw3000_state = RX;
}

void hw3000_init()
{
    MCU_gpio_init();
	
    hw3000_power_down();
    HAL_Delay_nMS(1);
    hw3000_power_up();//RFÈ«¾Ö¸´Î»  	
    
	while(!(hw3000_read_reg(INTFLAG) & 0x4000)); //wait for chip ready
    
    for(uint8_t i=0; i<sizeof(addr); i++){
        hw3000_write_reg(addr[i], value[i]);
    }
    hw3000_rx_enable();
}

void hw3000_send(uint8_t *data, uint8_t len)
{	
	if (_hw3000_state != TX) {
		_hw3000_state = TX;
		
		hw3000_write_reg(TRCTRL, 0x0100); 	//tx_enable
		hw3000_write_reg(FIFOSTA, 0x0108);  //flush fifo
        
        packet.len = len+1;
        memcpy(packet.data,data,len);
	    hw3000_write_fifo(FIFODATA, &packet.len, packet.len); //write fifo
        packet.len = 0;
        
		hw3000_write_reg(FIFOCTRL, 0x0001); //ocpy = 1
        
//        static uint16_t val = 0;
//        do{
//            val = hw3000_read_reg(INTFLAG);
//        }while(val && 0x0001 != 1);
        _hw3000_irq_request = 0;
        while (!_hw3000_irq_request); //wait for send finish
        _hw3000_irq_request = 0;    	
        
		hw3000_write_reg(FIFOCTRL, 0x0000); //ocpy = 0
		hw3000_write_reg(INTIC, 0x0001); //clr_int
    
        hw3000_rx_enable();
	}
}

static void hw3000_rcvd(){
    
    hw3000_read_fifo(FIFODATA, &packet.len, 1);
    packet.len --;
    hw3000_read_fifo(FIFODATA, packet.data, packet.len);    
    _hw3000_irq_request = 0;
    
    hw3000_write_reg(INTIC, 0x7009); //clear interrupt
    
    f(packet.data,packet.len);

    packet.len = 0;    
}

void hw3000_irq()
{
    if(_hw3000_irq_request == 1) return; // last irq isn't finished
    
    _hw3000_irq_request = 1;
    if(_hw3000_state == RX){
        hw3000_rcvd();
    }  
}
