/*
 * Copyright (c) 2013, Karl Palsson <karlp@tweak.net.au>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \file   mrf24j40-arch.c
 * 
 * \brief  MRF24J40 arch driver for seedeye platform
 * \author Karl Palsson <karlp@tweak.net.au>
 */

#include "dev/mrf24j40.h"
#include "mrf24j40-arch.h"

#include "ssp0/ssp0.h"
#include "gpio/gpio.h"


bool mrf24j40_arch_irq_is_enabled = false;

void mrf24j40_arch_wake_pin(int val)
{
    GPIOSetBitValue(MRF24J40_WAKE_PORT, MRF24J40_WAKE_PIN, val);
}

/**
 * @brief Perform or release hard reset on mrf24j40.
 *
 * @param val logical value for asserting reset, 1 means perform reset,
 *            0 relases the radio from reset.
 */
void mrf24j40_arch_hard_reset(int val)
{
#if MRF24J40_HARD_RESET_CONNECTED
    GPIOSetBitValue(MRF24J40_RESET_PORT, MRF24J40_RESET_PIN, val ? 0 : 1);
#endif /*MRF24J40_HARD_RESET_CONNECTED*/
}

int mrf24j40_arch_init(void)
{

     ssp0Init();

	/* Set the IO pins direction */
    GPIOSetDir(MRF24J40_WAKE_PORT, MRF24J40_WAKE_PIN, 1);
    GPIOSetDir(MRF24J40_INT_PORT, MRF24J40_INT_PIN, 0);
    GPIOSetDir(MRF24J40_CSn_PORT, MRF24J40_CSn_PIN, 1);

#if MRF24J40_HARD_RESET_CONNECTED
    GPIOSetDir(MRF24J40_RESET_PORT, MRF24J40_RESET_PIN, 1);
#endif /* MRF24J40_HARD_RESET_CONNECTED */

    /* Set interrupt registers and reset flags: falling edge */
    GPIOSetPinInterrupt(MRF24J40_INT_CHANNEL, MRF24J40_INT_PORT, MRF24J40_INT_PIN, 0, 0);

    mrf24j40_arch_irq_is_enabled = true;

	return 0;
}

int mrf24j40_arch_is_irq_enabled(void)
{
    return mrf24j40_arch_irq_is_enabled;
}

void mrf24j40_arch_irq_enable(void)
{
    GPIOPinIntEnable(MRF24J40_INT_CHANNEL, 0);
    mrf24j40_arch_irq_is_enabled = true;
}

void mrf24j40_arch_irq_disable(void)
{
    GPIOPinIntDisable(MRF24J40_INT_CHANNEL, 0);
    mrf24j40_arch_irq_is_enabled = false;
}

void mrf24j40_arch_select(void)
{
    GPIOSetBitValue(MRF24J40_CSn_PORT, MRF24J40_CSn_PIN, 0);
}

void mrf24j40_arch_deselect(void)
{
    GPIOSetBitValue(MRF24J40_CSn_PORT, MRF24J40_CSn_PIN, 1);
}

__attribute__ ((section(".after_vectors")))
void PIN_INT0_IRQHandler(void)
{
    if ( GPIOPinIntStatus(MRF24J40_INT_CHANNEL) )
    {
        mrf24j40_irq_handler();
    }
    // Clear the interrupt
    GPIOPinIntClear(MRF24J40_INT_CHANNEL);
}

void mrf24j40_arch_spi_write(uint8_t *data, int length) {
    ssp0Send(data, length);
}

void mrf24j40_arch_spi_read(uint8_t *data, int length) {
    ssp0Receive(data, length);
}

void mrf24j40_arch_wfi()
{
    __WFI();
}
