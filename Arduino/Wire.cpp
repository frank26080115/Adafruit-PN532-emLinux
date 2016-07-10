/*
  TwoWire - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2016 by Frank Zhao to make it a translation layer to embedded Linux I2C device access
*/

extern "C" {
    #include <Adafruit_PN532_emLinux.h>
    #include <stding.h>
    #include <inttypes.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <linux/i2c-dev.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    //#include "utility/twi.h" // does not exist on emLinux
}

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

char TwoWire::transmitting = 0;
//void (*TwoWire::user_onRequest)(void);
//void (*TwoWire::user_onReceive)(int);

int TwoWire::devNum = -1;
uint8_t TwoWire::targetAddr = -1;
int TwoWire::fd = 0;

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
  begin((int)2); // default for CHIP, arbitary choice for my first project
}

void TwoWire::begin(uint8_t dev)
{
  begin((int)dev);
}

void TwoWire::begin(int dev)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  devNum = dev;

  char path[256];
  sprintf(path, "/dev/i2c-%u", devNum);
  fd = open(path, O_RDWR);
  if (fd < 0) {
      errorf("Failed to open %s , errno %d\r\n", path, errno);
      exit(1);
  }
}

void TwoWire::end(void)
{
  if (fd > 0) {
      close(fd);
  }
}

void TwoWire::setClock(uint32_t clock)
{
    errorf("Unimplemented TwoWire::setClock\r\n");
}

int TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
    int ret;
    if (ioctl(fd, I2C_SLAVE, address >> 1) < 0) {
        errorf("Failed to set I2C slave address, errno %d\r\n", errno);
        return -1;
    }

    // clamp to buffer length
    if(quantity > BUFFER_LENGTH){
        quantity = BUFFER_LENGTH;
    }

    if (isize > 0 && sendStop == 0)
    {
        struct i2c_rdwr_ioctl_data seq;
        struct i2c_msg parts[2];
        uint32_t iaddrBuff = iaddress; // flip if endianness requires it
        seq.nmsgs = 2;
        seq.msgs = &parts;
        seq.msgs[0].addr = address >> 1;
        seq.msgs[0].flags = 0;
        seq.msgs[0].len = isize;
        seq.msgs[0].buf = &iaddrBuff;
        seq.msgs[1].addr = address >> 1;
        seq.msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
        seq.msgs[1].len = quantity;
        seq.msgs[1].buf = rxBuffer;
        if((ret = ioctl(file, I2C_RDWR, &seq)) < 0) {
            errorf("Failed to do I2C_RDWR, errno %d\r\n", errno);
            return -1;
        }
        rxBufferIndex = 0;
        rxBufferLength = ret;
        return ret;
    }
    else
    {
        if (isize > 0)
        {
            uint32_t* bptr = txBuffer;
            *bptr = iaddress;
            // do endian swap here if required
            if ((ret = write(fd, txBuffer, isize)) < 0) {
                errorf("Failed to I2C write, errno %d\r\n", errno);
                return -1;
            }
        }
        if ((ret = read(fd, rxBuffer, quantity)) < 0) {
            errorf("Failed to I2C read, errno %d\r\n", errno);
            return -1;
        }
        rxBufferIndex = 0;
        rxBufferLength = ret;
        return ret;
    }
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  targetAddr = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int ret;

    if (transmitting == 0) {
        return rxBufferLength;
    }

    // set the slave address
    if (ioctl(fd, I2C_SLAVE, targetAddr >> 1) < 0) {
        errorf("Failed to set I2C slave address, errno %d\r\n", errno);
        return -1;
    }

    // do the actual write operation
    if ((ret = write(fd, txBuffer, txBufferLength)) < 0) {
        errorf("Failed to I2C write, errno %d\r\n", errno);
        return -1;
    }

    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
    // indicate that we are done transmitting
    transmitting = 0;
    return ret;

  return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
  return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
      // slave mode not implemented
  }
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  if(transmitting){
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
      // slave mode not implemented
  }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoWire::flush(void)
{
  rxBufferIndex = rxBufferLength = 0;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();
