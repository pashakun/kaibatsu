#include <BMP280.h>
#include <SPI.h>
#include <STBLE.h>
#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

BMP280 bmp;

//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

String BLE_reading;

const int chipSelect =10;

int BLE_reading_alt = 25; // Altitude at JMA location in meters
//double reading_temp;
int BLE_reading_time;
double BLE_reading_pressure;
double P0 = 0.0;

bool newData = true;


void setup() {
  
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();

  if (!bmp.begin()) {
    SerialMonitorInterface.println("BMP init failed!");
    while (1);
  }
  else SerialMonitorInterface.println("BMP init success!");
  bmp.setOversampling(4);

  
}

void loop(){

    if (newData){
      while(!BLE_reading_time){
      readFromBLE();       
      }
      caculateP0(BLE_reading_alt, BLE_reading_pressure);
    }
    
    calculateAlt(P0);
//    displayAltitude(A);
}

void readFromBLE(){

    aci_loop();
  if (ble_rx_buffer_len) {//Check if data is available
    
    BLE_reading = (char*)ble_rx_buffer;
    int ind1 = BLE_reading.indexOf(',');
    BLE_reading_time = (BLE_reading.substring(0,ind1)).toInt();
    //int ind2 = reading.indexOf(',', ind1+1);
    //reading_temp = (reading.substring(ind1+1,ind2+1)).toDouble();
    BLE_reading_pressure = (BLE_reading.substring(ind1+1)).toDouble();
    SerialMonitorInterface.print("Reading_time = ");
    SerialMonitorInterface.println(BLE_reading_time);
    //SerialMonitorInterface.println(reading_temp);
    SerialMonitorInterface.print("Reading_pressure = ");
    SerialMonitorInterface.println(BLE_reading_pressure);
     
    ble_rx_buffer_len = 0;//clear afer reading

    newData = false;
  }
}


void caculateP0(int BLE_reading_alt, double BLE_reading_pressure){

    P0 = bmp.sealevel(BLE_reading_pressure, BLE_reading_alt);
    SerialMonitorInterface.print("P0 = ");
    SerialMonitorInterface.println(P0);
    
    }


void calculateAlt(double P0){

  double T, P;
  char result = bmp.startMeasurment();
  SerialMonitorInterface.println("Getting a BMP reading.");

  if (result != 0) {
     delay(result);
     result = bmp.getTemperatureAndPressure(T, P);  
     SerialMonitorInterface.println("Got T and P.");   
          }
         
  SerialMonitorInterface.println("Getting altitude.");
  double altitude_calc = bmp.altitude(P, P0);
  int A = round(altitude_calc);
  SerialMonitorInterface.print("Altitude = ");
  SerialMonitorInterface.println(A);

  displayAltitude(A);
}

void displayAltitude(int A){

  SerialMonitorInterface.println("Printing altitude.");
  String reading = String(A) + " ";
  int reading_len = reading.length();

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
  
}
