/**
 * \addtogroup mbxxx-platform
 *
 * @{
 */

/*
 * Copyright (c) 2010, STMicroelectronics.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
* \file
*			Real-timer specific implementation for STM32W.
* \author
*			Salvatore Pitrulli <salvopitru@users.sourceforge.net>
*/

#include "sys/energest.h"
#include "sys/rtimer.h"
#include "hal/micro/micro-common.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static uint16_t saved_TIM1CFG;
static uint32_t time_msb = 0;   /* Most significant bits of the current time. */

/* time of the next rtimer event. Initially is set to the max
    value. */
static rtimer_clock_t next_rtimer_time = 0;

/*---------------------------------------------------------------------------*/
void
halTimer1Isr(void)
{
  if(INT_TIM1FLAG & INT_TIMUIF) {
    rtimer_clock_t now, clock_to_wait;
    /* Overflow event. */
    /* PRINTF("O %4x.\r\n", TIM1_CNT); */
    /* printf("OV "); */
    time_msb++;
    now = ((rtimer_clock_t) time_msb << 16) | TIM1_CNT;
    clock_to_wait = next_rtimer_time - now;

    if(clock_to_wait <= 0x10000 && clock_to_wait > 0) { 
      /* We must now set the Timer Compare Register. */
      TIM1_CCR1 = (uint16_t) clock_to_wait;
      INT_TIM1FLAG = INT_TIMCC1IF;
      INT_TIM1CFG |= INT_TIMCC1IF;      /* Compare 1 interrupt enable. */
    }
    INT_TIM1FLAG = INT_TIMUIF;
  } else {
    if(INT_TIM1FLAG & INT_TIMCC1IF) {
      /* Compare event. */
      INT_TIM1CFG &= ~INT_TIMCC1IF;       /* Disable the next compare interrupt */
      PRINTF("\nCompare event %4x\r\n", TIM1_CNT);
      PRINTF("INT_TIM1FLAG %2x\r\n", INT_TIM1FLAG);
      ENERGEST_ON(ENERGEST_TYPE_IRQ);
      rtimer_run_next();
      ENERGEST_OFF(ENERGEST_TYPE_IRQ);
      INT_TIM1FLAG = INT_TIMCC1IF;
    }
  }
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{
  TIM1_CR1 = 0;
  TIM1_PSC = RTIMER_ARCH_PRESCALER;

  /* Counting from 0 to the maximum value. */
  TIM1_ARR = 0xffff;

  /* Bits of TIMx_CCMR1 as default. */
  /* Update Generation. */
  TIM1_EGR = TIM_UG;
  INT_TIM1FLAG = 0xffff;

  /* Update interrupt enable (interrupt on overflow).*/
  INT_TIM1CFG = INT_TIMUIF;

  /* Counter enable. */
  TIM1_CR1 = TIM_CEN;

  /* Enable top level interrupt. */
  INT_CFGSET = INT_TIM1;

}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_disable_irq(void)
{
  ATOMIC(saved_TIM1CFG = INT_TIM1CFG; INT_TIM1CFG = 0;)
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_enable_irq(void)
{
  INT_TIM1CFG = saved_TIM1CFG;
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t
rtimer_arch_now(void)
{
  rtimer_clock_t t;

  ATOMIC(t = ((rtimer_clock_t) time_msb << 16) | TIM1_CNT;)
  return t;
}

/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
  rtimer_clock_t now, clock_to_wait;
  PRINTF("rtimer_arch_schedule time %4x\r\n", /*((uint32_t*)&t)+1, */ 
         (uint32_t)t);
  next_rtimer_time = t;
  now = rtimer_arch_now();
  clock_to_wait = t - now;

  PRINTF("now %2x\r\n", TIM1_CNT);
  PRINTF("clock_to_wait %4x\r\n", clock_to_wait);

  if(clock_to_wait <= 0x10000) {
    /* We must now set the Timer Compare Register. */
    TIM1_CCR1 = (uint16_t)now + (uint16_t)clock_to_wait;
    INT_TIM1FLAG = INT_TIMCC1IF;
    INT_TIM1CFG |= INT_TIMCC1IF;        /* Compare 1 interrupt enable. */
    PRINTF("2-INT_TIM1FLAG %2x\r\n", INT_TIM1FLAG);
  }
  /* else compare register will be set at overflow interrupt closer to
     the rtimer event. */
}
/*---------------------------------------------------------------------------*/

#if RDC_CONF_MCU_SLEEP
void
rtimer_arch_sleep(rtimer_clock_t howlong)
{
  //TODO convert howlong to timer value
  int32u quarter_seconds = howlong * 4;
  uint8_t radio_on;

  halPowerDown();

  ENERGEST_OFF(ENERGEST_TYPE_CPU);
  halSleepForQsWithOptions(&quarter_seconds, 0);

  //ATOMIC(

        halPowerUp();

        /* Adjust clock.c for the time spent sleeping */
        unsigned long longhowlong=CLOCK_CONF_SECOND;
        longhowlong*=howlong;
        clock_adjust_ticks(longhowlong/RTIMER_ARCH_SECOND);

   ENERGEST_ON(ENERGEST_TYPE_CPU);

  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  SysTick_SetReload(RELOAD_VALUE);
  SysTick_ITConfig(ENABLE);
  SysTick_CounterCmd(SysTick_Counter_Enable);

  //)

  stm32w_radio_driver.init();

  uart1_init(115200);
  leds_init();
  rtimer_init();

  PRINTF("WakeInfo: %04x\r\n", halGetWakeInfo());


//    CRM->WU_CNTLbits.TIMER_WU_EN = 1;
//    CRM->WU_CNTLbits.RTC_WU_EN = 0;
//    CRM->WU_TIMEOUT = howlong;

//    /* the maca must be off before going to sleep */
//    /* otherwise the mcu will reboot on wakeup */
//    maca_off();

//    CRM->SLEEP_CNTLbits.DOZE = 0;
//    CRM->SLEEP_CNTLbits.RAM_RET = 3;
//    CRM->SLEEP_CNTLbits.MCU_RET = 1;
//    CRM->SLEEP_CNTLbits.DIG_PAD_EN = 1;
//    CRM->SLEEP_CNTLbits.HIB = 1;

//  /* wait for the sleep cycle to complete */
//  while((*CRM_STATUS & 0x1) == 0) { continue; }
//  /* write 1 to sleep_sync --- this clears the bit (it's a r1wc bit) and powers down */
//  *CRM_STATUS = 1;

//  /* asleep */

//  /* wait for the awake cycle to complete */
//  while((*CRM_STATUS & 0x1) == 0) { continue; }
//  /* write 1 to sleep_sync --- this clears the bit (it's a r1wc bit) and finishes wakeup */
//  *CRM_STATUS = 1;

//    CRM->WU_CNTLbits.TIMER_WU_EN = 0;
//    CRM->WU_CNTLbits.RTC_WU_EN = 1;

//    /* reschedule clock ticks */
//    clock_init();
//    clock_adjust_ticks((CRM->WU_COUNT*CLOCK_CONF_SECOND)/rtc_freq);
}
#endif /* RDC_CONF_MCU_SLEEP */
/*---------------------------------------------------------------------------*/

/** @} */
