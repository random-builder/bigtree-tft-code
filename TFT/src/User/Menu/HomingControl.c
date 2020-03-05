#include <HomingControl.h>
#include "includes.h"

//1��title(����), ITEM_PER_PAGE��item(ͼ��+��ǩ) 
const MENUITEMS homeItems = {
//   title
LABEL_HOME,
// icon                       label
 {
  {ICON_Home,                 LABEL_HOME_ALL}, // 0
  {ICON_Home_X,               LABEL_X_HOME},
  {ICON_Home_Y,               LABEL_Y_HOME},
  {ICON_Home_Z,               LABEL_Z_HOME},

  {ICON_Dec_Z,                LABEL_LOWER_BED}, // 4
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Back,                 LABEL_BACK},}
};

void menuHoming(void)
{
  KEY_VALUES key_num = KEY_IDLE;	
  menuDrawPage(&homeItems);
  while(infoMenu.menu[infoMenu.cur] == menuHoming)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: storeCmd("G28\n");   break; // Home ALL
      case KEY_ICON_1: storeCmd("G28 X\n"); break; // Home X
      case KEY_ICON_2: storeCmd("G28 Y\n"); break; // Home Y
      case KEY_ICON_3: storeCmd("G28 Z\n"); break; // Home Z

      case KEY_ICON_4: // Bed Down
          storeCmd("G90\n"); // absolute position
          storeCmd("G1 Z175 F4800\n"); // move down
          break;

      case KEY_ICON_7: infoMenu.cur--;      break; // Back
      default:break;            
    }
    loopProcess();
  }
}

