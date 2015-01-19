ReceiverFineOffset library for Arduino
======================================

An Arduino library for receiving WH2 Weather Sensor temperatur and humidity from Fine Offset Electronics.

Based on Luc Small's libary,
 https://github.com/lucsmall/WH2-Weather-Sensor-Library-for-Arduino <br />
And also on Randy Simons's Fuzzillogic libary, 
 https://bitbucket.org/fuzzillogic/433mhzforarduino/wiki/Home

**Examples of sensor:**
![alt tag](https://raw.githubusercontent.com/rosangen/ReceiverFineOffset/master/docs/image.jpg)

###InterruptChain
This library allows to have more than one callback for an interrupt signal.
Its purpose here is to allow listening for signals of both type remote switches and weather stations using only one receiver at the same time. You can combine all the functionality of all libraries in one sketch! 
However, its use is optional; the libraries can be used stand-alone as well.

For more info regarding interruptChain visit *fuzzillogic* at https://bitbucket.org/fuzzillogic/433mhzforarduino/wiki/Home

###Hardware setup
Connect a 433MHz receiver to digital pin 2 on the Arduino.

###Known problems
This library have quite bad receiving function, maximum 5-10 meters then i tested it.
https://github.com/lucsmall/BetterWH2 works better, but with no InterruptChange support.

---
This code contains a CRC-8 function adapted from the Arduino OneWire library:
 http://www.pjrc.com/teensy/td_libs_OneWire.html <br />
Thanks go to the authors of that project.

---
Released into the public domain.