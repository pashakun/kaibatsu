#include <SD.h>
#include <SPI.h>

#define P0 1013.25
BMP280 bmp;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif


const int chipSelect =10;

void setup()
{
  SerialMonitorInterface.begin(9600);
  SerialMonitorInterface.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    SerialMonitorInterface.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  SerialMonitorInterface.println("card initialized.");
  
  if (!bmp.begin()) {
    SerialMonitorInterface.println("BMP init failed!");
    while (1);
  }
  else SerialMonitorInterface.println("BMP init success!");

  bmp.setOversampling(4);

}
