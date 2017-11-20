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

#define QUADRATURE_RESOLUTION (600)

struct dma_channel_config dma_conf = { 0 };
qdec_config_t qdec_conf[3];

typedef volatile struct Kiwi_Sensor {
  uint8_t CONFIG;
  uint8_t STATUS;
  uint16_t ENC_POS[3];
  uint16_t ENC_FRQ[3];
  uint8_t ENC_DIR;
} Kiwi_Sensor_t;

volatile Kiwi_Sensor_t kiwi = { 0x00 };

TWI_Slave_t slave;

static inline void fill_twi_write_buffer(){
  // Trigger a block of size TWI SIZE
  dma_channel_trigger_block_transfer(TWI_DMA_CHANNEL);
  // Wait for completion and reenable
  while(dma_channel_is_busy(TWI_DMA_CHANNEL)){}
  dma_channel_enable(TWI_DMA_CHANNEL);
}

static void slave_process(void) {

  // Reset source ptr
  dma_channel_disable(TWI_DMA_CHANNEL);
  dma_channel_set_source_address( &dma_conf, (uint16_t)(uintptr_t) (((void*)&kiwi) + (slave.receivedData[0])) );
  dma_channel_write_config(TWI_DMA_CHANNEL, &dma_conf);
  dma_channel_enable(TWI_DMA_CHANNEL);

  fill_twi_write_buffer();

}

ISR(TWIC_TWIS_vect) {
  // If the TWI buffer is depleted, refill it and send the data.
  if( ( (slave.bytesSent == 0) || !(slave.interface->SLAVE.STATUS &
      TWI_SLAVE_RXACK_bm) ) && (slave.bytesSent == TWIS_SEND_BUFFER_SIZE) ) {

    // Refill and reset
    fill_twi_write_buffer();
    slave.bytesSent = 0;
  }
  TWI_SlaveInterruptHandler(&slave);
}

void qdec_read_cb(){

  ioport_set_pin_low( LED2_PIN );

  for( int i = 0; i < 3; i++ ) {
    kiwi.ENC_POS[i] = qdec_get_position(&(qdec_conf[i]));
    kiwi.ENC_FRQ[i] = qdec_get_frequency(&(qdec_conf[i]));
    kiwi.ENC_DIR    = ( ( qdec_get_direction(&(qdec_conf[i])) ? 1u : 0 ) << (7-i) );
  }

  ioport_set_pin_high( LED2_PIN );

}

int main( ) {

  pmic_init();

  sysclk_init();

  board_init();

  irq_initialize_vectors();

  // Configure TWI
  sysclk_enable_peripheral_clock(TWI_DEVICE);

  TWI_SlaveInitializeDriver(&slave, TWI_DEVICE, &slave_process);
  TWI_SlaveInitializeModule(&slave, TWI_CHIP, TWI_SLAVE_INTLVL_MED_gc);

  // Configure DMA
  dma_enable();

  dma_channel_set_interrupt_level(&dma_conf, DMA_INT_LVL_OFF ); // no DMA interrupts, busy wait
  dma_channel_set_burst_length(&dma_conf, DMA_CH_BURSTLEN_4BYTE_gc );
  dma_channel_set_transfer_count(&dma_conf, TWIS_SEND_BUFFER_SIZE );
  dma_channel_set_src_reload_mode(&dma_conf, DMA_CH_DESTRELOAD_NONE_gc );
  dma_channel_set_dest_reload_mode(&dma_conf, DMA_CH_DESTRELOAD_BLOCK_gc );
  dma_channel_set_src_dir_mode(&dma_conf, DMA_CH_SRCDIR_INC_gc );
  dma_channel_set_dest_dir_mode(&dma_conf, DMA_CH_DESTDIR_INC_gc );
  dma_channel_set_trigger_source(&dma_conf,  DMA_CH_TRIGSRC_OFF_gc );
  dma_channel_set_source_address(&dma_conf, (uint16_t)(uintptr_t)( &kiwi ));
  dma_channel_set_destination_address(&dma_conf, (uint16_t)(uintptr_t)( slave.sendData ));
  // Writing config and enabling is done in TWI interrupt handler.

  cpu_irq_enable();

  stdio_usb_init();

  sleepmgr_init();

  // Enable the back light of the LCD
  ioport_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);

  // Disable all LEDs
  ioport_set_pin_high(LED1_PIN);
  ioport_set_pin_high(LED2_PIN);

  // Setup QDEC, uses TC PORT D E F 0/1
  for( int i = 0; i < 3; i++ ) {
    struct qdec_config* qcp = &(qdec_conf[i]);

    qdec_get_config_defaults(qcp);
    qdec_config_disable_index_pin(qcp);
    qdec_config_phase_pins(qcp, QDEC0_PORT, QDEC0_PIN_BASE, false, 20 );
    qdec_config_tc(qcp, QDEC0_TC);
    qdec_config_enable_freq(qcp, 10 );
    qdec_config_freq_tc(qcp, QDEC0_FREQ_TC);
    qdec_config_revolution(qcp, QUADRATURE_RESOLUTION);
    qdec_enabled(qcp);
  }

  // Setup TC for measuring kiwi
  // 1024x prescaler = 23.43 kHz, 10 Hz = 2343
  tc_enable(TIMER_QDEC_READ);
  tc_set_overflow_interrupt_callback(TIMER_QDEC_READ, qdec_read_cb);
  tc_set_wgm(TIMER_QDEC_READ, TC_WG_NORMAL);
  tc_write_period(TIMER_QDEC_READ, TIMER_QDEC_READ_PERIOD);
  tc_write_clock_source(TIMER_QDEC_READ, TC2_CLKSEL_DIV1024_gc );
  tc_set_overflow_interrupt_level(TIMER_QDEC_READ, TC_INT_LVL_LO);
  tc_set_resolution(TIMER_QDEC_READ, TIMER_QDEC_READ_PERIOD);

  // Boot, start, etc.
  gfx_mono_init();

  char out_str[OUTPUT_STR_SIZE];
  snprintf(out_str, OUTPUT_STR_SIZE, "Git: %s\nDate: %s", GIT_COMMIT_HASH, BUILD_DATE);
  gfx_mono_draw_string(out_str, 0, 0, &sysfont);

  for(;;)
    sleepmgr_enter_sleep();

  return 0;
}