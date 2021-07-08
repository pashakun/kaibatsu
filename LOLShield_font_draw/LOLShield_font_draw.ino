#include "Charliplexing.h"
#include "Font.h"


#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif


void setup() {

  LedSign::Init();

  SerialMonitorInterface.begin(9600);
 
  }
  
void loop() {

  char number[] = "1234";
  for (int i = 0; i<5; i++) {
    LedSign::Clear();
    Font::Draw(number[i], -5, 0);
    delay(1000);
  }

}
