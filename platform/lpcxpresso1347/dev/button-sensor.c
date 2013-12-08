/**
 * \addtogroup lpcxpresso1347-platform
 *
 * @{
 */
/*
 * Copyright (c) 2013, Christian Taedcke <hacking@taedcke.com>
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
 * This file is part of the Contiki OS
 *
 */
/*---------------------------------------------------------------------------*/
/**
* \file
*			Button sensor.
* \author
*			Christian Taedcke <hacking@taedcke.com>
*/
/*---------------------------------------------------------------------------*/

#include "dev/button-sensor.h"
#include "gpio/gpio.h"

#define BUTTON_1_PIN    (1)
#define BUTTON_1_PORT   (0)

/* Must differ from MRF24J40_INT_CHANNEL */
#define BUTTON_INT_CHANNEL  (1)

static struct timer debouncetimer;

__attribute__ ((section(".after_vectors")))
void
PIN_INT1_IRQHandler(void)
{
    ENERGEST_ON(ENERGEST_TYPE_IRQ);
    if ( GPIOPinIntStatus(BUTTON_INT_CHANNEL) )
    {
        if(timer_expired(&debouncetimer)) {
          timer_set(&debouncetimer, CLOCK_SECOND / 4);
          sensors_changed(&button_sensor);
        }
    }
    // Clear the interrupt
    GPIOPinIntClear(BUTTON_INT_CHANNEL);
    ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/**
 * @brief Initializes the gpio pins and interrupts
 *
 * @pre GPIOInit() must be called prior to this function.
 */
static void
init()
{
    /* Input pin */
    GPIOSetDir(BUTTON_1_PORT, BUTTON_1_PIN, 0);

    /* Activate interrupt for falling edge. */
    GPIOSetPinInterrupt(BUTTON_INT_CHANNEL, BUTTON_1_PORT, BUTTON_1_PIN, 0, 0);

    /* Enable also rising edge interrupt. */
    LPC_GPIO_PIN_INT->IENR |= (0x1 << BUTTON_INT_CHANNEL);

#if (BUTTON_1_PORT == 0) && (BUTTON_1_PIN == 1)
    /* enable internal pull-up and invert */
    LPC_IOCON->PIO0_1 = (0x02 << 3) | (0x01 << 6);
#else
#error Adapt io port settings for your settings here.
#endif
}

static void
activate()
{
    timer_set(&debouncetimer, 0);
    GPIOPinIntEnable(BUTTON_INT_CHANNEL, 0);
    LPC_GPIO_PIN_INT->SIENR |= (0x1 << BUTTON_INT_CHANNEL);
}

static void
deactivate()
{
    GPIOPinIntDisable(BUTTON_INT_CHANNEL, 0);
    LPC_GPIO_PIN_INT->CIENR |= (0x1 << BUTTON_INT_CHANNEL);
}

static int
active()
{
    /* Are rising and falling edge interrupts enabled? */
    return ((LPC_GPIO_PIN_INT->IENF & (0x1 << BUTTON_INT_CHANNEL))
            && (LPC_GPIO_PIN_INT->IENR & (0x1 << BUTTON_INT_CHANNEL)))
            ? 1 : 0;
}

/**
 * @brief Returns the current state of the button.
 *
 * @todo Debouncing is currently broken, because the interrrupt is generated for both edges
 *       and the logic implemented here cannot handle it.
 *
 * @param type This parameter is ignored.
 * @return The current state of the button.
 */
static int
button_sensor_value(int type)
{
    return GPIOGetPinValue(BUTTON_1_PORT, BUTTON_1_PIN) || !timer_expired(&debouncetimer);
}

static int
button_sensor_configure(int type, int value)
{
    if (type == SENSORS_HW_INIT)
    {
        init();
        return 1;
    }
    if (type == SENSORS_ACTIVE)
    {
        if (value)
            activate();
        else
            deactivate();
        return 1;
    }

    return 0;
}

static int
button_sensor_status(int type)
{
    if (type == SENSORS_READY)
    {
        return active();
    }

    return 0;
}

SENSORS_SENSOR(button_sensor,
               BUTTON_SENSOR,
               button_sensor_value,
               button_sensor_configure,
               button_sensor_status);

/** @} */
