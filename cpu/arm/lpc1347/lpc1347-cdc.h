#ifndef LPC1347_CDC_H
#define LPC1347_CDC_H

void lpc1347_cdc_init(int (*input) (unsigned char c));

/**
 * \brief Print a stream of bytes
 * \param seq A pointer to the stream
 * \param len The number of bytes to print
 * \return The number of printed bytes
 *
 * This function is an arch-specific implementation required by the dbg-io
 * API in cpu/arm/common/dbg-io. It prints a stream of bytes over the
 * peripheral used by the platform.
 */
unsigned int dbg_send_bytes(const unsigned char *seq, unsigned int len);

/// \todo ctae doc
void dbg_putchar(unsigned char c);

#endif //LPC1347_CDC
