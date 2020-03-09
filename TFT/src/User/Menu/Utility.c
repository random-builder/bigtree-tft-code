//
// shared functions
//

#include "includes.h"

#include "Utility.h"

// show prompt at the bottom of the screen
void utility_prompt_touch_to_exit() {
    GUI_DispStringInRect(20, LCD_HEIGHT - (BYTE_HEIGHT * 2), LCD_WIDTH - 20, LCD_HEIGHT, language_text(LABEL_TOUCH_TO_EXIT));
    while (!isPress()) {
        loopProcess();
    }
    while (isPress()) {
        loopProcess();
    }
    infoMenu.cur--;
}

// verify if current menu function is active
bool utility_has_menu_func(void (*menu_func)(void)) {
    return infoMenu.menu[infoMenu.cur] == menu_func;
}

static bool has_key_in_list(  //
        const KEY_VALUE key_num,  //
        const int value_count,  //
        const KEY_VALUE value_list[]  //
        ) {
    for (int index = 0; index < value_count; index++) {
        if (key_num == value_list[index]) {
            return true;
        }
    }
    return false;
}

void utility_setup_config_menu(
        MENUITEMS *screen_page,
        uint32_t default_label_list[],
        int config_entry_count,
        CONFIG_ENTRY config_entry_list[],
        int button_protect_count,
        KEY_VALUE button_protect_list[]
        ) {
    for (int index = 0; index < config_entry_count; index++) {
        CONFIG_ENTRY *config_entry = &(config_entry_list[index]);
        if (has_key_in_list(config_entry->key, button_protect_count, button_protect_list)) {
            continue;  // protect
        }
        if (!config_entry->use) {
            continue;  // discard
        }
        ITEM *menu_item = &(screen_page->items[config_entry->key]);
        menu_item->icon = config_entry->icon;
        const int label_code = config_parse_int(config_entry->label);  // 0 for non-number
        if (label_code == 0) {
            // has text: custom user label from config.ini
            menu_item->label.address = (uint8_t*) config_entry->label;
        } else if (0 < label_code && label_code < UTILITY_DEFAULT_LABEL_COUNT) {
            // has number: lookup label index for the language
            const uint32_t label_index = default_label_list[label_code];
            menu_item->label.index = (uint32_t) label_index;
        } else {
            // FIXME: render error
        }
    }
}
