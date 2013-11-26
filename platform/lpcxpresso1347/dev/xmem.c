/*
 * Copyright (c) 2006, Swedish Institute of Computer Science
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
 */

/**
 * \file
 *         Device driver for the ST M25P80 40MHz 1Mbyte external memory.
 * \author
 *         Björn Grönvall <bg@sics.se>
 *
 *         Data is written bit inverted (~-operator) to flash so that
 *         unwritten data will read as zeros (UNIX style).
 */


#include "contiki.h"
#include <stdio.h>
#include <string.h>


#include "LPC13Uxx.h"
#include "gpio/gpio.h"
#include "ssp1/ssp1.h"
#include "dev/xmem.h"
#include "dev/watchdog.h"

#if 0
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...) do {} while (0)
#endif

#define  SPI_FLASH_INS_BUFFER1_WRITE 0x84
#define  SPI_FLASH_INS_BUFFER2_WRITE 0x87
#define  SPI_FLASH_INS_BUFFER1_TO_MAIN_W_ERASE 0x83
#define  SPI_FLASH_INS_BUFFER2_TO_MAIN_W_ERASE 0x86
#define  SPI_FLASH_INS_BUFFER1_TO_MAIN_WO_ERASE 0x88
#define  SPI_FLASH_INS_BUFFER2_TO_MAIN_WO_ERASE 0x89
#define  SPI_FLASH_INS_MAIN_TO_BUFFER1 0x53
#define  SPI_FLASH_INS_MAIN_TO_BUFFER2 0x55
#define  SPI_FLASH_INS_PAGE_ERASE  0x81
#define  SPI_FLASH_INS_BLOCK_ERASE 0x50
#define  SPI_FLASH_INS_SECTOR_ERASE 0x7c
#define  SPI_FLASH_INS_CHIP_ERASE_1 0xc7
#define  SPI_FLASH_INS_CHIP_ERASE_2 0x94
#define  SPI_FLASH_INS_CHIP_ERASE_3 0x80
#define  SPI_FLASH_INS_CHIP_ERASE_4 0x9a
#define  SPI_FLASH_INS_READ        0x03 /* continous array read low frequency */
#define  SPI_FLASH_INS_FAST_READ   0x0b /* continous array read high frequency */
#define  SPI_FLASH_INS_DP          0xb9 /* deep power down */
#define  SPI_FLASH_INS_RES         0xab /* resume from deep power down */
#define  SPI_FLASH_INS_STATUS_REGISTER 0xd7 /* read status register */
#define  SPI_FLASH_INS_READ_DEVICE_ID 0x9f /* read status register */

void at45db_arch_select(void)
{
    GPIOSetBitValue(AT45DB_CSn_PORT, AT45DB_CSn_PIN, 0);
}

void at45db_arch_deselect(void)
{
    GPIOSetBitValue(AT45DB_CSn_PORT, AT45DB_CSn_PIN, 1);
}

void at45db_arch_write_protect(void)
{
    GPIOSetBitValue(AT45DB_WPn_PORT, AT45DB_WPn_PIN, 0);
}

void at45db_arch_write_enable(void)
{
    GPIOSetBitValue(AT45DB_WPn_PORT, AT45DB_WPn_PIN, 1);
}

uint32_t
at45db_read_device_id()
{
    uint8_t cmd = SPI_FLASH_INS_READ_DEVICE_ID;
    uint32_t deviceId;

    uint32_t primask = __get_PRIMASK();
    __set_PRIMASK(1);
    at45db_arch_select();

    ssp1Send(&cmd, 1);
    ssp1Receive((uint8_t*) &deviceId, 4);

    at45db_arch_deselect();
    __set_PRIMASK(primask);

    return deviceId;

}

void
at45db_release_from_deep_power_down()
{
    uint8_t cmd = SPI_FLASH_INS_RES;
    uint32_t primask = __get_PRIMASK();
    __set_PRIMASK(1);
    at45db_arch_select();
    ssp1Send(&cmd, 1);
    at45db_arch_deselect();
    __set_PRIMASK(primask);
}

/*---------------------------------------------------------------------------*/
uint8_t
at45db_read_status_register(void)
{
  uint8_t status;

  uint8_t cmd = SPI_FLASH_INS_STATUS_REGISTER;
  uint32_t primask = __get_PRIMASK();
  __set_PRIMASK(1);
  at45db_arch_select();
  ssp1Send(&cmd, 1);
  ssp1Receive(&status, 1);
  at45db_arch_deselect();
  __set_PRIMASK(primask);

  return status;
}
/*---------------------------------------------------------------------------*/
/*
 * Wait for a write/erase operation to finish.
 */
static void
at45db_wait_ready(void)
{
  uint8_t status;
  do {
    status = at45db_read_status_register();
    watchdog_periodic();
  } while((status & 0x80) == 0);		/* READY bit is not set */
}
/*---------------------------------------------------------------------------*/
/*
 * Erase 264 bytes of data. It takes up to 35 milliseconds.
 */
