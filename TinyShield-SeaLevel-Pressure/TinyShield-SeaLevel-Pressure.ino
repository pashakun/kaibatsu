#include "BMP280.h"
#include "Wire.h"
#include <SPI.h>
#include <SD.h>

BMP280 bmp;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

double alt = 5.1; // Altitude of current location in meters
const int chipSelect =10;
double P0;
String dataString = "";
double temp = 30.7;
double atm = 1009.8;


void setup() {

  SerialMonitorInterface.begin(9600);
  // SerialMonitorInterface.println("Initializing SD card...");

  // see if the card is present and can be initialized:
/*
  if (!SD.begin(chipSelect)) {
    SerialMonitorInterface.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  SerialMonitorInterface.println("card initialized.");
  
  */
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
  // P0 = bmp.sealevel(P, alt);
  T = 0.7;
  P0 = bmp.sealevel(atm, alt);
  SerialMonitorInterface.println(P0);

  delay(1000);
/*
  File myFile;
  myFile = SD.open("p0.txt", FILE_WRITE);
  if (myFile) {
    SerialMonitorInterface.print("Writing PO to file: ");
    SerialMonitorInterface.println(P0);
    myFile.print("Sea level pressure: ");
    myFile.println(P0);
    myFile.println("Altitude readings: ");
    myFile.close();
  //  SerialMonitorInterface.println("Done");
  } else {
  SerialMonitorInterface.println("error opening p0.txt");
  }
  */
  }
  
void writeAltitude(String A) {
  File myFile;
  myFile = SD.open("P0.txt", FILE_WRITE);

/*
  if (myFile) {
    //SerialMonitorInterface.print("A = ");
    //SerialMonitorInterface.println(A);
    myFile.println(A);
    myFile.close();
  } else {
  SerialMonitorInterface.println("error opening alt.txt");
  }
  */
}


void loop() {

  double T, P;
  char result = bmp.startMeasurment();

  if (result != 0) {
     delay(result);
     result = bmp.getTemperatureAndPressure(T, P);     
          }
         
  double A = bmp.altitude(P, P0);

  writeAltitude(String(A));
  delay(1000);
}
