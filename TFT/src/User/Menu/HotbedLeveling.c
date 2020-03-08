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
            { ICON_BLTouchRepeat, LABEL_ISSUE_RESET },
            { ICON_ProbeOffset, LABEL_PROBE_OFFSET },
            { ICON_BabyStep, LABEL_BABYSTEP },
            { ICON_Back, LABEL_BACK },
          }
        };

void menuAutoLeveling(void) {
    KEY_VALUE key_num = KEY_IDLE;
    const SYSTEM_CONFIG *config = config_instance();

    menuDrawPage(&autoLevelingItems);

    while (infoMenu.menu[infoMenu.cur] == menuAutoLeveling) {
        key_num = menuKeyGetValue();
        switch (key_num) {
        case KEY_ICON_0:  // Drop
            config_issue_gcode(config->leveling_auto__command_probe_drop);
            break;
        case KEY_ICON_1:  // Stow
            config_issue_gcode(config->leveling_auto__command_probe_stow);
            break;
        case KEY_ICON_2:  // Test
            config_issue_gcode(config->leveling_auto__command_probe_test);
            break;
        case KEY_ICON_3:  // Invoke
            config_issue_gcode(config->leveling_auto__command_probe_invoke);
            break;
        case KEY_ICON_4:  // Reset
            config_issue_gcode(config->leveling_auto__command_probe_reset);
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
MENUITEMS manualLevelingItems =
        {
          LABEL_HAND_LEVEL,
          {
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Home, LABEL_HOME_ALL },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK },
          }
        };

// a tune point
typedef struct {
    bool use;       // value from config.ini
    u16 key;        // value from config.ini
    u16 icon;       // icon index from icon_list.inc resolved from icon name in config.ini
    char *label;    // label-code or user-text from config.ini
    int X;          // value from config.ini
    int Y;          // value from config.ini
} POINT_ENTRY;

// parsed tune plane configuration
POINT_ENTRY point_entry_list[POINT_COUNT] =
        {
          { 0 },
        };

// entry point config parser
#define PARSE_POINT(NUM) \
    point_entry_list[NUM-1].use = config_parse_bool(config->leveling_manual__point_##NUM##_use); \
    point_entry_list[NUM-1].key = config_parse_int(config->leveling_manual__point_##NUM##_key); \
    point_entry_list[NUM-1].icon = config_find_icon(config->leveling_manual__point_##NUM##_icon); \
    point_entry_list[NUM-1].label = config->leveling_manual__point_##NUM##_label; \
    point_entry_list[NUM-1].X = (int) config_parse_expr(config->leveling_manual__point_##NUM##_X); \
    point_entry_list[NUM-1].Y = (int) config_parse_expr(config->leveling_manual__point_##NUM##_Y); \
// PARSE_POINT

// extract manual leveling points from config.ini
void parse_point_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_POINT(1)
    PARSE_POINT(2)
    PARSE_POINT(3)
    PARSE_POINT(4)
    PARSE_POINT(5)
}

// find language label-index by user label-code
static const u16 default_point_label_list[POINT_COUNT + 1] =
        {
          0,  // invalid user entry
          LABEL_POINT_1,
          LABEL_POINT_2,
          LABEL_POINT_3,
          LABEL_POINT_4,
          LABEL_POINT_5,
        };

// activate configured point icons
void setup_point_menu() {
    for (int index = 0; index < POINT_COUNT; index++) {
        POINT_ENTRY *point_entry = &(point_entry_list[index]);
        if (point_entry->key == KEY_ICON_3) {
            continue;  // protect "Home"
        }
        if (point_entry->key == KEY_ICON_7) {
            continue;  // protect "Back"
        }
        if (point_entry->use) {
            ITEM *menu_item = &(manualLevelingItems.items[point_entry->key]);
            menu_item->icon = point_entry->icon;
            const int label_code = config_parse_int(point_entry->label);  // 0 for non-number
            if (label_code == 0) {
                // has text: custom user label from config.ini
                menu_item->label.address = (uint8_t*) point_entry->label;
            } else if (0 < label_code && label_code < POINT_COUNT + 1) {
                // has number: lookup label index for the language
                const uint32_t label_index = default_point_label_list[label_code];
                menu_item->label.index = (uint32_t) label_index;
            } else {
                // FIXME: render error
            }
        }
    }
}

// perform actual motion sequence
void invoke_point_move(const POINT_ENTRY *point_entry) {
    const SYSTEM_CONFIG *config = config_instance();
    // home on demand
    if (coordinateIsClear() == false) {
        storeCmd("G28\n");
    }
    // extract config.ini values
    int feed_rate_XY = (int) config_parse_expr(config->leveling_manual__feed_rate_XY);
    int feed_rate_ZZ = (int) config_parse_expr(config->leveling_manual__feed_rate_ZZ);
    float point_Z_upper = config_parse_expr(config->leveling_manual__point_Z_upper);
    float point_Z_lower = config_parse_expr(config->leveling_manual__point_Z_lower);
    // lower plate
    storeCmd("G0 Z%.3f     F%d\n", point_Z_lower, feed_rate_ZZ);
    // move to point
    storeCmd("G0 X%d Y%d   F%d\n", point_entry->X, point_entry->Y, feed_rate_XY);
    // raise plate
    storeCmd("G0 Z%.3f     F%d\n", point_Z_upper, feed_rate_ZZ);
}

// perform point move only for enabled key
void perform_point_move(const KEY_VALUE key_num) {
    for (int index = 0; index < POINT_COUNT; index++) {
        POINT_ENTRY *point_entry = &(point_entry_list[index]);
        if (point_entry->use && point_entry->key == key_num) {
            invoke_point_move(point_entry);
            return;  // use only first match
        }
    }
}

void menuManualLeveling(void) {
    KEY_VALUE key_num = KEY_IDLE;
    const SYSTEM_CONFIG *config = config_instance();

    parse_point_data();
    setup_point_menu();
    menuDrawPage(&manualLevelingItems);

    while (infoMenu.menu[infoMenu.cur] == menuManualLeveling) {
        key_num = menuKeyGetValue();
        switch (key_num) {
        case KEY_ICON_3:
            config_issue_gcode(config->leveling_manual__command_reset_home);
            break;
        case KEY_ICON_7:
            infoMenu.cur--;
            break;
        default:
            perform_point_move(key_num);
            break;
        }
        loopProcess();
    }
}
