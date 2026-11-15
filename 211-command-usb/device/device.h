#pragma once

#include <stdint.h>
#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "1.0.0"

#define DEVICE_PROJECT "211-command-usb"
#define DEVICE_REPO "https://github.com/Nordeland/es-student"
#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

struct info_t
{
    uint8_t revision;
    char name[13];
    uint32_t version;
};

extern struct info_t device_card;
void dev_info(void);

void device_info(void);