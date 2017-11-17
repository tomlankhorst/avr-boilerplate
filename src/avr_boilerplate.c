/**
 * avr-boilerplate
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */


#include <stdio.h>
#include <asf.h>

#include "conf_build.h"
#include "conf_application.h"

#include <string.h>

#define QUADRATURE_RESOLUTION (600)


typedef struct Kiwi_Sensor {
  uint8_t CONFIG;
  uint8_t STATUS;
  uint16_t ENC_POS[3];
  uint16_t ENC_FRQ[3];
  uint8_t DIR;
} Kiwi_Sensor_t;

Kiwi_Sensor_t kiwi = {
    .CONFIG = 0x01,
    .STATUS = 0x02,
    .ENC_POS = { 0x1010, 0x2020, 0x3030 },
    .ENC_FRQ = { 0x1010, 0x2020, 0x3030 },
    .DIR = ( 1 << 7 | 0 << 6 | 1 << 5 )
};

uint8_t recv_data[TWI_DATA_LEN] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

TWI_Slave_t slave;

static void slave_process(void) {
  /** int i;
  for(i = 0; i < TWI_DATA_LEN; i++) {
    recv_data[i] = slave.receivedData[i];
  } */

  if( slave.status == TWIS_RESULT_BUFFER_OVERFLOW ) {
    ioport_set_pin_high(LED1_PIN);
  }

  // got one.
  memcpy( (void*)slave.sendData, (((void*)&kiwi) + slave.receivedData[0]), TWIS_SEND_BUFFER_SIZE);

}

ISR(TWIC_TWIS_vect) {
  TWI_SlaveInterruptHandler(&slave);
  recv_data[TWI_DATA_LEN-1] = 0x00; // always make 0 term
}

int main( ) {

  qdec_config_t qdec_conf;
  uint16_t qdec_pos, qdec_frq;
  bool qdec_dir;

  sysclk_init();

  board_init();

  irq_initialize_vectors();

  cpu_irq_enable();

  stdio_usb_init();

  sleepmgr_init();

  // Enable the back light of the LCD
  ioport_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

  // Light up LED 1, wait for button press.
  // ioport_set_pin_low(LED1_PIN);

  // Light up LED 2, wait for button press.
  ioport_set_pin_low(LED2_PIN);

  // Setup QDEC
  qdec_get_config_defaults(&qdec_conf);
  qdec_config_disable_index_pin(&qdec_conf);
  qdec_config_phase_pins(&qdec_conf, QDEC_PORT, QDEC_PIN_BASE, false, 20 );
  qdec_config_tc(&qdec_conf, QDEC_TC);
  qdec_config_enable_freq(&qdec_conf, 100);
  qdec_config_freq_tc(&qdec_conf, QDEC_FREQ_TC);
  qdec_config_revolution(&qdec_conf, QUADRATURE_RESOLUTION);

  qdec_enabled(&qdec_conf);

  // Configure TWI
  sysclk_enable_peripheral_clock(TWI_DEVICE);

  TWI_SlaveInitializeDriver(&slave, TWI_DEVICE, &slave_process);
  TWI_SlaveInitializeModule(&slave, TWI_CHIP,
                            TWI_SLAVE_INTLVL_MED_gc);

  gfx_mono_init();

  char out_str[OUTPUT_STR_SIZE];
  snprintf(out_str, OUTPUT_STR_SIZE, "Git: %s\nDate: %s", GIT_COMMIT_HASH, BUILD_DATE);
  gfx_mono_draw_string(out_str, 0, 0, &sysfont);

  gfx_mono_generic_draw_filled_rect(0,0,128,32,GFX_PIXEL_XOR);

  delay_ms(250);


  gfx_mono_generic_draw_filled_rect(0,0,128,32,GFX_PIXEL_CLR);


  int i = 0;
  for(;;){

    i++;
    delay_ms(10);

    if(!(i%3))
      ioport_toggle_pin(LED1_PIN);

    if(!(i%5))
      ioport_toggle_pin(LED2_PIN);

    if((i%10))
      continue;

    qdec_pos = qdec_get_position(&qdec_conf);
    qdec_dir = qdec_get_direction(&qdec_conf);
    qdec_frq = qdec_get_frequency(&qdec_conf);

    memset(out_str, 0x00, OUTPUT_STR_SIZE);

    snprintf(out_str, OUTPUT_STR_SIZE, "%c%04u\n%u.%uHz\n%s",
             qdec_dir ? '+' : '-',
             qdec_pos,
             qdec_frq/10,
             qdec_frq%10,
             recv_data );

    //gfx_mono_generic_draw_filled_rect(0,0,128,32,GFX_PIXEL_CLR);
    gfx_mono_draw_string(out_str, 0, 0, &sysfont);

    printf("Tick: %d\r\n", i);

  }

  return 0;
}