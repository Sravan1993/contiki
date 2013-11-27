#include <stdint.h>
#include <stdio.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include <dev/leds.h>
#include "usb/usbd.h"
#include <gpio/gpio.h>
#include "dev/serial-line.h"
#include "netstack.h"
#include "init-net.h"
#include <lib/random.h>
#include "dev/xmem.h"

#define STARTUP_CONF_VERBOSE 0

#if STARTUP_CONF_VERBOSE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

unsigned int idle_count = 0;

int
main()
{ 
  clock_init();
  GPIOInit();
  leds_init();
  xmem_init();

  random_init(NODE_ID);
  process_init();

  usb_init();
  lpc1347_cdc_init(serial_line_input_byte);
  serial_line_init();

  PRINTF(CONTIKI_VERSION_STRING "\n");
  PRINTF("LPCXPRESSO1347ßn");

  PRINTF(" Net: ");
  PRINTF("%s\n", NETSTACK_NETWORK.name);
  PRINTF(" MAC: ");
  PRINTF("%s\n", NETSTACK_MAC.name);
  PRINTF(" RDC: ");
  PRINTF("%s\n", NETSTACK_RDC.name);

  leds_toggle(LEDS_RED);

  leds_toggle(LEDS_RED);
  process_start(&etimer_process, NULL);
  leds_toggle(LEDS_RED);

  init_net(NODE_ID);

   autostart_start(autostart_processes);
  leds_toggle(LEDS_RED);
  leds_toggle(LEDS_RED);
  while(1) {
    do {
    } while(process_run() > 0);
    idle_count++;
    /* Idle! */
    /* Stop processor clock */
    /* asm("wfi"::); */ 
    //leds_off(LEDS_ALL);
  }
  return 0;
}




