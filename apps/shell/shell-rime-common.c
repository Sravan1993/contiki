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
 *         Contains simple tool function common to multiple rime shells.
 * \author
 *         Christian Taedcke
 */

#include "contiki.h"
#include "contiki-conf.h"
#include "shell.h"
#include "shell-rime-common.h"

#include "net/rime.h"

#include <stdio.h>

uint8_t print_rime_address(const unsigned char* rime_address, char* as_string, unsigned char as_string_size)
{
  unsigned int i;
  int free_space;
  int chars_written, chars_written_total;

  free_space = as_string_size;
  chars_written_total = 0;
  for(i = 0; i < RIMEADDR_SIZE - 1; i++) {
    chars_written = snprintf(as_string + chars_written_total, free_space, "%d.", rime_address[i]);
    chars_written_total += chars_written;
    free_space -= chars_written;
    if(free_space <= 0){
      return 1;
    }
  }
  snprintf(as_string + chars_written_total, free_space, "%d", rime_address[i]);

  return 0;

}

uint8_t parse_rime_address(const char* input, char* as_string, unsigned char as_string_size, unsigned char* rime_address)
{
  const char *nextptr;
  const char *cursor;
  unsigned int i;

  memset(as_string, 0, as_string_size);
  cursor = input;
  for (i = 0; i < RIMEADDR_SIZE; i++) {
    rime_address[i] = shell_strtolong(cursor, &nextptr);
    if(nextptr == cursor || ((*nextptr != '.') && (i != RIMEADDR_SIZE - 1))) {

      return 1;
    }
    cursor = ++nextptr;
  }

  return print_rime_address(rime_address, as_string, as_string_size);
}
