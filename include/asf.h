/**
 * avr-boilerplate
 *
 * This file specifies all ASF dependencies
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */

#ifndef AVR_BOILERPLATE_ASF_H
#define AVR_BOILERPLATE_ASF_H

// From module: CPU specific features
#include <ccp.h>
#include <xmega_reset_cause.h>

// From module: Delay routines
#include <delay.h>

// From module: DMA - Direct Memory Access
#include <dma.h>

// From module: GFX Monochrome - Monochrome Graphic Library
#include <gfx_mono.h>

// From module: GFX Monochrome - System Font
#include <sysfont.h>

// From module: GPIO - General purpose Input/Output
#include <gpio.h>

// From module: Generic board support
#include <board.h>

// From module: IOPORT - General purpose I/O service
#include <ioport.h>

// From module: Interrupt management - XMEGA implementation
#include <interrupt.h>

// From module: NVM - Non Volatile Memory
#include <nvm.h>

// From module: PMIC - Programmable Multi-level Interrupt Controller
#include <pmic.h>

// From module: Part identification macros
#include <parts.h>

// From module: TC - Timer Counter
#include <tc.h>

// From module: TWI - Two-Wire Interface - XMEGA implementation
#include <twi_slave.h>
#include <xmega_twi/twi_slave.h>

// From module: Quadrature Decoder - XMEGA A/B/C/D Implementation
#include <qdec.h>

// From module: SPI - XMEGA implementation
#include <usart_spi.h>
#include <xmega_usart_spi/usart_spi.h>

// From module: ST7565R LCD controller
#include <st7565r.h>

// From module: Sleep Controller driver
#include <sleep.h>

// From module: Sleep manager - XMEGA A/AU/B/D implementation
#include <sleepmgr.h>
#include <xmega/sleepmgr.h>

// From module: System Clock Control - XMEGA A1U/A3U/A3BU/A4U/B/C implementation
#include <sysclk.h>

// From module: USART - Universal Synchronous/Asynchronous Receiver/Transmitter
#include <usart.h>

// From module: USB CDC Protocol
#include <usb_protocol_cdc.h>

// From module: USB Device CDC (Single Interface Device)
#include <udi_cdc.h>

// From module: USB Device CDC Standard I/O (stdio) - XMEGA implementation
#include <stdio_usb.h>

// From module: USB Device Stack Core (Common API)
#include <udc.h>
#include <udd.h>

// From module: XMEGA compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: XMEGA-A3BU Xplained LED support enabled
#include <led.h>

#endif //AVR_BOILERPLATE_ASF_H
