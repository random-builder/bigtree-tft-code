//
// filament load/unload
//

#include <FilamentControl.h>
#include "includes.h"

//
MENUITEMS ExtruderItems =
        {
          //   title
          LABEL_FILAMENT,
          {
            // icon       // label
            { ICON_Unload, LABEL_UNLOAD },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Load, LABEL_LOAD },
            { ICON_Heat, LABEL_PREHEAT },
            { ICON_Nozzle, LABEL_NOZZLE },
            { ICON_Normal, LABEL_NORMAL_SPEED },
            { ICON_Emm_5, LABEL_5_MM },
            { ICON_Back, LABEL_BACK }, }
        };

static u8 current_motor_index = 0;

#define SPEED_RATE_COUNT 3

static ITEM SpeedItems[SPEED_RATE_COUNT] =
        {
          // icon      // label
          { ICON_Slow, LABEL_SLOW_SPEED },
          { ICON_Normal, LABEL_NORMAL_SPEED },
          { ICON_Fast, LABEL_FAST_SPEED },
        };

// find language label-index by user label-code
static const u16 default_speed_label_list[POINT_COUNT + 1] =
        {
          0,  // invalid user entry
          LABEL_SLOW_SPEED,
          LABEL_NORMAL_SPEED,
          LABEL_FAST_SPEED,
        };

static u8 current_speed_index = 1;

#define LENGTH_STEP_COUNT 3

static ITEM LengthItems[LENGTH_STEP_COUNT] =
        {
          // icon      // label
          { ICON_Emm_1, LABEL_1_MM },
          { ICON_Emm_5, LABEL_5_MM },
          { ICON_Emm_10, LABEL_10_MM },
        };

static u8 length_step_list[LENGTH_STEP_COUNT] =
        {
          1,
          5,
          10,
        };

static u8 current_length_index = 1;

static float extruder_position = 0.0f;

const char *tool_change[EXTRUDER_COUNT] =  // TOOL_CHANGE;
        { 0 };  // all zero, build from config.ini

//const char *extruderDisplayID[EXTRUDER_COUNT] =  // EXTRUDER_ID;
//        { 0 };  // all zero, build from config.ini

// motor config descriptor
typedef struct {
    bool use;           // enable this tool
    //u8 key;           // key for index re-mapping
    u16 icon;           // tool icon index for display
    char *label;        // long tool name shown on regular icon and with edit mode
    char *gcode_tool;   // command for tool change
} MOTOR_ENTRY;

// parsed heater configuration
static MOTOR_ENTRY motor_entry_list[EXTRUDER_COUNT] =
        {
          { 0 },
        };

