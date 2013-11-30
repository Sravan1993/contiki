#ifndef LPC1347_CDC_H
#define LPC1347_CDC_H

/**
 * @brief Initializes the contiki processes for the lpc1347 usb cdc system.
 *
 * @pre usb_init() must be called prior to this.
 *
 * @param input The callback function that is called, when a character is
 *              received.
 */
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

/**
 * \brief Print one byte
 * \param c The character to print
 *
 * This function is an arch-specific implementation required by the dbg-io
 * API in cpu/arm/common/dbg-io. It prints a stream of bytes over the
 * peripheral used by the platform.
 */
void dbg_putchar(unsigned char c);

#endif //LPC1347_CDC
