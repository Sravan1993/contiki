#include "sys/process.h"

#include "lpc1347-cdc.h"
#include "usb/usb_cdc.h"

static process_event_t cdc_event = PROCESS_CONF_NUMEVENTS;

static int (*cdc_input_handler) (unsigned char c);

PROCESS(lpc1347_cdc_process, "lpc1347cdc");

/**
 * Called when an cdc event occurs
 */
void
usb_cdc_recv_isr(void)
{
    if (cdc_event != PROCESS_CONF_NUMEVENTS) {
        process_post(&lpc1347_cdc_process, cdc_event, NULL);
    }
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lpc1347_cdc_process, ev, data)
{
  static uint8_t buffer;

  PROCESS_BEGIN();

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(ev == cdc_event);
    while(usb_cdc_getc(&buffer)) {
        cdc_input_handler(buffer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

void
lpc1347_cdc_init(int (*input) (unsigned char c))
{
    cdc_input_handler = input;
    cdc_event = process_alloc_event();
    process_start(&lpc1347_cdc_process, NULL);
}


unsigned int
dbg_send_bytes(const unsigned char *s, unsigned int len)
{
    return usb_cdc_send(s, len);
}

void
dbg_putchar(unsigned char c)
{
    usb_cdc_putc(c);
}
