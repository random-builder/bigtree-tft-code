#include <HeaterPreset.h>
#include "includes.h"


MENUITEMS preheatItems = {
// title
LABEL_PREHEAT,
// icon                       label
  {
    {ICON_Preheat_1,          LABEL_PREHEAT_PLA},
    {ICON_Preheat_2,          LABEL_PREHEAT_PET},
    {ICON_Preheat_3,          LABEL_PREHEAT_ABS},
    {ICON_Nozzle,             LABEL_NOZZLE},
    {ICON_Preheat_4,          LABEL_PREHEAT_NYL},
    {ICON_Preheat_5,          LABEL_PREHEAT_PEK},
    {ICON_Stop,               LABEL_ISSUE_RESET},
    {ICON_Back,               LABEL_BACK},
  }
};

const ITEM itemToolPreheat[] = {
// icon                       label
  {ICON_Preheat_Both,         LABEL_PREHEAT_BOTH},
  {ICON_Hotbed,                  LABEL_BED},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
};   

const u16   preheat_bed_temp[] = PREHEAT_BED;
const u16   preheat_hotend_temp[] = PREHEAT_HOTEND;

static TOOL_PREHEAT nowHeater = PREHEAT_NOZZLE0;

void menuHeaterPreset(void)
{
  KEY_VALUES  key_num = KEY_IDLE;

  menuDrawPage(&preheatItems);

  while(infoMenu.menu[infoMenu.cur] == menuHeaterPreset)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
      case KEY_ICON_1:
      case KEY_ICON_2:
      case KEY_ICON_4:
      case KEY_ICON_5:
      case KEY_ICON_6:
        switch(nowHeater){
          case PREHEAT_BOTH:
            heatSetTargetTemp(TOOL_HOTBED, preheat_bed_temp[key_num]);
            heatSetTargetTemp(heatGetCurrentToolNozzle(), preheat_hotend_temp[key_num]);
            break;
          case PREHEAT_HOTBED:
            heatSetTargetTemp(TOOL_HOTBED, preheat_bed_temp[key_num]);
            break;
          case PREHEAT_NOZZLE0:
            heatSetTargetTemp(heatGetCurrentToolNozzle(), preheat_hotend_temp[key_num]);
            break;
        }
        break;
        
      case KEY_ICON_3: // Nozzle, Hotbed, or Both
        nowHeater = (TOOL_PREHEAT)((nowHeater+1) % 3);
        preheatItems.items[key_num] = itemToolPreheat[nowHeater];
        menuDrawItem(&preheatItems.items[key_num], key_num);;
        break;
      
      case KEY_ICON_7:
        infoMenu.cur--; break;
      default:break;
    }
    loopProcess();
  }
}
