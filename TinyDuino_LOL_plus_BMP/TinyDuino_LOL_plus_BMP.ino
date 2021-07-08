#include "BMP280.h"
#include "Wire.h"
#include <SPI.h>
#include "Charliplexing.h"
#include "Font.h"

BMP280 bmp;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

double alt = 35.0; // Altitude of current location in meters
const int chipSelect =10;
double P0;

void setup() {

  LedSign::Init();

  SerialMonitorInterface.begin(9600);
 
  if (!bmp.begin()) {
    SerialMonitorInterface.println("BMP init failed!");
    while (1);
  }
  else SerialMonitorInterface.println("BMP init success!");
  bmp.setOversampling(4);

  SerialMonitorInterface.println("Getting a sea level reading.");
  double T, P;
  char result = bmp.startMeasurment();
  delay(1000);

  result = bmp.getTemperatureAndPressure(T, P);
  P0 = bmp.sealevel(P, alt);

  }
  
void loop() {

  double T, P;
  char result = bmp.startMeasurment();
  SerialMonitorInterface.println("Getting an altitude reading.");

  if (result != 0) {
     delay(result);
     result = bmp.getTemperatureAndPressure(T, P);     
          }
         
  double A = bmp.altitude(P, P0);
  
  String reading = String(round(A)) + " ";
  int reading_len = reading.length();
  SerialMonitorInterface.print("reading_len = ");
  SerialMonitorInterface.println(reading_len);

// initialize an array with length equal to number of chars in the reading
// for char arrays, you need an extra space for null character
  char test[reading_len+1];
  reading.toCharArray(test, reading_len+1);

// a hacky way to interrupt the endless scroll
// we want to reading to scroll by just once
// the numbers are arrived at by trial and error
    int trigger = 0;
    int limit;

//  increase the reading_len by 1 to account for the space
    if (reading_len==3){
      limit = 18;
  } else if (reading_len==2) {
      limit = 12;
  } else {
      limit = 24;
  }
    
  for (int8_t x=DISPLAY_COLS, i=0; trigger<limit; x--, trigger++) {
        LedSign::Clear();       
        SerialMonitorInterface.print("trigger = ");
        SerialMonitorInterface.println(trigger);
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
            int8_t w = Font::Draw(test[i2], x2, 0);
            x2 += w, i2 = (i2+1)%strlen(test);
            if (x2 <= 0)  
          x = x2, i = i2;
            }
         
          delay(200);
    }
  SerialMonitorInterface.println("Got to the end");
    delay(3000);
}
