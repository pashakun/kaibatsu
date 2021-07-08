#include <SimpleTimer.h>
#include <Tiny7Seg.h>
#include <STBLE.h>
#include <BMP280.h>
#include <SPI.h>
#include <Wire.h>


BMP280 bmp;
Tiny7Seg seg;
SimpleTimer timer;

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

int BLE_reading_altitude; 
double P0 = 0.0;

bool newData = true;


void setup() {
  
  Wire.begin();
  seg.init();
  setUpBlink01();
  
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();

  if (!bmp.begin()) {
    SerialMonitorInterface.println("BMP init failed!");
    while (1);
  }
  else SerialMonitorInterface.println("BMP init success!");
  bmp.setOversampling(4);

  setUpBlink02();
  timer.setInterval(3600000);
  
}


void loop(){

    if (newData){
      while(!BLE_reading_altitude){
      readFromBLE();       
      }
      caculateP0(BLE_reading_altitude);
    }
    
    calculateAlt(P0);
}


// take a bluetooth reading
void readFromBLE(){

    aci_loop();
  if (ble_rx_buffer_len) { //Check if data is available
    
    BLE_reading = (char*)ble_rx_buffer;
    int ind1 = BLE_reading.indexOf(',');
    BLE_reading_altitude = (BLE_reading.substring(0,ind1)).toInt();
    SerialMonitorInterface.print("Reading_altitude = ");
    SerialMonitorInterface.println(BLE_reading_altitude);
     
    ble_rx_buffer_len = 0;//clear afer reading

    newData = false;
  }
}


// calculate sea level pressure, done once
void caculateP0(int BLE_reading_altitude){

     double T, P;
     char result = bmp.startMeasurment();
     SerialMonitorInterface.println("Getting first BMP reading.");

     if (result != 0) {
     delay(result);
     result = bmp.getTemperatureAndPressure(T, P);  
     SerialMonitorInterface.println("Got T and P.");   
          }

     P0 = bmp.sealevel(P, BLE_reading_altitude);
     SerialMonitorInterface.print("P0 = ");
     SerialMonitorInterface.println(P0);
    
    }


// update altitude, this part loops
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


void setUpBlink01(){
  // rapidly blinks decimal points in turn, shuts off
  seg.setDecPoints(1,0);
  delay(300);
  seg.setDecPoints(0,1);
  delay(300);
  seg.setDecPoints(1,0);
  delay(300);
  seg.setDecPoints(0,1);
  delay(300);
  seg.off();
}

void setUpBlink02(){
  // blinks both decimal points together, shuts off
  seg.setDecPoints(1,1);
  delay(300);
  seg.off();
  delay(300);
  seg.setDecPoints(1,1);
  delay(300);
  seg.off();
  delay(300);
  seg.setDecPoints(1,1);
  delay(1000);
  seg.off();
}

void blink00(){
  // blinks "00" three times, shuts off
  for(int i=0; i<3; i++){
    seg.printInt(00);
    delay(1000);
    seg.off();
    delay(500);
  };
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

void displayAltitude(int A){

  if (A < 100){
    seg.printInt(A);
    delay(3000);
    seg.off();
    delay(1000);
  } else {
    blink99();
  }
    
  
}
