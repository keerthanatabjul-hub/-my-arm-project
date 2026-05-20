#include <lpc21xx.h>
#include "types.h"
#include "scheduler.h"
#include "pwm.h"

#define TIMER1_VIC_CHANNEL   5

/*
   Assumption:
   PCLK = 15 MHz

   T1PR = 14999:
   Timer1 TC increments every 1 ms.
*/

volatile u8 g_task_100ms_flag = 0;
volatile u8 g_task_500ms_flag = 0;
volatile u8 g_task_1sec_flag  = 0;
volatile u8 g_task_5sec_flag  = 0;

static volatile u32 g_ms_count = 0;

void scheduler_timer1_init(void)
{
    /*
       Reset Timer1
    */
    T1TCR = (1 << 1);

    /*
       1 ms timer tick
    */
    T1PR = 14999;

    /*
       Match every 1 ms
    */
    T1MR0 = 1;

    /*
       Interrupt and reset on MR0
    */
    T1MCR = (1 << 0) | (1 << 1);

    /*
       Timer1 as IRQ
    */
    VICIntSelect &= ~(1 << TIMER1_VIC_CHANNEL);

    /*
       Use VIC slot 1.
       EINT0 can use slot 0.
    */
    VICVectAddr1 = (unsigned int)timer1_isr;
    VICVectCntl1 = (1 << 5) | TIMER1_VIC_CHANNEL;

    VICIntEnable = (1 << TIMER1_VIC_CHANNEL);

    /*
       Clear Timer1 interrupt flag
    */
    T1IR = 1;

    /*
       Start Timer1
    */
    T1TCR = (1 << 0);
}

void timer1_isr(void) __irq
{
    /*
       Clear Timer1 MR0 interrupt flag
    */
    T1IR = 1;

    /*
       Software PWM update every 1 ms
    */
    pwm_1ms_task();

    g_ms_count++;

    if((g_ms_count % 100) == 0)
    {
        g_task_100ms_flag = 1;
    }

    if((g_ms_count % 500) == 0)
    {
        g_task_500ms_flag = 1;
    }

    if((g_ms_count % 1000) == 0)
    {
        g_task_1sec_flag = 1;
    }

    if((g_ms_count % 5000) == 0)
    {
        g_task_5sec_flag = 1;
    }

    if(g_ms_count >= 60000)
    {
        g_ms_count = 0;
    }

    VICVectAddr = 0;
}

