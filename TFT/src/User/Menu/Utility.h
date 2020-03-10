//
// shared functions
//

#pragma once

void utility_prompt_touch_to_exit();

bool utility_has_menu_func(void (*menu_func)(void));

typedef struct {
    bool use;       // enable menu icon
    u16 key;        // re-map icon key position
    u16 icon;      // icon index from icon_list.inc resolved from icon name in config.ini
    char *label;    // label-code or user-text from config.ini
    char *gcode;    // command gcode multi-liner
} CONFIG_ENTRY;

#define UTILITY_DEFAULT_LABEL_COUNT 9

void utility_setup_config_menu(
        MENUITEMS *screen_page,
        uint32_t default_label_list[],
        int config_entry_count,
        CONFIG_ENTRY config_entry_list[],
        int button_protect_count,
        KEY_VALUE button_protect_list[]
        );

bool utility_has_matach(const char *source, const char *target);

bool utility_has_substring(const char *string, const char *search);
