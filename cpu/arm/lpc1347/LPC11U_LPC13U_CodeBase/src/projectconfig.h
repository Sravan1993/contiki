/**************************************************************************/
/*!
    Stripped down version of original project config with inlines information
    for the lxpxpresso LPC1347 board.

    @file     projectconfig.h
    @author   K. Townsend (microBuilder.eu)

    @brief    Indicates which board should be used during the build process
    @ingroup  Board/HW Abstration Layer

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2012, 2013 K. Townsend
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _PROJECTCONFIG_H_
#define _PROJECTCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sysdefs.h"
#include "errors.h"
#include "asserts.h"

#define CFG_MCU_LPC1347FBD48
#define CFG_MCU_FAMILY_LPC13UXX
#include "cmsis/LPC13Uxx.h"

#define CFG_ENABLE_USB

/*=========================================================================
    USB

    CFG_USB_STRING_MANUFACTURER Manufacturer name that will appear in the
                                device descriptor during USB enumeration
    CFG_USB_STRING_PRODUCT      Product name that will appear in the
                                device descriptor during USB enumeration
    CFG_USB_VENDORID            16-bit USB vendor ID
    USB_PRODUCT_ID              Define this to set a custom product ID
                                if you do not wish to use the 'auto'
                                product ID feature
    CFG_CDC                     Enable USB CDC support
    CFG_USB_HID_KEYBOARD        Enable USB HID keyboard emulation
    CFG_USB_HID_MOUSE           Enable USB HID mouse emulation for a five
                                button 'Windows' mouse with scroll wheels
    CFG_USB_HID_GENERIC         Enable USB HID Generic support for custom
                                in and out reports, with report size set
                                via CFG_USB_HID_GENERIC_REPORT_SIZE
    CFG_USB_MSC                 Enable USB Mass Storage support, pointing
                                to the SD card reader (requires mmc.c from
                                the FATFS drivers, but doesn't use FATFS)


    You can combine more than one USB class below and they will be
    automatically combined in a USB composite device within the limit of
    available USB endpoints.  The USB Product ID is calculated automatically
    based on the combination of classes defined below.

    NOTE: Windows requires the .inf file in '/core/usb' for CDC support
    -----------------------------------------------------------------------*/
    #ifdef CFG_ENABLE_USB
      #define CFG_USB_STRING_MANUFACTURER       "Contiki"
      #define CFG_USB_STRING_PRODUCT            "LPC1347 LPCXpresso"
      #define CFG_USB_VENDORID                  (0x1FC9)

      #define CFG_USB_CDC

      // #define CFG_USB_HID_KEYBOARD
// #define CFG_USB_HID_MOUSE
    //#define CFG_USB_HID_GENERIC
    //#define CFG_USB_HID_GENERIC_REPORT_SIZE (64)

    // #define CFG_USB_MSC

    // #define CFG_USB_CUSTOM_CLASS

    #if (defined(CFG_USB_CDC)       || defined(CFG_USB_HID_KEYBOARD) || \
         defined(CFG_USB_HID_MOUSE) || defined(CFG_USB_HID_GENERIC)  || \
         defined(CFG_USB_MSC)       || defined(CFG_USB_CUSTOM_CLASS))
      #define CFG_USB
      #if defined(CFG_USB_HID_KEYBOARD) || defined(CFG_USB_HID_MOUSE) || defined(CFG_USB_HID_GENERIC)
        #define CFG_USB_HID
        #if defined(CFG_USB_HID_GENERIC) && (CFG_USB_HID_GENERIC_REPORT_SIZE > 64)
          #error "CFG_USB_HID_GENERIC_REPORT_SIZE exceeds the maximum value of 64 bytes (based on USB specs 2.0 for 'Full Speed Interrupt Endpoint Size')"
        #endif
      #endif
    #endif
  #endif
/*=========================================================================*/


/*=========================================================================
    PRINTF REDIRECTION
    -----------------------------------------------------------------------

    CFG_PRINTF_MAXSTRINGSIZE  Maximum size of string buffer for printf
    CFG_PRINTF_UART           Will cause all printf statements to be
                              redirected to UART
    CFG_PRINTF_USBCDC         Will cause all printf statements to be
                              redirect to USB Serial
    CFG_PRINTF_NEWLINE        This is typically "\r\n" for Windows or
                              "\n" for *nix

    Note: If no printf redirection definitions are present, all printf
    output will be ignored.
    -----------------------------------------------------------------------*/
    #define CFG_PRINTF_MAXSTRINGSIZE    (255)

    // #define CFG_PRINTF_UART
    // #define CFG_PRINTF_USBCDC
    // #define CFG_PRINTF_DEBUG

    #ifdef CFG_PRINTF_DEBUG
      #define CFG_PRINTF_NEWLINE          "\n"
    #else
      #define CFG_PRINTF_NEWLINE          "\r\n"
    #endif
/*=========================================================================*/

/*=========================================================================
    SPI
    -----------------------------------------------------------------------

    CFG_SSP_SCK0_LOCATION     The location of the SCK pin for SSP0
    CFG_SSP_MISO1_LOCATION    The location of the MISO1 pin for SSP1
    CFG_SSP_MOSI1_LOCATION    The location of the MOSI1 pin for SSP1
    CFG_SSP_SCK1_LOCATION     The location of the SCK pin for SSP1

    -----------------------------------------------------------------------*/
    #define CFG_SSP_SCK0_0_6            (6)     // Used by USBConnect
    #define CFG_SSP_SCK0_0_10           (10)    // Used by SWD
    #define CFG_SSP_SCK0_1_29           (29)

    #define CFG_SSP_MISO1_0_22          (22)
    #define CFG_SSP_MISO1_1_21          (21)
    #define CFG_SSP_MOSI1_0_21          (21)
    #define CFG_SSP_MOSI1_1_22          (22)
    #define CFG_SSP_SCK1_1_15           (15)
    #define CFG_SSP_SCK1_1_20           (20)

    // Select the appropriate pin locations here
    #define CFG_SSP_SCK0_LOCATION       (CFG_SSP_SCK0_1_29)
    #define CFG_SSP_MISO1_LOCATION      (CFG_SSP_MISO1_0_22)
    #define CFG_SSP_MOSI1_LOCATION      (CFG_SSP_MOSI1_0_21)
    #define CFG_SSP_SCK1_LOCATION       (CFG_SSP_SCK1_1_15)

/// \todo ctae set pol for at45 and mrf24
    // Set the phase and polarity for SSP0 and SSP1
    #define CFG_SSP_CPOL0               (0) /* The MRF24J40 supports SPI (mode 0,0) which requires that SCK idles in a low state. */
    #define CFG_SSP_CPHA0               (0)
    #define CFG_SSP_CPOL1               (0)
    #define CFG_SSP_CPHA1               (0)
/*=========================================================================*/

/*=========================================================================
    EEPROM
    -----------------------------------------------------------------------
    EEPROM is used to persist certain user modifiable values to make
    sure that these changes remain in effect after a reset or hard
    power-down.
    -----------------------------------------------------------------------*/

#define CFG_EEPROM_SIZE                   (4032)
/*=========================================================================*/

#ifdef __cplusplus
}
#endif

#endif
