#include <stdio.h>
#include <debug-uart.h>
#include <string.h>

int
puts(const char *str)
{
    /// \todo ctae reenable the following two lines
  //dbg_send_bytes((unsigned char*)str, strlen(str));
  //dbg_putchar('\n');
  return 0;
}
