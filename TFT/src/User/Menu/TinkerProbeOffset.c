//
//
//

#include <TinkerProbeOffset.h>
#include "includes.h"

//
static MENUITEMS ProbeOffsetPage =
        {
          // title
          LABEL_PROBE_OFFSET,
          // icon                        label
          {
            { ICON_Dec, LABEL_DEC },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Inc, LABEL_INC },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_EEPROMSave, LABEL_EEPROM_SAVE },
            { ICON_Mmm_01, LABEL_01_MM },
            { ICON_Normal, LABEL_VALUE_ZERO },
            { ICON_Back, LABEL_BACK },
          }
        };

#define LENGTH_BLOCK_COUNT  3

static const ITEM ProbeOffsetEntryList[LENGTH_BLOCK_COUNT] =
        {
          // icon         // label
          { ICON_Mmm_001, LABEL_001_MM },
          { ICON_Mmm_01, LABEL_01_MM },
          { ICON_Mmm_1, LABEL_1_MM },
        };

static const float probe_offset_length_list[LENGTH_BLOCK_COUNT] =
        {
          0.01f,
          0.10f,
          1.00f,
        };

static float current_probe_offset = 0.0;

static void render_probe_offset(void) {
    GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, current_probe_offset, 3, 2, RIGHT);
}

typedef enum {
    EDIT_NONE,
    EDIT_DEC,
    EDIT_INC,
} EDIT_MODE;

static int current_length_index = 1;

// probster adjustment range
static float config_range_min(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config_parse_expr(config->tinker_probster__range_min);
}

// probster adjustment range
static float config_range_max(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config_parse_expr(config->tinker_probster__range_max);
}

// probster gcode for query
static char* config_command_query(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config->tinker_probster__command_query;
}

// probster gcode for change
static char* config_command_edit(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config->tinker_probster__command_edit;
}

// probster gcode for eeprom persist
static char* config_command_save(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config->tinker_probster__command_save;
}

//
static void perform_switch_unit(const KEY_VALUE key_num) {
    current_length_index = (current_length_index + 1) % LENGTH_BLOCK_COUNT;
    ProbeOffsetPage.items[key_num] = ProbeOffsetEntryList[current_length_index];
    menuDrawItem(&ProbeOffsetPage.items[key_num], key_num);
}

static void perform_probster_gcode(const float probster_change) {
    char command_text[64];
    my_sprintf(command_text, config_command_edit(), probster_change);
    config_issue_gcode(command_text);
}

static void perform_command_reset(void) {
    const float probster_change = -current_probe_offset;
    current_probe_offset = 0.0;
    perform_probster_gcode(probster_change);
}

//
static void perform_command_query(void) {
    config_issue_gcode(config_command_query());
}
//
static void perform_command_save(void) {
    config_issue_gcode(config_command_save());
}

//
static void perform_command_edit(const EDIT_MODE edit_mode) {
    const float probster_length = probe_offset_length_list[current_length_index];
    float probster_change;
    switch (edit_mode) {
    case EDIT_DEC:
        probster_change = -probster_length;
        break;
    case EDIT_INC:
        probster_change = +probster_length;
        break;
    default:
        // TODO render error
        return;
    }
    const float target_probster_length = utility_limit_float(
            config_range_min(),  //
            current_probe_offset + probster_change,  //
            config_range_max()  //
            );
    if (target_probster_length == current_probe_offset) {
        return;  // reached range limit
    }
    perform_probster_gcode(probster_change);
    current_probe_offset = target_probster_length;
}

//
void setCurrentOffset(const float probe_offset) {
    current_probe_offset = probe_offset;
}

void menu_tinker_probe_offset(void) {

    KEY_VALUE key_num = KEY_IDLE;

    perform_command_query();

    menuDrawPage(&ProbeOffsetPage);

    render_probe_offset();

    while (utility_has_menu_func(menu_tinker_probe_offset)) {
        key_num = menuKeyGetValue();

        switch (key_num) {

        case KEY_ICON_0:  // Decrease
            perform_command_edit(EDIT_DEC);
            break;

        case KEY_ICON_1:  // TODO
            break;

        case KEY_ICON_2:  // Increase
            perform_command_edit(EDIT_INC);
            break;

        case KEY_ICON_3:  // TODO
            break;

        case KEY_ICON_4:  // Save to PROM
            perform_command_save();
            break;

        case KEY_ICON_5:  // Switch Delta
            perform_switch_unit(key_num);
            break;

        case KEY_ICON_6:  // Reset to zero
            perform_command_reset();
            break;

        case BUTTON_BACK:
            infoMenu.cur--;
            break;

        default:
            break;
        }

        render_probe_offset();

        loopProcess();

    }

}
