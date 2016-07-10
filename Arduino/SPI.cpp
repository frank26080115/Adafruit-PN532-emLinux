/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * Copyright (c) 2016 by Frank Zhao
 * 
 * Dummy shell, TODO: Implement SPI functions on embedded Linux
 *
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

SPIClass SPI;

void SPIClass::begin()
{
    errortf("NOT IMPLEMENTED\r\n");
}

void SPIClass::end()
{
    errortf("NOT IMPLEMENTED\r\n");
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
    errortf("NOT IMPLEMENTED\r\n");
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{
    errortf("NOT IMPLEMENTED\r\n");
}