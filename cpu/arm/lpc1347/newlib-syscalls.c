//#include <debug-uart.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


bool usb_cdc_putc(uint8_t c);
bool usb_cdc_getc(uint8_t *c);
bool usb_cdc_isConnected();

int
_open(const char *name, int flags, int mode) {
  errno = ENOENT;
  return -1;
}

int
_close(int file)
{
  if (file == 1 || file == 2) {
    //dbg_drain();
    return 0;
  }
  errno = EBADF;
  return -1;
}

int
_isatty(int file)
{
  if (file >= 0 && file <= 2) return 1;
   return 0;
}


int
_read(int file, char *ptr, int len){
    int n;
    int num = 0;
    switch (file) {
    case 0:
        for (n = 0; n < len; n++) {
            while (usb_cdc_isConnected() && !usb_cdc_getc(ptr)) {
                //ASM("NOP");
            }
            ptr++;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}
 	

int
_write(int file, const char *ptr, int len){
    int n;
    switch (file) {
    case 0:
    case 1:
        for (n = 0; n < len; n++) {
            while (usb_cdc_isConnected() && !usb_cdc_putc(*ptr)) {
                //ASM("NOP");
            }
            ptr++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

int
_lseek(int file, int ptr, int dir){
    return 0;
}

int
_fstat(int file, struct stat *st) {
  if (file >= 0 && file <= 2) {
    st->st_mode = S_IFCHR;
    return 0;
  }
  errno = EBADF;
  return -1;
}

int
_stat(char *file, struct stat *st) {
  errno = ENOENT;
  return -1;
}
#if 0
caddr_t
_sbrk(int incr)
{
  extern char __fixed_heap_start;		/* Defined by the linker */
  extern char __fixed_heap_end; //__heap_end__;		/* Defined by the linker */
  static char *heap_end = &__fixed_heap_start;
  char *prev_heap_end;
 
  prev_heap_end = heap_end;
  if (heap_end + incr > &__fixed_heap_end) {
    printf("Heap full (requested %d, available %d)\n",
        incr, (int)(&__fixed_heap_end - heap_end));
    errno = ENOMEM;
    return (caddr_t)-1;
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
#endif
static unsigned char __HEAP_START[1024*2];
caddr_t _sbrk ( int incr )
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  //printf ("_sbrk (%d)\n\r", incr);
  if (heap == NULL) {
    heap = (unsigned char *)__HEAP_START;
  }
  prev_heap = heap;
  /* check removed to show basic approach */
  if ((heap + incr) > (__HEAP_START + sizeof(__HEAP_START))) {
    prev_heap = NULL;
  } else {
    heap += incr;
  }
  if (prev_heap == NULL) {
    //printf ("_sbrk %d return %p\n\r", incr, prev_heap);
  }
  return (caddr_t) prev_heap;
}


int
fsync(int fd)
{
  if (fd == 1 || fd == 2) {
    //dbg_drain();
    return 0;
  }
  if (fd == 0) return 0;
  errno = EBADF;
  return -1;
}

void
_exit(int status)
{
  while(1);
}

void
_abort()
{
  while(1);
}

void
_kill()
{
  while(1);
}

pid_t
_getpid(void)
{
  return 1;
}

const unsigned long
bkpt_instr = 0xe1200070;
