#include <UnifiedMotion.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon + label)
const MENUITEMS UnifiedMoveItems = {
// title
LABEL_MOTION,
// icon                       label
 {
  {ICON_Home,                 LABEL_HOMING},       // 0
  {ICON_Move,                 LABEL_WALKING},
  {ICON_Leveling,             LABEL_AUTO_LEVEL},
  {ICON_ManualLevel,          LABEL_HAND_LEVEL},
  {ICON_Stop,                 LABEL_MOTION_STOP}, // 4
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Back,                 LABEL_BACK},}
};

void menuUniMotion(void)
{
  KEY_VALUE key_num = KEY_IDLE;	
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