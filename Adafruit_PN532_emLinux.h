#ifndef _ADAFRUIT_PN532_EMLINUX_H_
#define _ADAFRUIT_PN532_EMLINUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EMLINUX

#include <stdint.h>
#include <stdio.h>
#include "consoleoutput.h"
#include <CHIP_IO.h>

#ifndef byte
#define byte uint8_t
#else
#error stupid arduino and the type byte is causing problems
#endif

#ifndef bool
#define bool uint8_t
#endif

#ifndef true
#define true (1 == 1)
#endif

#ifndef false
#define false (1 == 0)
#endif

void emlinux_hal_init(void);
void emlinux_hal_deinit(void);

#ifdef __cplusplus
}
#endif

#endif