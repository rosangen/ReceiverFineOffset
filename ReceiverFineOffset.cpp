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

unsigned long ReceiverFineOffset::lastChange=0;
SensorReceiverCallback ReceiverFineOffset::callback;
byte ReceiverFineOffset::data[5];
word ReceiverFineOffset::duration;
boolean ReceiverFineOffset::enabled;

void ReceiverFineOffset::init(int8_t interrupt, SensorReceiverCallback callbackIn) {
  callback = callbackIn;
  
  enable();
  
  if (interrupt >= 0) {
  attachInterrupt(interrupt, interruptHandler, CHANGE);
  }  
}

void ReceiverFineOffset::interruptHandler() {  
  if (!enabled) {
    return;
  }
  unsigned long currentTime=micros();
  duration=currentTime-lastChange; // Duration = Time between edges
  lastChange = currentTime;
  
  byte sample;
  volatile static byte state = 0;
  volatile static byte packet_no, bit_no, history;
  
  volatile word pTime[50];
  static byte bitTotal;

  // Dead time is ~1000us
  if (duration > 700 && duration < 1250) {
    return;
  
  // 1 is indicated by 500uS pulse
  } else if (duration > 300 && duration < 700) {
    sample = 1;
  
  // 0 is indicated by ~1500us pulse
  } else if (duration > 1250  && duration < 1750) {
    sample = 0;

  } else {
    state = 0;
    return;
  } 
  
  // reset if in initial state
  if (state == 0) {
     // should history be 0, does it matter?
    history = 0x00; // 0xFF; 
    state = 1;
  } // fall thru to state one

  // acquire preamble
  if (state == 1) {
     // shift history right and store new value
     history <<= 1;
     // store a 1 if required (right shift along will store a 0)
     if (sample == 1) {
       history |= 0x01;
     }
     // check if we have a valid start of frame
     // xxxxx110
     if ((history & B00000111) == B00000110) {
       // need to clear packet, and counters
       packet_no = 0;
       // start at 1 because only need to acquire 7 bits for first packet byte.
       bit_no = 1;
       data[0] = data[1] = data[2] = data[3] = data[4] = 0;
       // we've acquired the preamble
       state = 2;
	   bitTotal = 0;
	   //Serial.print(".");
     }
    return;
  }
  // acquire packet
  if (state == 2) {
    data[packet_no] <<= 1;
    if (sample == 1) {
      data[packet_no] |= 0x01;
    }

    bit_no ++;
    if (bit_no > 7) {
      // Serial.println(bit_no , BIN);
      bit_no = 0;
      packet_no ++; 
    }
    
    if (packet_no > 4) {
      // start the sampling process from scratch
      state = 0;
	    if (1==0){
		for (int i=0; i<9; i++) {
          Serial.println(pTime[i]);
	    }
		}
	  
      if ( _crc8(data, 4) == data[4] ) (callback)(data);

    }
	
	if (1==0){
	pTime[bitTotal] = duration;
	bitTotal++;
	if ( bitTotal > 49 ) bitTotal = 0;
	}
  }

  return;
}

uint8_t ReceiverFineOffset::_crc8( uint8_t *addr, uint8_t len)
{
  uint8_t crc = 0;

  // Indicated changes are from reference CRC-8 function in OneWire library
  while (len--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x80; // changed from & 0x01
      crc <<= 1; // changed from right shift
      if (mix) crc ^= 0x31;// changed from 0x8C;
      inbyte <<= 1; // changed from right shift
    }
  }
  return crc;
}

void ReceiverFineOffset::enable() {
  enabled = true;
}

void ReceiverFineOffset::disable() {
  enabled = false;
}
