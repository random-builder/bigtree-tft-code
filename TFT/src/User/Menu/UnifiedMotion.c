#include <UnifiedMotion.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon + label)
const MENUITEMS UnifiedMoveItems = {
// title
LABEL_UNI_MOVE,
// icon                       label
 {
  {ICON_Home,                 LABEL_HOME},       // 0
  {ICON_Move,                 LABEL_MOVE},
  {ICON_Leveling,             LABEL_AUTO_LEVEL},
  {ICON_ManualLevel,          LABEL_HAND_LEVEL},
  {ICON_Stop,                 LABEL_MOTION_STOP}, // 4
  {ICON_EMPTY,           LABEL_BACKGROUND},
  {ICON_EMPTY,           LABEL_BACKGROUND},
  {ICON_Back,                 LABEL_BACK},}
};

void menuUniMotion(void)
{
  KEY_VALUES key_num = KEY_IDLE;	
  menuDrawPage(&UnifiedMoveItems);
  while(infoMenu.menu[infoMenu.cur] == menuUniMotion)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: infoMenu.menu[++infoMenu.cur] = menuHoming; break;
      case KEY_ICON_1: infoMenu.menu[++infoMenu.cur] = menuPosition; break;
      case KEY_ICON_2: infoMenu.menu[++infoMenu.cur] = menuAutoLeveling; break;
      case KEY_ICON_3: infoMenu.menu[++infoMenu.cur] = menuManualLeveling; break;

      case KEY_ICON_4: storeCmd("M84\n");   break; // Motion Stop

      case KEY_ICON_7: infoMenu.cur--; break;
      default: break;            
    }
    loopProcess();
  }
}
