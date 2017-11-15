/**
 * avr-boilerplate
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */

#ifndef AVR_BOILERPLATE_CONF_CLOCK_H_H
#define AVR_BOILERPLATE_CONF_CLOCK_H_H

// 32 MHz clock
#define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_RC32MHZ

/* Fbus = Fsys / (2 ^ BUS_div) */
#define CONFIG_SYSCLK_PSADIV          SYSCLK_PSADIV_4
#define CONFIG_SYSCLK_PSBCDIV         SYSCLK_PSBCDIV_1_1

#endif //AVR_BOILERPLATE_CONF_CLOCK_H_H
