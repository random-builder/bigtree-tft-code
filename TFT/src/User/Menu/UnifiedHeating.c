#include <UnifiedHeating.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon + label)
const MENUITEMS UnifiedHeatItems = {
// title
LABEL_HEATING,
// icon                       label
 {{ICON_Heat_1,               LABEL_PREHEAT},
  {ICON_Heat_2,               LABEL_HEAT_CTL},
  {ICON_Fan,                  LABEL_FAN},
  {ICON_CoolDown,             LABEL_COOLDOWN},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Back,                 LABEL_BACK},}
};

void menuUniHeating(void)
{
  KEY_VALUES key_num = KEY_IDLE;	
  menuDrawPage(&UnifiedHeatItems);
  while(infoMenu.menu[infoMenu.cur] == menuUniHeating)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: infoMenu.menu[++infoMenu.cur] = menuHeaterPreset;   break;
      case KEY_ICON_1: infoMenu.menu[++infoMenu.cur] = menuHeaterControl;    break;
      case KEY_ICON_2: infoMenu.menu[++infoMenu.cur] = menuCoolerControl;    break;
      case KEY_ICON_3:
        for(TOOL i = TOOL_HOTBED; i < HEATER_NUM; i++)
        {
          heatSetTargetTemp(i, 0);
        }
        break;
        
      case KEY_ICON_7: infoMenu.cur--;      break;
      default:break;            
    }
    loopProcess();
  }
}
