#ifndef _PREHEATMENU_H_
#define _PREHEATMENU_H_

#include "Configuration.h"

typedef enum
{
  PREHEAT_BOTH = 0,
  PREHEAT_HOTBED = 1,
  PREHEAT_NOZZLE0 = 2,
}TOOL_PREHEAT;


void menuHeaterPreset(void);

#endif

