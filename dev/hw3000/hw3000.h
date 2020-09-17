/******************************************************************************
 * @file     hw3000.h
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
#ifndef __M928_HW3000_H
#define __M928_HW3000_H

#define	HW3000_BUF_LEN	255

typedef enum {
	IDLE = 0,
	TX = 1,
	RX = 2
} hw3000_state_t;

void hw3000_init();
void hw3000_setRcvdCallback(void (* cb)(uint8_t *data, uint8_t len));
void hw3000_send(uint8_t *data, uint8_t len);
void hw3000_irq();

#endif
