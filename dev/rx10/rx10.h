/*************************************************************************
 * \File:rx10.h
 * \Summary: driver for rx10
 * \Version:1.0
 * \Created:2020/10/8 by CaoKang
 *************************************************************************/
#ifndef _RX10_DRIVER_
#define _RX10_DRIVER_

#include "stm8s.h"

#define  RX10_DATA_GPIO   GPIOA
#define  RX10_DATA_PIN    GPIO_PIN_3
#define  RX10_IRQ_PORT    EXTI_PORT_GPIOA

#define  ACCEPTABLE_ERROR  0.3         // acceptable margin of error [0 ~ 0.99]
 
#define  RATIO_SYNC        28        // The ratio of high level and low level
//#define  RATIO_DATA0       2.7       // The ratio of low level and high level
#define  RATIO_DATA1       3.3       // The ratio of low level and high level

#define  NOISE_THRESHOLD   10           // filtrate the high level noise under this threshold and 
                                       // low level noise under NOISE_THRESHOLD*RATIO_SYNC
                                       // *** this value should more than 1 ***


void RX10_init();
uint32_t RX10_readData();

#endif /*_HX10_DRIVER_*/

