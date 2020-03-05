//
//
//

#include <HeaterPreset.h>
#include "includes.h"

// dynamic preheat menu
MENUITEMS preheatItems =
        {
          // title
          LABEL_PREHEAT,
          // icon             label
          {
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Nozzle, LABEL_NOZZLE },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Stop, LABEL_ISSUE_RESET },
            { ICON_Back, LABEL_BACK },
          }
        };

const ITEM itemToolPreheat[] =
        {
          // icon              label
          { ICON_Preheat_Both, LABEL_PREHEAT_BOTH },
          { ICON_Hotbed, LABEL_BED },
          { ICON_Nozzle, LABEL_NOZZLE },
          { ICON_Nozzle, LABEL_NOZZLE },
          { ICON_Nozzle, LABEL_NOZZLE },
          { ICON_Nozzle, LABEL_NOZZLE },
          { ICON_Nozzle, LABEL_NOZZLE },
          { ICON_Nozzle, LABEL_NOZZLE },
        };

// material descriptor
typedef struct {
    bool use;
    u16 key;
    u16 icon;
    u16 label;
    int hotbed;
    int nozzle;
} PRESET_ENTRY;

// parsed teperature configuration
PRESET_ENTRY preset_list[PRESET_COUNT] =
        {
          { 0 },
        };

static TOOL_PREHEAT current_heater_tool = PREHEAT_NOZZLE0;

// preset entry config parser
#define PRESET_PARSE(NUM) \
    preset_list[NUM-1].use = config_parse_bool(config.heater_preset__preheat_##NUM##_use); \
    preset_list[NUM-1].key = config_parse_int(config.heater_preset__preheat_##NUM##_key); \
    preset_list[NUM-1].icon = config_find_icon(config.heater_preset__preheat_##NUM##_icon); \
    preset_list[NUM-1].label = config_find_label(config.heater_preset__preheat_##NUM##_label); \
    preset_list[NUM-1].hotbed = (int) config_parse_expr(config.heater_preset__preheat_##NUM##_hotbed); \
    preset_list[NUM-1].nozzle = (int) config_parse_expr(config.heater_preset__preheat_##NUM##_nozzle); \
// PRESET_PARSE

// extract preheat temps from config.ini
void parse_preset_data() {
    SYSTEM_CONFIG config = config_instance();
    PRESET_PARSE(1)
    PRESET_PARSE(2)
    PRESET_PARSE(3)
    PRESET_PARSE(4)
    PRESET_PARSE(5)
}

// activate configured point icons
void setup_preset_menu() {
    for (int index = 0; index < POINT_COUNT; index++) {
        PRESET_ENTRY preset = preset_list[index];
        if (preset.key == KEY_ICON_3) {
            continue;  // protect Tool
        }
        if (preset.key == KEY_ICON_6) {
            continue;  // protect Stop
        }
        if (preset.key == KEY_ICON_7) {
            continue;  // protect Back
        }
        if (preset.use) {
            preheatItems.items[preset.key].icon = preset.icon;
        }
    }
}

// perform temperature control only for enabled key
void perform_preset_apply(const KEY_VALUES key_num) {
    for (int index = 0; index < POINT_COUNT; index++) {
        PRESET_ENTRY preset = preset_list[index];
        if (preset.use && preset.key == key_num) {
            switch (current_heater_tool) {
            case PREHEAT_BOTH:
                heatSetTargetTemp(TOOL_HOTBED, preset.hotbed);
                heatSetTargetTemp(heatGetCurrentToolNozzle(), preset.nozzle);
                break;
            case PREHEAT_HOTBED:
                heatSetTargetTemp(TOOL_HOTBED, preset.hotbed);
                break;
            case PREHEAT_NOZZLE0:
                heatSetTargetTemp(heatGetCurrentToolNozzle(), preset.nozzle);
                break;
            }
            return;  // use only first match
        }
    }
}

// switch between Nozzle, Hotbed, or Both
void perform_switch_tool(const KEY_VALUES key_num) {
    current_heater_tool = (TOOL_PREHEAT) ((current_heater_tool + 1) % 3);
    preheatItems.items[key_num] = itemToolPreheat[current_heater_tool];
    menuDrawItem(&preheatItems.items[key_num], key_num);
}

// turn off heating
void perform_temperature_reset() {
    switch (current_heater_tool) {
    case PREHEAT_BOTH:
        heatSetTargetTemp(TOOL_HOTBED, 0);
        heatSetTargetTemp(heatGetCurrentToolNozzle(), 0);
        break;
    case PREHEAT_HOTBED:
        heatSetTargetTemp(TOOL_HOTBED, 0);
        break;
    case PREHEAT_NOZZLE0:
        heatSetTargetTemp(heatGetCurrentToolNozzle(), 0);
        break;
    }
}

void menuHeaterPreset(void) {
    KEY_VALUES key_num = KEY_IDLE;

    parse_preset_data();
    setup_preset_menu();
    menuDrawPage(&preheatItems);

    while (infoMenu.menu[infoMenu.cur] == menuHeaterPreset) {
        key_num = menuKeyGetValue();

        switch (key_num) {
        case KEY_ICON_0:  //
        case KEY_ICON_1:  //
        case KEY_ICON_2:  //
        case KEY_ICON_4:  //
        case KEY_ICON_5:  // Apply
            perform_preset_apply(key_num);
            break;
        case KEY_ICON_3:  // Switch
            perform_switch_tool(key_num);
            break;
        case KEY_ICON_6:  // Reset
            perform_temperature_reset();
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
