//
// walk around coordinates
//

#include <PositionControl.h>
#include "includes.h"

// menu page defined in config.ini
static MENUITEMS PositionPage =
        {
          //  title
          LABEL_MOVE,
          // icon         // label
          {
            //
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Mmm_1, LABEL_1_MM },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK },
          }
        };

#define LENGTH_STEP_COUNT 3

static const ITEM LengthStepItem[LENGTH_STEP_COUNT] =
        {
          // icon                       label
          { ICON_Mmm_01, LABEL_01_MM },
          { ICON_Mmm_1, LABEL_1_MM },
          { ICON_Mmm_10, LABEL_10_MM },
        };

static const float length_step_list[LENGTH_STEP_COUNT] =
        {
          0.1f,
          1.0f,
          10.0f,
        };

#define SPEED_RATE_COUNT 3

typedef struct {
    u16 rate;           // speed feed rate
    u16 icon;           // speed icon index for display
    char *label;        // label-code or user-text
} SPEED_ENTRY;

static SPEED_ENTRY speed_rate_list[SPEED_RATE_COUNT] =
        { 0 };  // loaded from config.ini

// speed feed rate entry config parser
#define SPEED_PARSE(NUM) \
    speed_rate_list[NUM-1].rate = config_parse_expr(config->position_control__speed_##NUM##_rate); \
    speed_rate_list[NUM-1].icon = config_find_icon(config->position_control__speed_##NUM##_icon); \
    speed_rate_list[NUM-1].label = config->position_control__speed_##NUM##_label; \
// SPEED_PARSE

static u8 current_length_index = 1;

static u32 current_time_stamp = 0;

static u32 position_update_timeout = 50;  // 1 seconds is 100

// default lables for label-code lookup
static uint32_t default_label_list[UTILITY_DEFAULT_LABEL_COUNT] =
        {
          0,  // invalid user entry
          LABEL_Z_DEC,  // label-code=1
          LABEL_Y_INC,  // label-code=2
          LABEL_Z_INC,  // label-code=3
          LABEL_1_MM,
          LABEL_X_DEC,  // label-code=5
          LABEL_Y_DEC,  // label-code=6
          LABEL_X_INC,  // label-code=7
          LABEL_BACK,
        };

// (2 directions) * (3 axis)
#define CONFIG_ENTRY_COUNT 6

// extended config fields
typedef struct {
    char *shift;  // axis and direction of move
} EXTEND_ENTRY;

// default position command configuration
static CONFIG_ENTRY config_entry_list[CONFIG_ENTRY_COUNT] =
        {
          { 0 },
        };

// extended position command configuration
static EXTEND_ENTRY extend_entry_list[CONFIG_ENTRY_COUNT] =
        {
          { 0 },
        };

#define BUTTON_PROTECT_COUNT 2

// list of button keys protected from user config
static KEY_VALUE button_protect_list[BUTTON_PROTECT_COUNT] =
        {
          KEY_ICON_3,  // Switch Length
          BUTTON_BACK,
        };

// position entry config parser
#define PARSE_CONFIG(NUM) \
    config_entry_list[NUM-1].use = config_parse_bool(config->position_control__entry_##NUM##_use); \
    config_entry_list[NUM-1].key = config_parse_int(config->position_control__entry_##NUM##_key); \
    config_entry_list[NUM-1].icon = config_find_icon(config->position_control__entry_##NUM##_icon); \
    config_entry_list[NUM-1].label = config->position_control__entry_##NUM##_label; \
    config_entry_list[NUM-1].gcode = config->position_control__entry_##NUM##_gcode; \
    extend_entry_list[NUM-1].shift = config->position_control__entry_##NUM##_shift; \
// PARSE_CONFIG

// load motion profile from config.ini
static void parse_position_data(void) {
    const SYSTEM_CONFIG *config = config_instance();
    // menu item 0 ... 5
    PARSE_CONFIG(1)
    PARSE_CONFIG(2)
    PARSE_CONFIG(3)
    PARSE_CONFIG(4)
    PARSE_CONFIG(5)
    PARSE_CONFIG(6)
    // speed rate 0 ... 2
    SPEED_PARSE(1)
    SPEED_PARSE(2)
    SPEED_PARSE(3)
    //
}

// build and draw postion menu
static void render_position_page(void) {
    utility_setup_config_menu(
            &PositionPage,
            default_label_list,
            CONFIG_ENTRY_COUNT,
            config_entry_list,
            BUTTON_PROTECT_COUNT,
            button_protect_list
            );
    menuDrawPage(&PositionPage);
}

// toggle motion length through the list
static void perform_length_change(const KEY_VALUE key_num) {
    current_length_index = (current_length_index + 1) % LENGTH_STEP_COUNT;
    PositionPage.items[key_num] = LengthStepItem[current_length_index];
    menuDrawItem(&PositionPage.items[key_num], key_num);
}

// convert axis/direction token into -1/+1 factor
static int compute_length_factor(const char *shift) {
    if (false) {
        // noop
    } else if (utility_has_matach(shift, "X-")) {
        if (infoSettings.invert_xaxis == 0) {
            return -1;
        } else {
            return +1;
        }
    } else if (utility_has_matach(shift, "X+")) {
        if (infoSettings.invert_xaxis == 0) {
            return +1;
        } else {
            return -1;
        }
    } else if (utility_has_matach(shift, "Y-")) {
        if (infoSettings.invert_yaxis == 0) {
            return -1;
        } else {
            return +1;
        }
    } else if (utility_has_matach(shift, "Y+")) {
        if (infoSettings.invert_yaxis == 0) {
            return +1;
        } else {
            return -1;
        }
    } else if (utility_has_matach(shift, "Z-")) {
        if (infoSettings.invert_zaxis == 0) {
            return -1;
        } else {
            return +1;
        }
    } else if (utility_has_matach(shift, "Z+")) {
        if (infoSettings.invert_zaxis == 0) {
            return +1;
        } else {
            return -1;
        }
    } else {
        return 0;  // failure
    }
}

// issue gcode to change head position
static void perform_position_command(const KEY_VALUE key_num) {
    const float length_value = length_step_list[current_length_index];
    for (int entry_index = 0; entry_index < CONFIG_ENTRY_COUNT; entry_index++) {
        const CONFIG_ENTRY *config_entry = &(config_entry_list[entry_index]);
        const EXTEND_ENTRY *extend_entry = &(extend_entry_list[entry_index]);
        if (config_entry->use && config_entry->key == key_num) {
            const char *gcode = config_entry->gcode;
            const char *shift = extend_entry->shift;
            const int length_factor = compute_length_factor(shift);
            const float length_result = length_value * length_factor;
            char command_text[128];
            my_sprintf(command_text, gcode, length_result);
            config_issue_gcode(command_text);
            //popupReminder((u8*) "shift", (u8*) shift);
            return;
        }
    }
}

// find head location
static char* command_postion_query() {
    const SYSTEM_CONFIG *config = config_instance();
    return config->position_control__command_postion_query;
}

// invoke before move
static char* command_move_prefix() {
    const SYSTEM_CONFIG *config = config_instance();
    return config->position_control__command_move_prefix;
}

// invoke after the move
static char* command_move_suffix() {
    const SYSTEM_CONFIG *config = config_instance();
    return config->position_control__command_move_suffix;
}

// change motion speed
static char* command_feed_rate() {
    const SYSTEM_CONFIG *config = config_instance();
    return config->position_control__command_feed_rate;
}

// display full position banner
static void render_position_layout(void) {
    char text_buff[128];
    my_sprintf(text_buff, "X:%.1f  ", getAxisLocation(0));
    GUI_DispString(START_X + 1 * SPACE_X + 1 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
    my_sprintf(text_buff, "Y:%.1f  ", getAxisLocation(1));
    GUI_DispString(START_X + 2 * SPACE_X + 2 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
    my_sprintf(text_buff, "Z:%.1f  ", getAxisLocation(2));
    GUI_DispString(START_X + 3 * SPACE_X + 3 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
}

// update only banner position values
static void render_position_update(void) {
    if (OS_GetTime() > current_time_stamp + position_update_timeout)
            {
        if (infoHost.connected == true && infoHost.wait == false) {
            config_issue_gcode(command_postion_query());
        }
        render_position_layout();
        current_time_stamp = OS_GetTime();
    }
}

// issue gcode to control motion speed
static void perform_command_feed_rate() {
    const int speed_index = limitValue(0, infoSettings.move_speed, SPEED_RATE_COUNT - 1);
    SPEED_ENTRY speed_entry;
    switch (speed_index) {
    case 1:  // slow
        speed_entry = speed_rate_list[0];
        break;
    case 0:  // norm
        speed_entry = speed_rate_list[1];
        break;
    case 2:  // fast
        speed_entry = speed_rate_list[2];
        break;
    default:  // failure
        speed_entry = speed_rate_list[1];
        break;
    }
    const char *speed_gcode = command_feed_rate();
    char command_text[128];
    my_sprintf(command_text, speed_gcode, speed_entry.rate);
    config_issue_gcode(command_text);
    //popupReminder((u8*) "feed rate", (u8*) command_text);
}

// issue gcode commands before navigation start
static void perform_startup_command(void) {
    config_issue_gcode(command_move_prefix());
    config_issue_gcode(command_postion_query());
    perform_command_feed_rate();
}

// issue gcode commands after navigation finish
static void perform_terminate_command(void) {
    config_issue_gcode(command_move_suffix());
}

void menuPosition(void) {
    KEY_VALUE key_num = KEY_IDLE;

    parse_position_data();
    render_position_page();
    render_position_layout();
    perform_startup_command();

    while (infoMenu.menu[infoMenu.cur] == menuPosition) {
        key_num = menuKeyGetValue();
        switch (key_num) {
        case KEY_ICON_3:  // Switch Length
            perform_length_change(key_num);
            break;
        case BUTTON_BACK:
            infoMenu.cur--;
            break;
        default:
            perform_position_command(key_num);
            break;
        }
        loopProcess();
        render_position_update();
    }

    perform_terminate_command();

}
