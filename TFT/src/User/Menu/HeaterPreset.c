//
//
//

#include <HeaterPreset.h>
#include "includes.h"

// icon text overlay location
#define ICON_TEXT_X(COL)   ( (COL) * ICON_WIDTH  + (COL-1) * SPACE_X + START_X - BYTE_WIDTH/2 )
#define ICON_TEXT_Y(ROW)   ( (ROW) * ICON_HEIGHT + (ROW)   * SPACE_Y - 7       - BYTE_HEIGHT/2 )

// icon text overlay location
static const GUI_POINT position_icon_text[8] =
        {
          { ICON_TEXT_X(1), ICON_TEXT_Y(1) },
          { ICON_TEXT_X(2), ICON_TEXT_Y(1) },
          { ICON_TEXT_X(3), ICON_TEXT_Y(1) },
          { ICON_TEXT_X(4), ICON_TEXT_Y(1) },
          { ICON_TEXT_X(1), ICON_TEXT_Y(2) },
          { ICON_TEXT_X(2), ICON_TEXT_Y(2) },
          { ICON_TEXT_X(3), ICON_TEXT_Y(2) },
          { ICON_TEXT_X(4), ICON_TEXT_Y(2) },
        };

// dynamic preheat menu
static MENUITEMS PreheatItems =
        {
          // title
          LABEL_PREHEAT,
          // icon             label
          {
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Preheat_Both, LABEL_PREHEAT_BOTH },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_CoolDown, LABEL_ISSUE_RESET },
            { ICON_Back, LABEL_BACK },
          }
        };

static const ITEM PreheatToolItems[] =
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
    char *label;
    int hotbed;
    int nozzle;
} PRESET_ENTRY;

// parsed teperature configuration
static PRESET_ENTRY preset_entry_list[PRESET_COUNT] =
        {
          { 0 },
        };

// remember selected tool between invocations
static TOOL_PREHEAT current_heater_tool = PREHEAT_BOTH;

// preset entry config parser
#define PRESET_PARSE(NUM) \
    preset_entry_list[NUM-1].use = config_parse_bool(config->heater_preset__preheat_##NUM##_use); \
    preset_entry_list[NUM-1].key = config_parse_int(config->heater_preset__preheat_##NUM##_key); \
    preset_entry_list[NUM-1].icon = config_find_icon(config->heater_preset__preheat_##NUM##_icon); \
    preset_entry_list[NUM-1].label = config->heater_preset__preheat_##NUM##_label; \
    preset_entry_list[NUM-1].hotbed = (int) config_parse_expr(config->heater_preset__preheat_##NUM##_hotbed); \
    preset_entry_list[NUM-1].nozzle = (int) config_parse_expr(config->heater_preset__preheat_##NUM##_nozzle); \
// PRESET_PARSE

// extract preheat temps from config.ini
void parse_preset_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PRESET_PARSE(1)
    PRESET_PARSE(2)
    PRESET_PARSE(3)
    PRESET_PARSE(4)
    PRESET_PARSE(5)
}

// show hotend/hotbed values on top of preheat icons
void render_icon_text(const PRESET_ENTRY *preset_entry) {
    char icon_text[32];
    my_sprintf(icon_text, "%d/%d", preset_entry->nozzle, preset_entry->hotbed);
    const GUI_POINT *position = &(position_icon_text[preset_entry->key]);
    GUI_SetColor(WHITE);
    GUI_SetBkColor(RED);
    GUI_DispStringRight(position->x, position->y, (u8*) icon_text);
    GUI_RestoreColorDefault();
}

// show hotend/hotbed values on top of preheat icons
void render_menu_text_overlay() {
    for (int index = 0; index < PRESET_COUNT; index++) {
        const PRESET_ENTRY *preset_entry = &(preset_entry_list[index]);
        if (preset_entry->use) {
            render_icon_text(preset_entry);
        }
    }
}

// activate configured point icons
void setup_preset_menu() {
    for (int index = 0; index < PRESET_COUNT; index++) {
        const PRESET_ENTRY *preset_entry = &(preset_entry_list[index]);
        if (preset_entry->key == KEY_ICON_3) {
            continue;  // protect "Tool"
        }
        if (preset_entry->key == KEY_ICON_6) {
            continue;  // protect "Stop"
        }
        if (preset_entry->key == KEY_ICON_7) {
            continue;  // protect "Back"
        }
        if (preset_entry->use) {
            ITEM *menu_item = &(PreheatItems.items[preset_entry->key]);
            menu_item->icon = preset_entry->icon;
            menu_item->label.address = (uint8_t*) preset_entry->label;
        }
    }
}

// perform temperature control only for enabled key
void perform_preset_apply(const KEY_VALUES key_num) {
    for (int index = 0; index < PRESET_COUNT; index++) {
        const PRESET_ENTRY *preset_entry = &(preset_entry_list[index]);
        if (preset_entry->use && preset_entry->key == key_num) {
            render_icon_text(preset_entry);
            switch (current_heater_tool) {
            case PREHEAT_BOTH:
                heatSetTargetTemp(TOOL_HOTBED, preset_entry->hotbed);
                heatSetTargetTemp(heatGetCurrentToolNozzle(), preset_entry->nozzle);
                break;
            case PREHEAT_HOTBED:
                heatSetTargetTemp(TOOL_HOTBED, preset_entry->hotbed);
                break;
            case PREHEAT_NOZZLE0:
                heatSetTargetTemp(heatGetCurrentToolNozzle(), preset_entry->nozzle);
                break;
            }
            return;  // use only first match
        }
    }
}

// switch between Nozzle, Hotbed, or Both
void perform_switch_tool(const KEY_VALUES key_num) {
    current_heater_tool = (TOOL_PREHEAT) ((current_heater_tool + 1) % 3);
    PreheatItems.items[key_num] = PreheatToolItems[current_heater_tool];
    menuDrawItem(&PreheatItems.items[key_num], key_num);
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
    menuDrawPage(&PreheatItems);
    render_menu_text_overlay();

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
