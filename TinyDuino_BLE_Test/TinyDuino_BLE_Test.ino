#include <SPI.h>
#include <STBLE.h>


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

String reading;


int reading_alt = 25; // Altitude at JMA location in meters
double reading_temp;
int reading_time;
double reading_pressure;
double P0;

void setup() {
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+!
  BLEsetup();
}


void loop() {

  readFromBLE();
}


void readFromBLE(){

    aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  if (ble_rx_buffer_len) {//Check if data is available
    
    reading = (char*)ble_rx_buffer;
    int ind1 = reading.indexOf(',');
    reading_time = (reading.substring(0,ind1)).toInt();
    int ind2 = reading.indexOf(',', ind1+1);
    reading_temp = (reading.substring(ind1+1,ind2+1)).toDouble();
    reading_pressure = (reading.substring(ind2+1)).toDouble();
    SerialMonitorInterface.println(reading_time);
    SerialMonitorInterface.println(reading_temp);
    SerialMonitorInterface.println(reading_pressure);
     
    ble_rx_buffer_len = 0;//clear afer reading
  }
}
