#include <BMP280.h>
#include <SPI.h>
#include <STBLE.h>
#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

BMP280 bmp;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

/*
//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif
*/

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

const int chipSelect =10;

double alt = 25.0; // Altitude at JMA location in meters
double T;
int reading_timestamp;
double P0;

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {

   SerialMonitorInterface.begin(9600);
   while (!SerialMonitorInterface);
   BLEsetup();
   LedSign::Init();
 
  if (!bmp.begin()) {
    SerialMonitorInterface.println("BMP init failed!");
    while (1);
  }
  else SerialMonitorInterface.println("BMP init success!");
  bmp.setOversampling(4);
  }

void loop() {
  aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  if (ble_rx_buffer_len) {//Check if data is available
  SerialMonitorInterface.print(ble_rx_buffer_len);
  SerialMonitorInterface.print(" : ");
  SerialMonitorInterface.println((char*)ble_rx_buffer);
  ble_rx_buffer_len = 0;//clear afer reading
  }
  if (SerialMonitorInterface.available()) {//Check if serial input is available to send
    delay(10);//should catch input
    uint8_t sendBuffer[21];
    uint8_t sendLength = 0;
    while (SerialMonitorInterface.available() && sendLength < 19) {
      sendBuffer[sendLength] = SerialMonitorInterface.read();
      sendLength++;
    }
    if (SerialMonitorInterface.available()) {
      SerialMonitorInterface.print(F("Input truncated, dropped: "));
      if (SerialMonitorInterface.available()) {
        SerialMonitorInterface.write(SerialMonitorInterface.read());
      }
    }
    sendBuffer[sendLength] = '\0'; //Terminate string
    sendLength++;
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength))
    {
      SerialMonitorInterface.println(F("TX dropped!"));
    }
  }
  
  
  //receiveScrapeData();
  //SerialMonitorInterface.println(receivedChars);
}

void receiveScrapeData(){
 
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;


    if (ble_rx_buffer_len) {//Check if data is available
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    SerialMonitorInterface.println((char*)ble_rx_buffer);
    ble_rx_buffer_len = 0;//clear afer reading
  }
 
    while (SerialMonitorInterface.available() > 0 && newData == false) {
     
        rc = SerialMonitorInterface.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

/*
void calculateNewP0() {
    if (newData == true) {
        
        newData = false;
    }
}
*/
