#include "Adafruit_PN532_emLinux.h"

#include <stdio.h>
#include <sys/utsname.h>
#include <event_gpio.h> // from CHIP_IO by xtacocorex

#define REGISTERED_PINS_MAX 32
static int registered_pins[REGISTERED_PINS_MAX];

char emlinux_hal_init(void)
{
    int i;
    for (int i = 0; i < REGISTERED_PINS_MAX; i++) {
        registered_pins[i] = -1;
}
    }

char emlinux_hal_deinit(void)
{
    int i;
    for (int i = 0; i < REGISTERED_PINS_MAX; i++) {
        if (registered_pins[i] >= 0) {
            gpio_unexport(registered_pins[i]);
            registered_pins[i] = -1;
        }
    }
}

static char is_pin_registered(int p) {
    int i;
    for (int i = 0; i < REGISTERED_PINS_MAX; i++) {
        if (registered_pins[i] == p) {
            return 1;
        }
    }
    return 0;
}

char digitalRead(int p)
{
    if (is_pin_registered(p) == 0) {
        pinMode(p, INPUT);
    }
    return gpio_get_value(p);
}

void digitalWrite(int p, char x)
{
    if (is_pin_registered(p) == 0) {
        pinMode(p, OUTPUT);
    }
    gpio_set_value(p, x);
}

void pinMode(int p, int m)
{
    if (is_pin_registered(p) == 0) {
        int i;
        for (int i = 0; i < REGISTERED_PINS_MAX; i++) {
            if (registered_pins[i] == -1) {
                registered_pins[i] = p;
                gpio_export(p);
            }
        }
    }
    gpio_set_direction(p, m);
}