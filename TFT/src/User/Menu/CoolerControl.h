//
// cooling fan control
//

#pragma once

#include "stdint.h"
#include "stdbool.h"

extern const char *fanID[];
extern const char *fanCmd[];

void menuCoolerControl(void);

void fanSetSpeed(uint8_t i, uint8_t speed);
void fanSetSendWaiting(uint8_t i, bool isWaiting);
uint8_t fanGetCurIndex(uint8_t i);
void fanSetCurIndex(uint8_t i);
uint8_t fanGetSpeed(uint8_t i);
