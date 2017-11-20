/**
 * avr-boilerplate
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */

#ifndef AVR_BOILERPLATE_CONF_APPLICATION_H_H
#define AVR_BOILERPLATE_CONF_APPLICATION_H_H

// Application
#define LED1_PIN              LED0_GPIO
#define LED2_PIN              LED1_GPIO
#define OUTPUT_STR_SIZE       64

// QDEC
#define QDEC0_PORT             &PORTC
#define QDEC0_PIN_BASE         2
#define QDEC0_TC               &TCC0
#define QDEC0_FREQ_TC          &TCC1

#define QDEC1_PORT             &PORTD
#define QDEC1_PIN_BASE         2
#define QDEC1_TC               &TCD0
#define QDEC1_FREQ_TC          &TCD1

#define QDEC2_PORT             &PORTE
#define QDEC2_PIN_BASE         2
#define QDEC2_TC               &TCE0
#define QDEC2_FREQ_TC          &TCE1

// TWI
#define TWI_SPEED             100000
#define TWI_CHIP              0x20
#define TWI_DEVICE            &TWIC
#define TWI_DATA_LEN          8

// TC
#define TIMER_QDEC_READ         &TCD0
#define TIMER_QDEC_READ_PERIOD  2343

// DMA
#define TWI_DMA_CHANNEL       0

#endif //AVR_BOILERPLATE_CONF_APPLICATION_H_H
