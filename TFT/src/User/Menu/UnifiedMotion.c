#include <UnifiedMotion.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items(icon + label)
static const MENUITEMS UnifiedMovePage =
        {
          // title
          LABEL_MOTION,
          // icon                       label
          {
            { ICON_Home, LABEL_HOMING },
            { ICON_Move, LABEL_WALKING },
            { ICON_Leveling, LABEL_AUTO_LEVEL },
            { ICON_ManualLevel, LABEL_HAND_LEVEL },
            { ICON_BabyStep, LABEL_MINI_STEP },
            { ICON_ProbeOffset, LABEL_PROBE_OFFSET },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK }, }
        };

void menuUniMotion(void) {
    KEY_VALUE key_num = KEY_IDLE;

    menuDrawPage(&UnifiedMovePage);

    while (utility_has_menu_func(menuUniMotion)) {
        key_num = menuKeyGetValue();

        switch (key_num) {

        case KEY_ICON_0:
            infoMenu.menu[++infoMenu.cur] = menuHoming;
            break;
        case KEY_ICON_1:
            infoMenu.menu[++infoMenu.cur] = menuPosition;
            break;
        case KEY_ICON_2:
            infoMenu.menu[++infoMenu.cur] = menuAutoLeveling;
            break;
        case KEY_ICON_3:
            infoMenu.menu[++infoMenu.cur] = menuManualLeveling;
            break;

        case KEY_ICON_4:  // Steps Z
            infoMenu.menu[++infoMenu.cur] = menu_tinker_mini_step;
            break;
        case KEY_ICON_5:  // Probe Z
            infoMenu.menu[++infoMenu.cur] = menu_tinker_probe_offset;
            break;
        case KEY_ICON_6:  // TODO
            break;

        case BUTTON_BACK:
            infoMenu.cur--;
            break;
        default:
            break;
        }
        loopProcess();
    }
}
