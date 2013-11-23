#include <cmsis/LPC13Uxx.h>
#include <sys/clock.h>
#include <sys/cc.h>
#include <sys/etimer.h>
#include <dev/leds.h>

static volatile clock_time_t current_clock = 0;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;

void
SysTick_Handler(void) __attribute__ ((interrupt));

/// \todo ctae FIX IT!!
void
SysTick_Handler(void)
{
    /// \todo ctae why reading CSR register here?
  (void)SysTick->CTRL;

  //clear pending flag
  SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
  current_clock++;
  if(etimer_pending() && etimer_next_expiration_time() <= current_clock) {
    etimer_request_poll();
    /* printf("%d,%d\n", clock_time(),etimer_next_expiration_time  	()); */

  }

  if (--second_countdown == 0) {
    current_seconds++;
    second_countdown = CLOCK_SECOND;
  }
}


void
clock_init()
{
    SysTick_Config(SystemCoreClock / CLOCK_SECOND);
    /// \todo ctae is this needed to set systick irqprio? else max value
    NVIC_SetPriority (SysTick_IRQn, 8);  /* set Priority for Systick Interrupt to 8 (high) */
}

clock_time_t
clock_time(void)
{
  return current_clock;
}

/// \todo ctae implement clock_delay
void
clock_delay(unsigned int t)
{
    (void) t; //in uSec
}

unsigned long
clock_seconds(void)
{
  return current_seconds;
}
