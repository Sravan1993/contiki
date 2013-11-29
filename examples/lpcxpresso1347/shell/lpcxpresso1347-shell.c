/*
 * Copyright (c) 2013, Christian Taedcke
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
 *         lpcxpresso1347-specific Contiki shell
 * \author
 *         Christian Taedcke <hacking@taedcke.com>
 *
 */

#include "contiki.h"
#include "serial-shell.h"
#include "shell-at45db.h"
#include "shell-sysinfo.h"
#include "shell-mrf24j40.h"

/*---------------------------------------------------------------------------*/
PROCESS(lpcxpresso1347_shell_process, "LPCXPRESSO1347 Contiki shell");
AUTOSTART_PROCESSES(&lpcxpresso1347_shell_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lpcxpresso1347_shell_process, ev, data)
{
  PROCESS_BEGIN();

  serial_shell_init();
  shell_blink_init();
  shell_ps_init();
  shell_reboot_init();
  shell_text_init();
  shell_time_init();
  shell_at45db_init();
  shell_sysinfo_init();
  shell_mrf24j40_init();
  shell_rime_init();
  shell_rime_netcmd_init();

#if COFFEE
  shell_coffee_init();
  shell_file_init();
#endif
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
