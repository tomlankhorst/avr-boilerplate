/**
 * avr-boilerplate ee.h
 *
 * @author Tom Lankhorst
 * @date 15/11/2017
 */

#include "conf_application.h"

#include <util/delay.h>

int main( ) {

  int a = 0;

  for(;;){
    _delay_ms(10);
    a++;
  }

  return 0;
}