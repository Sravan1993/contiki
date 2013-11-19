#include <stdint.h>
#include <stdio.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include <dev/leds.h>
#include <usb/usbd.h>
#include <gpio/gpio.h>
#include "dev/serial-line.h"

unsigned int idle_count = 0;


int
main()
{
    /// \todo ctae setup usb for printf
  //dbg_setup_uart();
  
  clock_init();
  GPIOInit();
  /* Led initialization */
  leds_init();

  usb_init();

  lpc1347_cdc_init(serial_line_input_byte);
  //leds_off(LEDS_ALL);
  leds_toggle(LEDS_RED);
  process_init();
//leds_off(LEDS_ALL);
  leds_toggle(LEDS_RED);
  process_start(&etimer_process, NULL);
  leds_toggle(LEDS_RED);
  //leds_off(LEDS_ALL);
  autostart_start(autostart_processes);
  leds_toggle(LEDS_RED);
  //leds_off(LEDS_ALL);
  printf("Processes running\n");
  leds_toggle(LEDS_RED);
  printf("Starting contiki... \n");
  //leds_off(LEDS_ALL);
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




