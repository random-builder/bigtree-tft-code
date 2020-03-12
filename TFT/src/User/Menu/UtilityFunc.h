//
// shared functions
//

#pragma once

#include <stdbool.h>

#include "AnyMenu.h"
#include "variants.h"

bool utility_has_menu_func(void (*menu_func)(void));

bool utility_has_matach(const char *source, const char *target);

bool utility_has_substring(const char *string, const char *search);

void utility_prompt_touch_to_exit();

void utility_await_button_touch();

float utility_limit_float(float min, float value, float max);

typedef struct {
    bool use;       // enable menu icon
    u16 key;        // re-map icon key position
    u16 icon;       // icon index from icon_list.inc resolved from icon name in config.ini
    char *label;    // label-code or user-text from config.ini
    char *gcode;    // command gcode multi-liner
} CONFIG_ENTRY;

typedef struct {
    bool use;       // enable menu icon
    char *icon;     // icon name
    char *label;    // label-code or user-text from config.ini
    char *gcode;
} LISTER_ENTRY;

#define UTILITY_DEFAULT_LABEL_COUNT 9

void utility_setup_config_menu(
        MENUITEMS *screen_page,
        uint32_t default_label_list[],
        int config_entry_count,
        CONFIG_ENTRY config_entry_list[],
        int button_protect_count,
        KEY_VALUE button_protect_list[]
        );

// list view page index and count
typedef int LISTER_PAGE;

// list view entry index inside the page
typedef int LISTER_INDEX;

// setup page buttons attached to the list view
void lister_setup_pager_icons(  //
        LISTITEMS *list_view_page,  //
        const LISTER_PAGE page_index,  //
        const LISTER_PAGE page_count  //
        );

// process key press for a list view
void lister_process_navigation(  //
        const KEY_VALUE key_num,  //
        LISTER_PAGE *page_index,  //
        const LISTER_PAGE page_count,  //
        void (*react_button_page)(void),  //
        void (*react_button_default)(const KEY_VALUE)  //
                );

// resolve list view (page,index) into model index
int lister_index_from_view(
        LISTER_PAGE lister_page_now,
        LISTER_INDEX lister_index
        );

// build list view page from config entries
void lister_setup_config_page(
        LISTITEMS *lister_page,
        LISTER_PAGE lister_page_now,
        LISTER_PAGE lister_page_count,
        LISTER_ENTRY config_entry_list[],
        int config_entry_count
        );
