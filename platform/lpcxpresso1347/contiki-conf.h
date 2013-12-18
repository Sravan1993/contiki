#ifndef __CONTIKI_CONF_H__CDBB4VIH3I__
#define __CONTIKI_CONF_H__CDBB4VIH3I__

#include <stdint.h>

#define CCIF
#define CLIF

#define WITH_UIP 1
#define WITH_ASCII 1

#define CLOCK_CONF_SECOND 100

/* These names are deprecated, use C99 names. */
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int8_t s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;

typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

/*
 * rtimer.h typedefs rtimer_clock_t as unsigned short. We need to define
 * RTIMER_CLOCK_LT to override this
 */
typedef uint32_t rtimer_clock_t;
#define RTIMER_CLOCK_LT(a,b)     ((int32_t)((a)-(b)) < 0)


#ifndef BV
#define BV(x) (1<<(x))
#endif

/* uIP configuration */
#define UIP_CONF_LLH_LEN         0
#define UIP_CONF_BROADCAST       1
#define UIP_CONF_LOGGING 1
#define UIP_CONF_BUFFER_SIZE 116

#define UIP_CONF_TCP_FORWARD 1

/* Radio and 802.15.4 params */
/* 802.15.4 radio channel */
#define RF_CHANNEL						23
/* 802.15.4 PAN ID */
#define IEEE802154_CONF_PANID					0x1235

#define PROFILE_CONF_ON 0
#ifndef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON 1
#endif /* ENERGEST_CONF_ON */

#ifdef WITH_UIP6
#define NETSTACK_CONF_NETWORK                   sicslowpan_driver
#define NETSTACK_CONF_FRAMER                    framer_802154
#define NETSTACK_CONF_MAC                       nullmac_driver
#define NETSTACK_CONF_RDC                       nullrdc_driver
#define NETSTACK_CONF_RADIO                     mrf24j40_driver
#define RIMEADDR_CONF_SIZE                      8
#else
#define NETSTACK_CONF_NETWORK                   rime_driver
#define NETSTACK_CONF_FRAMER                    framer_802154
#define NETSTACK_CONF_MAC                       nullmac_driver
#define NETSTACK_CONF_RDC                       nullrdc_driver
#define NETSTACK_CONF_RADIO                     mrf24j40_driver
#define RIMEADDR_CONF_SIZE                      8
#endif

#define RDC_CONF_HARDWARE_CSMA                  1

#define CONTIKIMAC_CONF_WITH_CONTIKIMAC_HEADER  0

#ifdef WITH_UIP6
#define UIP_CONF_ROUTER                         1
#ifndef UIP_CONF_IPV6_RPL
#define UIP_CONF_IPV6_RPL                       1
#endif /* UIP_CONF_IPV6_RPL */

/* IPv6 configuration options */
#define UIP_CONF_IPV6                           1
#define NBR_TABLE_CONF_MAX_NEIGHBORS                    20 /* number of neighbors */
#define UIP_CONF_DS6_ROUTE_NBU                  20 /* number of routes */
#define UIP_CONF_ND6_SEND_RA                    0
#define UIP_CONF_ND6_REACHABLE_TIME             600000
#define UIP_CONF_ND6_RETRANS_TIMER              10000


#define UIP_CONF_BUFFER_SIZE                    240

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                       16
#endif /* QUEUEBUF_CONF_NUM */

/* UDP configuration options */
#define UIP_CONF_UDP                            1
#define UIP_CONF_UDP_CHECKSUMS                  1
#define UIP_CONF_UDP_CONNS                      10

/* 6lowpan options (for ipv6) */
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD   63
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                    1
#define SICSLOWPAN_CONF_MAXAGE                  8
#endif /* SICSLOWPAN_CONF_FRAG */

/* General configuration options */
#define UIP_CONF_STATISTICS                     0
#define UIP_CONF_LOGGING                        0
#define UIP_CONF_BROADCAST                      1
#define UIP_CONF_LLH_LEN                        0
#define UIP_CONF_LL_802154                      1
#endif


#define XMEM_ERASE_UNIT_SIZE (264L)

#define CFS_CONF_OFFSET_TYPE    long

/* Use the first 64k of external flash for node configuration */
#define NODE_ID_XMEM_OFFSET     (0 * XMEM_ERASE_UNIT_SIZE)

/* Use the second 64k of external flash for codeprop. */
#define EEPROMFS_ADDR_CODEPROP  (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_XMEM_CONF_OFFSET    (2 * XMEM_ERASE_UNIT_SIZE)
#define CFS_XMEM_CONF_SIZE      (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_RAM_CONF_SIZE 4096


#define AT45DB_CSn_PORT   1
#define AT45DB_CSn_PIN    19
#define AT45DB_WPn_PORT   0
#define AT45DB_WPn_PIN    23

#ifndef NODE_ID
#define NODE_ID 1
#endif /* NODE_ID */

#if NODE_ID == 1
#define MRF24J40_PAN_COORDINATOR
#endif /* NODE_ID == 1 */

#define EEPROM_CONF_SIZE        (4032)

/* include the project config */
/* PROJECT_CONF_H might be defined in the project Makefile */
#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */


#endif /* __CONTIKI_CONF_H__CDBB4VIH3I__ */
