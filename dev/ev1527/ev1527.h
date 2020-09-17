/*************************************************************************
 * \File:ev1527.h
 * \Summary:
 * \Version:1.0
 * \Created:2020/9/15 by CaoKang
 *************************************************************************/
#ifndef _1527_DECODER_
#define _1527_DECODER_

#include "stm8s.h"

#define  EV1527_DATA_GPIO   GPIOA
#define  EV1527_EXTI_PORT   EXTI_PORT_GPIOA
#define  EV1527_DATA_PIN    GPIO_PIN_3

void EV1527_init();
uint8_t EV1527_getPinStatus();
uint32_t EV1527_decode();

#endif /*_1527_DECODER_*/

