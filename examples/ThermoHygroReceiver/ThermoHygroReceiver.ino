/*
 * ReceiverFineOffset library for Arduino
 * 
 * This library receives, decodes, decrypts and receives data of
 * remote weather sensors from Fine Offset Electronics.
 * WH2 outdoor weather sensor and also known by name Viking 02035
 * and under other names.
 * 
 * Based on https://bitbucket.org/fuzzillogic/433mhzforarduino/wiki/Home
 * and on https://github.com/lucsmall/WH2-Weather-Sensor-Library-for-Arduino
 *
 * Parts (mostly) of this code based on Luc Small's library
 * I thank him very much!
 *
 * License: GPLv3. See license.txt
 */

#include <ReceiverFineOffset.h>
volatile int rSensorId,rTemperature,rHumidity;

void setup() {
  Serial.begin(115200);

  // Init the receiver on interrupt pin 0 (digital pin 2).
  // Set the callback to function "getTempHumi", which is called
  // whenever valid sensor data has been received.
  ReceiverFineOffset::init(0, getTempHumi);
}

void loop() {
  // Checking for data received. However, you can do other stuff here if you like.
  if ( rSensorId ) {
    Serial.print("Id: ");
    Serial.print(rSensorId,HEX);
    Serial.print(" Temperature: ");
    Serial.print(rTemperature);
    Serial.print(" Humidity: ");
    Serial.println(rHumidity);
    
    rSensorId = 0;
  }
}

void getTempHumi(byte *data) {

  // 0x48/72 0x30/48 0xBE/190 0xFF/255 0x71/113 | Sensor ID: 0x483 | 255% | 19.0000000000 | OK

  if (data[0]) {
    int sensorId = (data[0] << 4) + (data[1] >> 4);
    
    /* Temperature in deci-degrees. e.g. 251 = 25.1 */
    int temperature;
    temperature = ((data[1] & B00000111) << 8) + data[2];
    if (data[1] & B00001000) { // make negative
      temperature = -temperature;
    }

    int humidity = data[3];

    rSensorId = sensorId;
    rTemperature = temperature;
    rHumidity = humidity;
  }
}
