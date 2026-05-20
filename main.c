#include <lpc21xx.h>
#include "types.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "adc.h"
#include "rtc.h"
#include "led.h"
#include "eint.h"
#include "app.h"
#include "pwm.h"
#include "scheduler.h"
#include "config.h"
#include "motion.h"
#define LOAD_DEMO_RTC_ON_START   1

int main(void)
{
	  init_timer0();
    init_lcd();
	 app_load_custom_symbols();
	config_init();
    streetlight_led_init();
	 pwm_init();
    init_kpm();
    init_adc();
    rtc_init();
    eint0_init();
	motion_init();
	

#if LOAD_DEMO_RTC_ON_START

    SetRTCTimeInfo(17, 59, 50);
    SetRTCDateInfo(16, 5, 2026);
    SetRTCDay(SAT);

#endif

    app_show_startup_message();
      /*
       Timer1 scheduler starts after startup message.
    */
    scheduler_timer1_init();
    while(1)
    {
        if(g_eint0_flag == 1)
        {
            g_eint0_flag = 0;

             pwm_set_brightness(0);


            lcd_clear();
            lcd_gotoxy(0, 0);
            strlcd("RTC EDIT MODE");

            tdelay_ms(800);

            app_rtc_menu_process();
					  lcd_clear();
        }

        if(g_task_100ms_flag == 1)
        {
            g_task_100ms_flag = 0;
            app_keypad_task();
					  motion_task_100ms();
        }

        if(g_task_500ms_flag == 1)
        {
            g_task_500ms_flag = 0;
            app_display_task();
        }

        if(g_task_1sec_flag == 1)
        {
            g_task_1sec_flag = 0;
            app_streetlight_task();
        }

        if(g_task_5sec_flag == 1)
        {
            g_task_5sec_flag = 0;
            app_fault_task();
        }
    }
}

