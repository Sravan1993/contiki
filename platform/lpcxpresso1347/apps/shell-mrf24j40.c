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
 *          Shell function for mrf24j40 commands.
 * \author
 *          Christian Taedcke <hacking@taedcke.com>
 */

#include <string.h>
#include <stdio.h>

#include "contiki.h"
#include "shell.h"
#include "dev/mrf24j40.h"

/*---------------------------------------------------------------------------*/
PROCESS(shell_mrf24j40_process, "mrf24j40");
SHELL_COMMAND(mrf24j40_command,
          "mrf24j40",
          "mrf24j40: display mrf24j40 information",
          &shell_mrf24j40_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_mrf24j40_process, ev, data)
{
  
  char str_buf[35];
  uint8_t i;
  int written;
  char* cursor = str_buf;
  
  PROCESS_BEGIN();
  i = mrf24j40_get_status();
  sprintf(cursor, "0x%x", i);
  shell_output_str(&mrf24j40_command, "Status: ", str_buf);
  i = mrf24j40_get_rssi();
  sprintf(cursor, "0x%x", i);
  shell_output_str(&mrf24j40_command, "RSSI: ", str_buf);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_mrf24j40_init(void)
{
  shell_register_command(&mrf24j40_command);
}
/*---------------------------------------------------------------------------*/
/** @} */
