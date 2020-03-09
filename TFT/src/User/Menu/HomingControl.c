//
// homing support
//

#include <HomingControl.h>
#include "includes.h"

// dynamic homing menu
static MENUITEMS HomingPage =
        {
          // title
          LABEL_HOME,
          // icon           // label
          {
            { _ICON_EMPTY_, _LABEL_EMPTY_ },  // 0
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },

            { _ICON_EMPTY_, _LABEL_EMPTY_ },  // 4
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK }, }
        };

#define CONFIG_ENTRY_COUNT 7

// parsed homeing command configuration
static CONFIG_ENTRY config_entry_list[CONFIG_ENTRY_COUNT] =
        {
          { 0 },
        };

// homing entry config parser
#define PARSE_CONFIG(NUM) \
    config_entry_list[NUM-1].use = config_parse_bool(config->homing_control__entry_##NUM##_use); \
    config_entry_list[NUM-1].key = config_parse_int(config->homing_control__entry_##NUM##_key); \
    config_entry_list[NUM-1].icon = config_find_icon(config->homing_control__entry_##NUM##_icon); \
    config_entry_list[NUM-1].label = config->homing_control__entry_##NUM##_label; \
    config_entry_list[NUM-1].gcode = config->homing_control__entry_##NUM##_gcode; \
// PARSE_CONFIG

// load homing from config.ini
static void parse_homing_data(void) {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_CONFIG(1)
    PARSE_CONFIG(2)
    PARSE_CONFIG(3)
    PARSE_CONFIG(4)
    PARSE_CONFIG(5)
    PARSE_CONFIG(6)
    PARSE_CONFIG(7)
}

// default lables for label-code lookup
static uint32_t default_label_list[UTILITY_DEFAULT_LABEL_COUNT] =
        {
          0,  // invalid user entry
          LABEL_HOME_ALL,
          LABEL_X_HOME,
          LABEL_Y_HOME,
          LABEL_Z_HOME,
          LABEL_HOME,
          LABEL_HOME,
          LABEL_HOME,
          LABEL_BACK,
        };

#define BUTTON_PROTECT_COUNT 1

// list of button keys protected from user config
static KEY_VALUE button_protect_list[BUTTON_PROTECT_COUNT] =
        {
          BUTTON_BACK,
        };

static void render_homing_page(void) {
    utility_setup_config_menu(
            &HomingPage,
            default_label_list,
            CONFIG_ENTRY_COUNT,
            config_entry_list,
            BUTTON_PROTECT_COUNT,
            button_protect_list
            );
    menuDrawPage(&HomingPage);
}

static void perform_homing_command(const KEY_VALUE key_num) {
    for (int config_index = 0; config_index < CONFIG_ENTRY_COUNT; config_index++) {
        CONFIG_ENTRY *config_entry = &(config_entry_list[config_index]);
        if (config_entry->use && config_entry->key == key_num) {
            config_issue_gcode(config_entry->gcode);
        }
    }
}

void menuHoming(void) {
    KEY_VALUE key_num = KEY_IDLE;

    parse_homing_data();
    render_homing_page();

    while (utility_has_menu_func(menuHoming)) {
        key_num = menuKeyGetValue();

        switch (key_num) {

        case BUTTON_BACK:
            infoMenu.cur--;
            break;

        default:
            perform_homing_command(key_num);
            break;
        }

        loopProcess();
    }
}
