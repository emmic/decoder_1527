/*************************************************************************
 * \File:delay.c
 * \Summary:
 * \Version:1.0
 * \Created:2020/9/17 by CaoKang
 *************************************************************************/

#include "delay.h"

#if(FREQ == 160000000UL)    
#define  PARA     3200
#elif(FREQ == 20000000UL)
#define  PARA     390
#else
#error "no support"
#endif
void delay_ms(unsigned int time_delay)
{
 unsigned int i,j;
 for(i=time_delay;i>0;i--)
 {
   for(j=PARA; j>0; j--);
 }

}

//16MHz: 2.25us
//2MHz: 18us

void delay(unsigned int time) 
{
    while(time--);
}