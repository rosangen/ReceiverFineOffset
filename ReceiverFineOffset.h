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

#ifndef ReceiverFineOffset_h
#define ReceiverFineOffset_h

#include <Arduino.h>

typedef void (*SensorReceiverCallback)(byte *); // pointer to data

/**
 * Generic class for receiving and decoding 433MHz remote weather sensors as made by Cresta.
 * E.g. http://www.cresta.nl/index.php?Itemid=2&option=com_zoo&view=item&category_id=32&item_id=281&lang=en
 *
 * Cresta is just a brandname. The original OEM seems to be Hideki Electronics. There are
 * other brands which use the same hardware and / or protocol. As far as I know these include
 * Mebus, Irox, Honeywell, Cresta and RST.
 *
 *
 * This class should be able to receive all sensor types: thermo/hygro, rain, uv, anemo.
 * However, only thermo/hygro is tested and has special support.
 *
 * Hardware required for this library:
 * A 433MHz/434MHz SAW receiver, e.g. http://www.sparkfun.com/products/10532
 */
class ReceiverFineOffset {
	public:
		/**
		 * Initializes the receiver. When a valid data package has been received, the callback is called
		 * with a pointer to the validated and decrypted data. For more details about the data format,
		 * see CrestaProtocol.pdf
		 *
		 * For the thermo/hygro-sensor, you can use decodeThermoHygro() for easy decoding the data.
		 *
		 * If interrupt >= 0, init will register pin <interrupt> to this library.
		 * If interrupt < 0, no interrupt is registered. In that case, you have to call interruptHandler()
		 * yourself whenever the output of the receiver changes, or you can use InterruptChain.
		 *
		 * @param interrupt 	The interrupt as is used by Arduino's attachInterrupt function. See attachInterrupt for details.
								If < 0, you must call interruptHandler() yourself.
		 * @param callbackIn	Pointer to a callback function, with signature void (*func)(byte *, byte). 
		 *						First parameter is the decoded data, the second the length of the package in bytes, including checksums.
		 * 					
		 */
		static void init(int8_t interrupt, SensorReceiverCallback callbackIn);

		/**
		* Enable decoding. No need to call enable() after init().
		*/
		static void enable();

		/**
		* Disable decoding. You can re-enable decoding by calling enable();
		*/
		static void disable();
		
		/**
		 * interruptHandler is called on every change in the input signal. If SensorReceiver::init is called
		 * with interrupt <0, you have to call interruptHandler() yourself. (Or use InterruptChain)
		 */
		static void interruptHandler();

			
	private:
		static uint8_t _crc8( uint8_t *addr, uint8_t len);
		
		static unsigned long lastChange;		// Timestamp of previous edge
		static SensorReceiverCallback callback;	// Pointer to callback function, which is called after valid package has been received
		static word duration;				// Duration of current edge.
		static boolean enabled;				// If true, monitoring and decoding is enabled. If false, interruptHandler will return immediately.
		
		static byte data[5]; 				// Maximum number of bytes used by Fine Offset

};

#endif