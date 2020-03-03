//
// main menu
//

#include <ArkonMenu.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon+label) 
const MENUITEMS mainPageItems = {
// title
LABEL_READY,
// icon                       label
   {
    {ICON_Home_Move,            LABEL_UNI_MOVE}, // 0
    {ICON_Heat_Fan,             LABEL_UNI_HEAT}, // 1
    {ICON_Extrude,              LABEL_FILAMENT}, // 2
    {ICON_Settings,             LABEL_SETTINGS}, // 3
    {ICON_EMPTY,           LABEL_BACKGROUND},
    {ICON_Gcode,                LABEL_GCODE},    // 5
    {ICON_Custom,               LABEL_CUSTOM},   // 6
    {ICON_Back,                 LABEL_BACK}      // 7
  }
};

void menuMain(void)
{
  KEY_VALUES key_num=KEY_IDLE;
  GUI_SetBkColor(ST7920_BKCOLOR);
  menuDrawPage(&mainPageItems);

  while(infoMenu.menu[infoMenu.cur] == menuMain)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
        case KEY_ICON_0: infoMenu.menu[++infoMenu.cur] = menuUniMotion;     break;
        case KEY_ICON_1: infoMenu.menu[++infoMenu.cur] = menuUniHeating;     break;
        case KEY_ICON_2: infoMenu.menu[++infoMenu.cur] = menuFilament;        break;
        case KEY_ICON_3: infoMenu.menu[++infoMenu.cur] = menuSettings;        break;
        // case KEY_ICON_4:
        case KEY_ICON_5: infoMenu.menu[++infoMenu.cur] = menuInvokeGcode;       break;
        case KEY_ICON_6: infoMenu.menu[++infoMenu.cur] = menuCustom ;         break;
        case KEY_ICON_7: infoMenu.cur--;        break;
        default:break;
    }
    loopProcess();
  }
}
