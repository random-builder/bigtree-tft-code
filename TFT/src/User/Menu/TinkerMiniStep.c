//
// runtime z offset adjustment
//

#include <TinkerMiniStep.h>
#include "includes.h"

//
static MENUITEMS MiniStepPage =
        {
          //title
          LABEL_MINI_STEP,
          //icon       // label
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

static const ITEM MiniStepEntryList[LENGTH_BLOCK_COUNT] =
        {
          // icon         // label
          { ICON_Mmm_001, LABEL_001_MM },
          { ICON_Mmm_01, LABEL_01_MM },
          { ICON_Mmm_1, LABEL_1_MM },
        };

static const float ministep_length_list[LENGTH_BLOCK_COUNT] =
        {
          0.01f,
          0.10f,
          1.00f,
        };

typedef enum {
    EDIT_NONE,
    EDIT_DEC,
    EDIT_INC,
} EDIT_MODE;

static int current_length_index = 1;

static float current_ministep_length = 0.0;

// ministep adjustment range
static float config_range_min(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config_parse_expr(config->tinker_ministep__range_min);
}

// ministep adjustment range
static float config_range_max(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config_parse_expr(config->tinker_ministep__range_max);
}

// ministep gcode for change
static char* config_command_edit(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config->tinker_ministep__command_edit;
}

// ministep gcode for eeprom persist
static char* config_command_save(void) {
    const SYSTEM_CONFIG *config = config_instance();
    return config->tinker_ministep__command_save;
}

//
static void render_ministep(void) {
    GUI_DispFloat(CENTER_X - 5 * BYTE_WIDTH / 2, CENTER_Y, current_ministep_length, 3, 2, RIGHT);
}

//
static void perform_switch_unit(const KEY_VALUE key_num) {
    current_length_index = (current_length_index + 1) % LENGTH_BLOCK_COUNT;
    MiniStepPage.items[key_num] = MiniStepEntryList[current_length_index];
    menuDrawItem(&MiniStepPage.items[key_num], key_num);
}

static void perform_ministep_gcode(const float ministep_change) {
    char command_text[64];
    my_sprintf(command_text, config_command_edit(), ministep_change);
    config_issue_gcode(command_text);
}

static void perform_command_reset(void) {
    const float ministep_change = -current_ministep_length;
    current_ministep_length = 0.0;
    perform_ministep_gcode(ministep_change);
}

//
static void perform_command_save(void) {
    config_issue_gcode(config_command_save());
}

//
static void perform_command_edit(const EDIT_MODE edit_mode) {
    const float ministep_length = ministep_length_list[current_length_index];
    float ministep_change;
    switch (edit_mode) {
    case EDIT_DEC:
        ministep_change = -ministep_length;
        break;
    case EDIT_INC:
        ministep_change = +ministep_length;
        break;
    default:
        // TODO render error
        return;
    }
    const float target_ministep_length = utility_limit_float(
            config_range_min(),  //
            current_ministep_length + ministep_change,  //
            config_range_max()  //
            );
    if (target_ministep_length == current_ministep_length) {
        return;  // reached range limit
    }
    perform_ministep_gcode(ministep_change);
    current_ministep_length = target_ministep_length;
}

void menu_tinker_mini_step(void) {

    KEY_VALUE key_num = KEY_IDLE;

//    perform_command_query();

    menuDrawPage(&MiniStepPage);

    render_ministep();

    while (utility_has_menu_func(menu_tinker_mini_step)) {
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

        render_ministep();

        loopProcess();

    }

}
