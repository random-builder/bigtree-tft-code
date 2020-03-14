//
// main menu
//

#include <ArkonMenu.h>
#include <CustomCommand.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon+label) 
static const MENUITEMS MainPage =
        {
          // title
          LABEL_READY,
          // icon           // label
          {
            { ICON_Home_Move, LABEL_MOTION },
            { ICON_Heat_Fan, LABEL_HEATING },
            { ICON_Extrude, LABEL_FILAMENT },
            { ICON_Settings, LABEL_SETTINGS },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Gcode, LABEL_GCODE },
            { ICON_Custom, LABEL_CUSTOM },
            { ICON_Back, LABEL_BACK }
          }
        };

void menuMain(void) {
    KEY_VALUE key_num = KEY_IDLE;
    GUI_SetBkColor(ST7920_BKCOLOR);
    menuDrawPage(&MainPage);

    while (utility_has_menu_func(menuMain)) {
        key_num = menuKeyGetValue();

        switch (key_num) {
        case KEY_ICON_0:
            infoMenu.menu[++infoMenu.cur] = menuUniMotion;
            break;
        case KEY_ICON_1:
            infoMenu.menu[++infoMenu.cur] = menuUniHeating;
            break;
        case KEY_ICON_2:
            infoMenu.menu[++infoMenu.cur] = menuFilament;
            break;
        case KEY_ICON_3:
            infoMenu.menu[++infoMenu.cur] = menuSettings;
            break;
        case KEY_ICON_4:  // TODO
            break;
        case KEY_ICON_5:
            infoMenu.menu[++infoMenu.cur] = menuInvokeGcode;
            break;
        case KEY_ICON_6:
            infoMenu.menu[++infoMenu.cur] = menuCustomCommand;
            break;
        case KEY_ICON_7:
            infoMenu.cur--;
            break;
        default:
            break;
        }
        loopProcess();
    }

}
