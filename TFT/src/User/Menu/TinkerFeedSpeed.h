//
// adjust runtime feed rate and motion speed
//

#pragma once

#include "stdint.h"
#include "stdbool.h"

void menuFeedSpeed(void);

void speedSetSendWaiting(uint8_t tool, bool isWaiting);
void speedSetPercent(uint8_t tool, uint16_t per);
uint16_t speedGetPercent(uint8_t tool);
