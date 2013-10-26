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

#ifndef __SHELL_RIME_COMMON_H__
#define __SHELL_RIME_COMMON_H__

/**
 * TODO: doc
 * @brief Parses the given rime address from the given string.
 * @param input The user input from the shell. Must start with the rime address.
 * @param as_string The rime address a a string is printed into this buffer.
 * @param as_string_size The buffer size of as_string.
 * @param rime_address The rime address is saved into this buffer. It must be of the size RIMEADDR_SIZE.
 * @return The success of parsing the rime address.
 *  \retval 0 if the rime address was parsed without any error.
 *  \retval 1 if an error occured. In this case the output parameter are invalid.
 */
uint8_t parse_rime_address(const char* input, char* as_string, unsigned char as_string_size, unsigned char* rime_address);

/** TODO: doc
 * @brief print_rime_address
 * @param rime_address
 * @param as_string
 * @param as_string_size
 * @return
 */
uint8_t print_rime_address(const unsigned char* rime_address, char* as_string, unsigned char as_string_size);

#endif /* __SHELL_RIME_COMMON_H__ */
