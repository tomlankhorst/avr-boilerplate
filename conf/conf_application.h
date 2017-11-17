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
#define QDEC_PORT             &PORTC
#define QDEC_PIN_BASE         2
#define QDEC_TC               &TCC0
#define QDEC_FREQ_TC          &TCC1

// TWI
#define TWI_SPEED             100000
#define TWI_CHIP              0x20
#define TWI_DEVICE            &TWIC
#define TWI_DATA_LEN          8

#endif //AVR_BOILERPLATE_CONF_APPLICATION_H_H
