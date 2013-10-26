/*
 * Copyright (c) 2013, Christian Taedcke <hacking@taedcke.com>
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         MBxxx-specific Contiki shell commands
 * \author
 *         Christian Taedcke <hacking@taedcke.com>
 */

#include "contiki.h"
#include "shell-mbxxx.h"

#include "sensors.h"

#include "dev/stm32w-systick.h"
#include "dev/stm32w-radio.h"
#include "hal/micro/micro-common.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(shell_sleep_process, "sleep");
SHELL_COMMAND(sleep_command,
	      "sleep",
	      "sleep [seconds]: how many seconds should the mcu deep sleep",
	      &shell_sleep_process);
/*---------------------------------------------------------------------------*/
/* The value that will be load in the SysTick value register. */
#define RELOAD_VALUE 24000-1    /* 1 ms with a 24 MHz clock */

void sleep(unsigned int seconds)
{
  int32u quarter_seconds = seconds * 4;
  //uint8_t radio_on;
  uint32_t elapsed_quarter_seconds;
  uint32_t wakeInfo;
  const struct sensors_sensor *sensor;

  //TODO: Enabling the radio at the end of this function keeps it on, maybe sth has to be done in contikimac?
  //radio_on = stm32w_radio_is_on();
  stm32w_radio_deinit();

  //TODO: Move tis functionality into sensors.c
  //Deactivate sensors before sleep, reactivating them at the end of this function
  for (sensor = sensors_first(); sensor; sensor = sensors_next(sensor)) {
    SENSORS_DEACTIVATE(*sensor);
  }
  process_exit(&sensors_process);

  halPowerDown();

  ENERGEST_OFF(ENERGEST_TYPE_CPU);
  halSleepForQsWithOptions(&quarter_seconds, 0);
  elapsed_quarter_seconds = seconds * 4 - quarter_seconds;

  ATOMIC(

    halPowerUp();

    // Update OS system ticks.
    clock_adjust_ticks(elapsed_quarter_seconds * CLOCK_SECOND / 4);

    if(etimer_pending()) {
      etimer_request_poll();
    }

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_SetReload(RELOAD_VALUE);
    SysTick_ITConfig(ENABLE);
    SysTick_CounterCmd(SysTick_Counter_Enable);

  )

  uart1_init(115200);
  leds_init();
  rtimer_init();

  //hack due to reset of rtimer
  energest_total_time[ENERGEST_TYPE_LPM2].current += RTIMER_ARCH_SECOND * elapsed_quarter_seconds / 4;
  ENERGEST_ON(ENERGEST_TYPE_CPU);

  wakeInfo = halGetWakeInfo();
  if(wakeInfo != 0x81000000) {
    printf("Error while sleeping: %04x\r\n", wakeInfo);
  }

  process_start(&sensors_process, NULL);
  for (sensor = sensors_first(); sensor; sensor = sensors_next(sensor)) {
    SENSORS_ACTIVATE(*sensor);
  }

  stm32w_radio_driver.init();
  //if(radio_on) {
  //    stm32w_radio_driver.on();
  //}
}
/*--------------------------------------------------------------------------*/
PROCESS_THREAD(shell_sleep_process, ev, data)
{
  struct {
    uint32_t sleepTime;
  } msg;
  unsigned long newtime = 0;
  const char *nextptr;

  PROCESS_BEGIN();

  if(data != NULL) {
    newtime = shell_strtolong(data, &nextptr);
    if(data == nextptr) {
        newtime = 0;
    }
  }

  if (newtime == 0) {
      newtime = 1;
  }

  char buffer[30];
  sprintf(buffer, "sleeping %lu seconds", newtime);
  shell_output_str(&sleep_command, buffer, "");
  sleep(newtime);

  msg.sleepTime = newtime;

  shell_output(&sleep_command, &msg, sizeof(msg), "", 0);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_mbxxx_init(void)
{
  shell_register_command(&sleep_command);
}
/*---------------------------------------------------------------------------*/

