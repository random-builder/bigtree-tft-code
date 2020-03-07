//
//
//

#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "Configuration.h"

typedef enum {
    TOOL_HOTBED = 0,
    TOOL_NOZZLE0 = 1,
    TOOL_NOZZLE1 = 2,
    TOOL_NOZZLE2 = 3,
    TOOL_NOZZLE3 = 4,
    TOOL_NOZZLE4 = 5,
    TOOL_NOZZLE5 = 6,
//    HEATER_NUM = TOOL_NUM + TOOL_NOZZLE0,
} TOOL;

// tool temperature state
typedef struct {
    int16_t current;  // current temperature
    int16_t target;   // target temperature
    bool waiting;     // tool is waiting for current to reach target
} _TEMP_;

// heater tool selection and temperature state
typedef struct {
    _TEMP_ T[HEATER_COUNT];  // track temperature state per tool
    TOOL tool;    // index of selected tool (hotbed or nozzle)
    TOOL nozzle;  // index of active nozzle (nozzle only)
} HEATER;

// FIXME remove this

extern uint16_t heat_max_temp[];
extern char *toolID[];
extern char *heatDisplayID[];
extern char *heatCmd[];
extern char *heatWaitCmd[];

void menuHeaterControl(void);
void updateLastHeatCheckTime(void);
void heatSetTargetTemp(TOOL tool, uint16_t temp);
void heatSyncTargetTemp(TOOL tool, uint16_t temp);
uint16_t heatGetTargetTemp(TOOL tool);
void heatSetCurrentTemp(TOOL tool, int16_t temp);
int16_t heatGetCurrentTemp(TOOL tool);
bool heatGetIsWaiting(TOOL tool);
bool heatHasWaiting(void);
void heatSetIsWaiting(TOOL tool, bool isWaiting);
void heatClearIsWaiting(void);
void heatSetCurrentTool(TOOL tool);
TOOL heatGetCurrentTool(void);
void heatSetCurrentToolNozzle(TOOL tool);
TOOL heatGetCurrentToolNozzle(void);
void heatSetUpdateTime(uint32_t time);
void heatSetUpdateWaiting(bool isWaiting);
void heatSetSendWaiting(TOOL tool, bool isWaiting);
void loopCheckHeater(void);

void heater_ensure_config(void);
bool heater_has_use(TOOL tool);
TOOL heater_next_tool(TOOL this_tool);
TOOL heater_next_nozzle(TOOL this_tool);
