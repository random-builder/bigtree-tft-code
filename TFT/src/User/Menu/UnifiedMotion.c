#include <UnifiedMotion.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon + label)
const MENUITEMS UnifiedMoveItems = {
// title
LABEL_UNI_MOVE,
// icon                       label
 {
  {ICON_HOME,                 LABEL_HOME},
  {ICON_MOVE,                 LABEL_MOVE},
  {ICON_LEVELING,             LABEL_AUTO_LEVEL},
  {ICON_MANUAL_LEVEL,         LABEL_HAND_LEVEL},
  {ICON_Z_DEC,                LABEL_LOWER_BED}, // 4
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
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
      case KEY_ICON_1: infoMenu.menu[++infoMenu.cur] = menuMove; break;
      case KEY_ICON_2: infoMenu.menu[++infoMenu.cur] = menuAutoLeveling; break;
      case KEY_ICON_3: infoMenu.menu[++infoMenu.cur] = menuManualLeveling; break;

      case KEY_ICON_4: // Bed Down
          storeCmd("G90\n"); // absolute position
          storeCmd("G1 Z175 F4800\n"); // move down
          break;

      case KEY_ICON_7: infoMenu.cur--; break;
      default: break;            
    }
    loopProcess();
  }
}

