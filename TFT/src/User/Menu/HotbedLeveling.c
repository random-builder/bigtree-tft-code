//
// automatic and manual hot bed leveling
//

#include <HotbedLeveling.h>
#include "includes.h"

const MENUITEMS autoLevelingItems =
        { LABEL_AUTO_LEVEL,
          {
            { ICON_BLTouchDeploy, LABEL_BLTOUCH_DEPLOY },
            { ICON_BLTouchStow, LABEL_BLTOUCH_STOW },
            { ICON_BLTouchTest, LABEL_BLTOUCH_TEST },
            { ICON_BLTouch, LABEL_AUTO_PROBE },
            { ICON_BLTouchRepeat, LABEL_BLTOUCH_REPEAT },
            { ICON_ProbeOffset, LABEL_PROBE_OFFSET },
            { ICON_BabyStep, LABEL_BABYSTEP },
            { ICON_Back, LABEL_BACK },
          }
        };

void menuAutoLeveling(void) {
    KEY_VALUES key_num = KEY_IDLE;
    SYSTEM_CONFIG config = config_instance();

    menuDrawPage(&autoLevelingItems);

    while (infoMenu.menu[infoMenu.cur] == menuAutoLeveling) {
        key_num = menuKeyGetValue();
        switch (key_num) {
        case KEY_ICON_0:  // Drop
            config_issue_gcode(config.leveling_auto__command_probe_drop);
            break;
        case KEY_ICON_1:  // Stow
            config_issue_gcode(config.leveling_auto__command_probe_stow);
            break;
        case KEY_ICON_2:  // Test
            config_issue_gcode(config.leveling_auto__command_probe_test);
            break;

        case KEY_ICON_3:  // Invoke
            config_issue_gcode(config.leveling_auto__command_probe_invoke);
            break;

        case KEY_ICON_4:  // TODO
            config_issue_gcode(config.leveling_auto__command_probe_repeat);
            break;
        case KEY_ICON_5:  // Offset Z
            infoMenu.menu[++infoMenu.cur] = menuProbeOffset;
            break;
        case KEY_ICON_6:  // Baby Step
            infoMenu.menu[++infoMenu.cur] = menuBabyStep;
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

// dynamic tune point menu
/* vars */MENUITEMS manualLevelingItems =
        {
          LABEL_HAND_LEVEL,
          {
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_Home, LABEL_HOME_ALL },
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_EMPTY, LABEL_BACKGROUND },
            { ICON_Back, LABEL_BACK },
          }
        };

// single tune point
typedef struct {
    bool use;
    u16 key;
    u16 X;
    u16 Y;
    u16 icon;
} POINT_ENTRY;

// parsed tune plane configuration
POINT_ENTRY point_list[POINT_COUNT] =
        {
          { 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0 },
        };

void moveToLevelingPoint(u8 point) {

    static const s16 pointPosition[][2] =
            {
              { LEVELING_POINT_1_X, LEVELING_POINT_1_Y },
              { LEVELING_POINT_2_X, LEVELING_POINT_2_Y },
              { LEVELING_POINT_3_X, LEVELING_POINT_3_Y },
              { LEVELING_POINT_4_X, LEVELING_POINT_4_Y },
              { LEVELING_POINT_5_X, LEVELING_POINT_5_Y },
            };

    if (coordinateIsClear() == false) {
        storeCmd("G28\n");
    }

    storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_MOVE_Z, LEVELING_POINT_Z_FEEDRATE);
    storeCmd("G0 X%d Y%d F%d\n", pointPosition[point][0], pointPosition[point][1], LEVELING_POINT_XY_FEEDRATE);
    storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_Z, LEVELING_POINT_Z_FEEDRATE);

}

// extract manual leveling points from config.ini
void parse_point_data() {
    SYSTEM_CONFIG config = config_instance();
    // Point 1
    point_list[0].use = config_parse_bool(config.leveling_manual__point_1_use);
    point_list[0].key = config_parse_int(config.leveling_manual__point_1_key);
    point_list[0].X = config_parse_int(config.leveling_manual__point_1_X);
    point_list[0].Y = config_parse_int(config.leveling_manual__point_1_Y);
    point_list[0].icon = config_find_icon(config.leveling_manual__point_1_icon);
    // Point 2
    point_list[1].use = config_parse_bool(config.leveling_manual__point_2_use);
    point_list[1].key = config_parse_int(config.leveling_manual__point_2_key);
    point_list[1].X = config_parse_int(config.leveling_manual__point_2_X);
    point_list[1].Y = config_parse_int(config.leveling_manual__point_2_Y);
    point_list[1].icon = config_find_icon(config.leveling_manual__point_2_icon);
    // Point 3
    point_list[2].use = config_parse_bool(config.leveling_manual__point_3_use);
    point_list[2].key = config_parse_int(config.leveling_manual__point_3_key);
    point_list[2].X = config_parse_int(config.leveling_manual__point_3_X);
    point_list[2].Y = config_parse_int(config.leveling_manual__point_3_Y);
    point_list[2].icon = config_find_icon(config.leveling_manual__point_3_icon);
    // Point 4
    point_list[3].use = config_parse_bool(config.leveling_manual__point_4_use);
    point_list[3].key = config_parse_int(config.leveling_manual__point_4_key);
    point_list[3].X = config_parse_int(config.leveling_manual__point_4_X);
    point_list[3].Y = config_parse_int(config.leveling_manual__point_4_Y);
    point_list[3].icon = config_find_icon(config.leveling_manual__point_4_icon);
    // Point 5
    point_list[4].use = config_parse_bool(config.leveling_manual__point_5_use);
    point_list[4].key = config_parse_int(config.leveling_manual__point_5_key);
    point_list[4].X = config_parse_int(config.leveling_manual__point_5_X);
    point_list[4].Y = config_parse_int(config.leveling_manual__point_5_Y);
    point_list[4].icon = config_find_icon(config.leveling_manual__point_5_icon);
}

// activate configured point icons
void setup_point_menu() {
    for (int index = 0; index < POINT_COUNT; index++) {
        POINT_ENTRY point = point_list[index];
        if (point.key == KEY_ICON_3) {
            continue;  // protect Home
        }
        if (point.key == KEY_ICON_7) {
            continue;  // protect Back
        }
        if (point.use) {
            manualLevelingItems.items[point.key].icon = point.icon;
        }
    }
}

// perform actual motion sequence
void invoke_point_move(const POINT_ENTRY point) {
    point.X;
    point.Y;
}

// perform point move only for enabled key
void perform_point_move(const KEY_VALUES key_num) {
    for (int index = 0; index < POINT_COUNT; index++) {
        POINT_ENTRY point = point_list[index];
        if (point.use && point.key == key_num) {
            invoke_point_move(point);
            return;  // use only first match
        }
    }
}

void menuManualLeveling(void) {
    KEY_VALUES key_num = KEY_IDLE;
    SYSTEM_CONFIG config = config_instance();

    parse_point_data();
    setup_point_menu();

    menuDrawPage(&manualLevelingItems);

    while (infoMenu.menu[infoMenu.cur] == menuManualLeveling) {
        key_num = menuKeyGetValue();
        perform_point_move(key_num);
        switch (key_num) {
        case KEY_ICON_3:
            config_issue_gcode(config.leveling_manual__command_reset_home);
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