// extruder entry config parser
#define MOTOR_PARSE(NUM) \
    motor_entry_list[NUM-1].use = config_parse_bool(config->filament_control__tool_##NUM##_use); \
    motor_entry_list[NUM-1].icon = config_find_icon(config->filament_control__tool_##NUM##_icon); \
    motor_entry_list[NUM-1].label = config->filament_control__tool_##NUM##_label; \
    motor_entry_list[NUM-1].gcode_tool = config->filament_control__tool_##NUM##_gcode_tool; \
// MOTOR_PARSE

typedef struct {
    u16 rate;           // speed feed rate
    u16 icon;           // speed icon index for display
    char *label;        // label-code or user-text
} SPEED_ENTRY;

static SPEED_ENTRY speed_rate_list[SPEED_RATE_COUNT] =
        { 0 };  // loaded from config.ini

// speed feed rate entry config parser
#define SPEED_PARSE(NUM) \
    speed_rate_list[NUM-1].rate = config_parse_expr(config->filament_control__speed_##NUM##_rate); \
    speed_rate_list[NUM-1].icon = config_find_icon(config->filament_control__speed_##NUM##_icon); \
    speed_rate_list[NUM-1].label = config->filament_control__speed_##NUM##_label; \
// SPEED_PARSE

// extract extruder settings from config.ini
static void parse_motor_data(void) {
    const SYSTEM_CONFIG *config = config_instance();
    // nozzle 0 ... 5
    MOTOR_PARSE(1)
    MOTOR_PARSE(2)
    MOTOR_PARSE(3)
    MOTOR_PARSE(4)
    MOTOR_PARSE(5)
    MOTOR_PARSE(6)
    // speed rate 0 ... 2
    SPEED_PARSE(1)
    SPEED_PARSE(2)
    SPEED_PARSE(3)
    //
}

// customize tool menu item icon and label
static void setup_motor_icon(const u8 motor_index, const KEY_VALUE key_num) {
    MOTOR_ENTRY *motor_entry = &(motor_entry_list[motor_index]);
    ITEM *menu_item = &(ExtruderItems.items[key_num]);
    menu_item->icon = motor_entry->icon;
    menu_item->label.address = (uint8_t*) motor_entry->label;
}

// customize speed rate item icon and label
static void setup_speed_data(void) {
    for (u8 speed_index = 0; speed_index < SPEED_RATE_COUNT; speed_index++) {
        SPEED_ENTRY *speed_entry = &(speed_rate_list[speed_index]);
        ITEM *menu_item = &(SpeedItems[speed_index]);
        menu_item->icon = speed_entry->icon;
        const int label_code = config_parse_int(speed_entry->label);  // 0 for non-number
        if (label_code == 0) {
            // has text: custom user label from config.ini
            menu_item->label.address = (uint8_t*) speed_entry->label;
        } else if (0 <= label_code && label_code < SPEED_RATE_COUNT) {
            // has number: lookup label index for the language
            const uint32_t label_index = default_speed_label_list[label_code];
            menu_item->label.index = (uint32_t) label_index;
        } else {
            // FIXME: render error
        }
    }
}

// populate from config arrays-to-be-removed
static void setup_legacy_data(void) {
    for (u8 motor_index = 0; motor_index < EXTRUDER_COUNT; motor_index++) {
        const MOTOR_ENTRY *motor_entry = &(motor_entry_list[motor_index]);
        tool_change[motor_index] = motor_entry->gcode_tool;
//        extruderDisplayID[motor_index] = motor_entry->label;
    }
}

static void render_speed_icon(const KEY_VALUE key_num) {
    ExtruderItems.items[key_num] = SpeedItems[current_speed_index];
    menuDrawItem(&ExtruderItems.items[key_num], key_num);
}

static void render_length_icon(const KEY_VALUE key_num) {
    ExtruderItems.items[key_num] = LengthItems[current_length_index];
    menuDrawItem(&ExtruderItems.items[key_num], key_num);
}

static void render_extruder_label(void) {
    const GUI_RECT rect =
            { exhibitRect.x0, CENTER_Y - BYTE_HEIGHT, exhibitRect.x1, CENTER_Y };
    GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
    GUI_DispStringInPrect(&rect, (u8*) motor_entry_list[current_motor_index].label);
}

static void render_extruder_position(void) {
    const GUI_RECT rect =
            { exhibitRect.x0, CENTER_Y, exhibitRect.x1, CENTER_Y + BYTE_HEIGHT };
    char text_buff[36];
    my_sprintf(text_buff, "%.2f", extruder_position);
    GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
    GUI_DispStringInPrect(&rect, (u8*) text_buff);
}

static void render_extruder_status(void) {
    render_extruder_label();
    render_extruder_position();
}

static void render_extruder_icon(const KEY_VALUE key_num) {
    setup_motor_icon(current_motor_index, key_num);
    menuDrawItem(&ExtruderItems.items[key_num], key_num);
}

// report if tool is enabled in config.ini
bool extruder_has_use(const u8 motor_index) {
    return motor_entry_list[motor_index].use;
}

// select next enabled tool via round robin
u8 extruder_next_tool(const u8 this_index) {
    const u8 base_index = this_index + 1;
    for (u8 index = base_index; index < base_index + EXTRUDER_COUNT; index++) {
        u8 next_index = index % EXTRUDER_COUNT;
        if (extruder_has_use(next_index)) {
            return next_index;
        }
    }
    return this_index;
}

void menuFilament(void) {
    KEY_VALUE key_num = KEY_IDLE;

    u32 feed_rate = 0;
    bool has_relative_E = false;
    float extruder_original = 0.0f;
    float extruder_temporary = 0.0f;

    while (infoCmd.count != 0) {  // wait for empty queue
        loopProcess();
    }

    feed_rate = coordinateGetFeedRate();
    has_relative_E = eGetRelative();
    extruder_position = coordinateGetAxisTarget(E_AXIS);
    extruder_original = extruder_position;
    extruder_temporary = extruder_position;

    if (has_relative_E) {
        mustStoreCmd("M82\n");  // set extruder to absolute
    }

    parse_motor_data();
    setup_speed_data();
    setup_legacy_data();
    setup_motor_icon(current_motor_index, KEY_ICON_4);

    menuDrawPage(&ExtruderItems);
    render_extruder_status();
    render_speed_icon(KEY_ICON_5);
    render_length_icon(KEY_ICON_6);

    while (utility_has_menu_func(menuFilament)) {

        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:  // Unload
            extruder_temporary -= length_step_list[current_length_index];
            break;

        case KEY_ICON_1:  // TODO
            // exruder status popup
            break;

        case KEY_ICON_2:  // Load
            extruder_temporary += length_step_list[current_length_index];
            break;

        case KEY_ICON_3:  // Preheat
            infoMenu.menu[++infoMenu.cur] = menuHeaterPreset;
            break;

        case KEY_ICON_4:  // Switch Tool
            current_motor_index = extruder_next_tool(current_motor_index);
            render_extruder_status();
            render_extruder_icon(key_num);
            break;

        case KEY_ICON_5:  // Switch Speed
            current_speed_index = (current_speed_index + 1) % SPEED_RATE_COUNT;
            render_speed_icon(key_num);
            break;

        case KEY_ICON_6:  // Switch Length
            current_length_index = (current_length_index + 1) % LENGTH_STEP_COUNT;
            render_length_icon(key_num);
            break;

        case KEY_ICON_7:  // Back
            infoMenu.cur--;
            break;

        default:
            break;
        }
        if (extruder_position != extruder_temporary) {
            extruder_position = extruder_temporary;
            render_extruder_position();
            // FIXME decouple from heater
            if (current_motor_index != heatGetCurrentToolNozzle() - TOOL_NOZZLE0) {
                config_issue_gcode(motor_entry_list[current_motor_index].gcode_tool);
            }
            storeCmd("G0 E%.5f F%d\n", extruder_position, speed_rate_list[current_speed_index]);
        }
        loopProcess();
    }

    mustStoreCmd("G92 E%.5f\n", extruder_original);
    mustStoreCmd("G0 F%d\n", feed_rate);

    if (has_relative_E) {
        mustStoreCmd("M83\n");  // resotre extruder to relative
    }
}
