/*
  TinyCircuits 7 Segment Display TinyShield Example Sketch

  This sketch demonstrates the functionality of the display by
  counting down from 5, flashing "00" three times, then toggles
  the decimal segments indefinitely.

  Written 
  By 
  Modified 07 January 2019
  By Hunter Hykes

  https://TinyCircuits.com
*/

#include <Wire.h>
#include "Tiny7Seg.h"

Tiny7Seg seg;

void setup()
{
  Wire.begin();
  seg.init();
  }

void loop()
{

 blink99();
 delay(3000);
}

void blink99(){
  // blinks "99" three times, shuts off
  for(int i=0; i<3; i++){
    seg.printInt(99);
    delay(1000);
    seg.off();
    delay(500);
  };
}
