
/**
* \file
*                       Leds.
* \author
*                       Christian Taedcke <hacking@taedcke.com>
*/

#include "contiki-conf.h"
#include "dev/leds.h"
#include <cmsis/LPC13Uxx.h>
#include <gpio/gpio.h>


#define LEDS_CONF_RED_PIN   7  //PIO0_7
#define LEDS_CONF_RED       (1 << LEDS_CONF_RED_PIN)

#define LEDS_PORT LPC_GPIO->MPIN[PORT0]

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
    //set to output
    GPIOSetDir(PORT0, LEDS_CONF_RED_PIN, 1);
    //switch off
    LPC_GPIO->MASK[PORT0] = ~LEDS_CONF_RED; //only enable read and write for this bit
    LPC_GPIO->MPIN[PORT0] = 0;
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return ((LEDS_PORT & LEDS_CONF_RED) ? LEDS_RED : 0);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
  LEDS_PORT = (leds & LEDS_RED) ? LEDS_CONF_RED : 0;
}
/*---------------------------------------------------------------------------*/
/** @} */
