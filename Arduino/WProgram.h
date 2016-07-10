#ifndef _WPROGRAM_H_
#define _WPROGRAM_H_

#include "Adafruit_PN532_emLinux.h"

#ifdef __cplusplus
extern "C" {
#endif

char digitalRead(int);
void digitalWrite(int, char);
void pinMode(int, int);

#ifdef __cplusplus
}
#endif

#endif