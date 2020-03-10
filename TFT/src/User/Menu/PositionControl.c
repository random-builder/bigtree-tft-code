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

const ITEM LengthStepItem[LENGTH_STEP_COUNT] =
        {
          // icon                       label
          { ICON_Mmm_01, LABEL_01_MM },
          { ICON_Mmm_1, LABEL_1_MM },
          { ICON_Mmm_10, LABEL_10_MM },
        };

const float length_step_list[LENGTH_STEP_COUNT] =
        {
          0.1f,
          1,
          10,
        };

static u8 current_length_index = 1;

static u32 nowTime = 0;

static u32 update_time = 50;  // 1 seconds is 100

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
          KEY_ICON_3,  // length
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
    // expose parent struct
    PARSE_CONFIG(1)
    PARSE_CONFIG(2)
    PARSE_CONFIG(3)
    PARSE_CONFIG(4)
    PARSE_CONFIG(5)
    PARSE_CONFIG(6)
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

//
static void update_length_value(const KEY_VALUE key_num) {
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
    const int length_value = length_step_list[current_length_index];
    for (int entry_index = 0; entry_index < CONFIG_ENTRY_COUNT; entry_index++) {
        const CONFIG_ENTRY *config_entry = &(config_entry_list[entry_index]);
        const EXTEND_ENTRY *parser_entry = &(extend_entry_list[entry_index]);
        if (config_entry->use && config_entry->key == key_num) {
            const char *gcode = config_entry->gcode;
            const char *shift = parser_entry->shift;
            const int length_factor = compute_length_factor(shift);
            const float length_result = length_value * length_factor;
            char command_text[128];
            my_sprintf(command_text, gcode, length_result);
            config_issue_gcode(command_text);
            //popupReminder((u8*) "debug", (u8*) shift);
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

static void render_position(void) {
    char text_buff[128];
    my_sprintf(text_buff, "X:%.1f  ", getAxisLocation(0));
    GUI_DispString(START_X + 1 * SPACE_X + 1 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
    my_sprintf(text_buff, "Y:%.1f  ", getAxisLocation(1));
    GUI_DispString(START_X + 2 * SPACE_X + 2 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
    my_sprintf(text_buff, "Z:%.1f  ", getAxisLocation(2));
    GUI_DispString(START_X + 3 * SPACE_X + 3 * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (u8*) text_buff);
}

static void update_position(void) {
    if (OS_GetTime() > nowTime + update_time)
            {
        if (infoHost.connected == true && infoHost.wait == false) {
            storeCmd("M114\n");
        }
        render_position();
        nowTime = OS_GetTime();
    }
}

void menuPosition(void) {
    KEY_VALUE key_num = KEY_IDLE;

    parse_position_data();
    render_position_page();

    config_issue_gcode(command_move_prefix());
    config_issue_gcode(command_postion_query());

    switch (infoSettings.move_speed) {
    case 1:
        mustStoreCmd("G1 F%d\n", SPEED_MOVE_SLOW);
        break;
    case 2:
        mustStoreCmd("G1 F%d\n", SPEED_MOVE_FAST);
        break;
    default:
        mustStoreCmd("G1 F%d\n", DEFAULT_SPEED_MOVE);
        break;
    }

    render_position();

    while (infoMenu.menu[infoMenu.cur] == menuPosition) {
        key_num = menuKeyGetValue();

        switch (key_num) {

        case KEY_ICON_3:
            update_length_value(key_num);
            break;

        case BUTTON_BACK:
            infoMenu.cur--;
            break;

        default:
            perform_position_command(key_num);
            break;

        }

        loopProcess();

        update_position();

    }

    config_issue_gcode(command_move_suffix());

}