static void
at45db_erase_page(unsigned long offset)
{
  uint8_t cmd[] = {SPI_FLASH_INS_PAGE_ERASE, offset >> 16, offset >> 8, offset};

  at45db_wait_ready();

  uint32_t primask = __get_PRIMASK();
  __set_PRIMASK(1);
  at45db_arch_select();
  at45db_arch_write_enable();
  
  ssp1Send(cmd, sizeof(cmd));
  at45db_arch_deselect();

  at45db_wait_ready();
  at45db_arch_write_protect();
  __set_PRIMASK(primask);
}
/*---------------------------------------------------------------------------*/
/*
 * Initialize external flash *and* SPI bus!
 */
void
xmem_init(void)
{
  ssp1Init();

  GPIOSetDir(AT45DB_CSn_PORT, AT45DB_CSn_PIN, 1);
  GPIOSetDir(AT45DB_WPn_PORT, AT45DB_WPn_PIN, 1);

  at45db_release_from_deep_power_down();
  at45db_wait_ready();

  PRINTF("xmem_init: device id 0x%lx\n", at45db_read_device_id());
}
/*---------------------------------------------------------------------------*/
int
xmem_pread(void *_p, int size, unsigned long offset)
{
  uint8_t *p = _p;
  uint8_t cmd[] = {SPI_FLASH_INS_READ, offset >> 16, offset >> 8, offset};

  at45db_wait_ready();

  ENERGEST_ON(ENERGEST_TYPE_FLASH_READ);

  uint32_t primask = __get_PRIMASK();
  __set_PRIMASK(1);
  at45db_arch_select();


  ssp1Send(cmd, sizeof(cmd));
  ssp1Receive(p, size);

  at45db_arch_deselect();
  __set_PRIMASK(primask);

  ENERGEST_OFF(ENERGEST_TYPE_FLASH_READ);

  return size;
}
/*---------------------------------------------------------------------------*/
static const unsigned char *
program_page(unsigned long offset, const unsigned char *p, int nbytes)
{
  uint8_t bufferWrite[] = {SPI_FLASH_INS_BUFFER1_WRITE, offset >> 16, offset >> 8, offset};
  uint8_t bufferToMain[] = {SPI_FLASH_INS_BUFFER1_TO_MAIN_WO_ERASE, offset >> 16, offset >> 8, offset};
  uint8_t mainToBuffer[] = {SPI_FLASH_INS_MAIN_TO_BUFFER1, offset >> 16, offset >> 8, offset};


  at45db_wait_ready();

  uint32_t primask = __get_PRIMASK();
  __set_PRIMASK(1);
  at45db_arch_select();
  at45db_arch_write_enable();

  /* read from main memory into buffer 1*/
  ssp1Send(mainToBuffer, sizeof(mainToBuffer));
  at45db_arch_deselect();
  at45db_wait_ready();
  
  /* write data into buffer 1 */
  at45db_arch_select();
  ssp1Send(bufferWrite, sizeof(bufferWrite));
  ssp1Send((uint8_t*) p, nbytes);
  at45db_arch_deselect();

  /*write dafrom buffer 1 into main memory */
  at45db_arch_select();
  ssp1Send(bufferToMain, sizeof(bufferToMain));
  at45db_arch_deselect();

  /* wait until write done */
  at45db_wait_ready();

  at45db_arch_write_protect();
  __set_PRIMASK(primask);

  return p;
}
/*---------------------------------------------------------------------------*/
int
xmem_pwrite(const void *_buf, int size, unsigned long addr)
{
  const unsigned char *p = _buf;
  const unsigned long end = addr + size;
  unsigned long i, next_page;

  ENERGEST_ON(ENERGEST_TYPE_FLASH_WRITE);

  for(i = addr; i < end;) {
    next_page = (i | 0xff) + 1;
    if(next_page > end) {
      next_page = end;
    }
    p = program_page(i, p, next_page - i);
    i = next_page;
  }

  ENERGEST_OFF(ENERGEST_TYPE_FLASH_WRITE);

  return size;
}
/*---------------------------------------------------------------------------*/
int
xmem_erase(long size, unsigned long addr)
{
  unsigned long end = addr + size;

  if(size % XMEM_ERASE_UNIT_SIZE != 0) {
    PRINTF("xmem_erase: bad size\n");
    return -1;
  }

  if(addr % XMEM_ERASE_UNIT_SIZE != 0) {
    PRINTF("xmem_erase: bad offset\n");
    return -1;
  }

  for (; addr < end; addr += XMEM_ERASE_UNIT_SIZE) {
    at45db_erase_page(addr);
  }

  return size;
}
/*---------------------------------------------------------------------------*/
