/**
 * avr-boilerplate ee.h
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */

#include "conf_application.h"
#include "asf.h"
#include <stdio.h>

int main( ) {

  board_init();

  board_init();
  sysclk_init();
  sleepmgr_init();
  irq_initialize_vectors();
  cpu_irq_enable();
  gfx_mono_init();

  // Enable the back light of the LCD
  ioport_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

  // Light up LED 1, wait for button press.
  ioport_set_pin_low(LED1_PIN);

  // Light up LED 2, wait for button press.
  ioport_set_pin_low(LED2_PIN);

  char out_str[OUTPUT_STR_SIZE];

  snprintf(out_str, OUTPUT_STR_SIZE, "Voltage: %4d mV", 102);

  gfx_mono_draw_string(out_str, 0, 0, &sysfont);

  return 0;
}