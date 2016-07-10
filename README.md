This is a library for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 
These chips use I2C or SPI to communicate.

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada & Kevin Townsend for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

This library has been edited to work on embedded Linux platforms such as the NextThingCo CHIP computer.
Minimal changes has been made to the original Adafruit_PN532 library, but there is a layer of code implemented to translate Arduino function calls into the style of function calls required to work on embedded Linux.