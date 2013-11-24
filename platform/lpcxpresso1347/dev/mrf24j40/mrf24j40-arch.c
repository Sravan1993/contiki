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
 * @param val logical value for asserting reset.
 * FIXME - make this proper doxygen style!
 */
void mrf24j40_arch_hard_reset(int val)
{
    GPIOSetBitValue(MRF24J40_RESET_PORT, MRF24J40_RESET_PIN, val ? 0 : 1);
}

int mrf24j40_arch_init(void)
{
	/* Set the IO pins direction */
    GPIOSetDir(MRF24J40_WAKE_PORT, MRF24J40_WAKE_PIN, 1);
    GPIOSetDir(MRF24J40_RESET_PORT, MRF24J40_RESET_PIN, 1);
    GPIOSetDir(MRF24J40_INT_PORT, MRF24J40_INT_PIN, 0);
    GPIOSetDir(MRF24J40_CSn_PORT, MRF24J40_CSn_PIN, 1);


	/* Set interrupt registers and reset flags */
    /* falling edge*/
    GPIOSetPinInterrupt(MRF24J40_INT_CHANNEL, MRF24J40_INT_PORT, MRF24J40_INT_PIN, 0, 0);

    ssp0Init();
    ssp0ClockFast();
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
    mrf24j40_irq_handler();
}

void mrf24j40_arch_spi_write(uint8_t *data, int length) {
    ssp0Send(data, length);
}

void mrf24j40_arch_spi_read(uint8_t *data, int length) {
    ssp0Receive(data, length);
}
