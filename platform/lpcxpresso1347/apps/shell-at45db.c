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
 *          Shell function for AT45DB flash.
 * \author
 *          Christian Taedcke <hacking@taedcke.com>
 */

#include <string.h>
#include <stdio.h>

#include "contiki.h"
#include "shell.h"
#include "contiki-net.h"
#include "dev/xmem.h"

/*---------------------------------------------------------------------------*/
PROCESS(shell_at45db_process, "at45db");
SHELL_COMMAND(at45db_command,
          "at45db",
          "at45db {id|status|read}: get flash content",
          &shell_at45db_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_at45db_process, ev, data)
{
  
  char str_buf[35];
  
  PROCESS_BEGIN();

  if(data == NULL) {
    shell_output_str(&at45db_command,
             "at45db {id|status|read|write}: a command must be specified", "");
    PROCESS_EXIT();
  }
  
  if (strcmp(data,"id") == 0) {
    
      uint32_t deviceId = at45db_read_device_id();
    
    snprintf(str_buf,sizeof(str_buf),"%lx", deviceId);

    shell_output_str(&at45db_command, "Device id: ", str_buf);
    
  } else if (strcmp(data,"status") == 0) {

      uint32_t status = at45db_read_status_register();

    snprintf(str_buf,sizeof(str_buf),"%lx", status);

    shell_output_str(&at45db_command, "Status: ", str_buf);

  } else if (strcmp(data,"read")==0) {
      uint8_t i;
      int written;
      char* cursor = str_buf;
      uint8_t buffer[8];
      xmem_pread(buffer, sizeof(buffer), 0);
      for (i=0; i < sizeof(buffer); i++) {
          written = sprintf(cursor, "%x ", buffer[i]);
          cursor += written;
      }

     shell_output_str(&at45db_command, "Buffer ", str_buf);
    
  } else if (strcmp(data,"write")==0) {
      uint8_t buffer[] = {1, 2, 3, 4};
      xmem_pwrite(buffer, sizeof(buffer), 0);
      shell_output_str(&at45db_command, "Data written", "");

  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_at45db_init(void)
{
  shell_register_command(&at45db_command);
}
/*---------------------------------------------------------------------------*/
/** @} */
