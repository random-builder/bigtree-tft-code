//
// shared functions
//

#include <UtilityFunc.h>
#include "includes.h"

void utility_await_button_touch() {
    while (!isPress()) {
        loopProcess();
    }
    while (isPress()) {
        loopProcess();
    }
}

// show prompt at the bottom of the screen
void utility_prompt_touch_to_exit() {
    GUI_DispStringInRect(20, LCD_HEIGHT - (BYTE_HEIGHT * 2), LCD_WIDTH - 20, LCD_HEIGHT, language_text(LABEL_TOUCH_TO_EXIT));
    utility_await_button_touch();
}

// verify if current menu function is active
bool utility_has_menu_func(void (*menu_func)(void)) {
    return infoMenu.menu[infoMenu.cur] == menu_func;
}

bool utility_has_matach(const char *source, const char *target) {
    return strcmp(source, target) == 0;
}

bool utility_has_substring(const char *string, const char *search) {
    return strstr(string, search) != NULL;
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

void utility_setup_label_from_data(
        LABEL *label,                // target label
        const char *label_data,      // literal text or label index in list
        const uint32_t label_list[]  // label lookup by label index from data
        ) {
    const int label_code = config_parse_int(label_data);  // 0 for non-number
    if (label_code == 0) {
        // has text: custom user label from config.ini
        label->address = (uint8_t*) label_data;
    } else if (0 < label_code && label_code < UTILITY_DEFAULT_LABEL_COUNT) {
        // has number: lookup default label index for the language
        const uint32_t label_index = label_list[label_code];
        label->index = (uint32_t) label_index;
    } else {
        // FIXME: render error
    }

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
        const CONFIG_ENTRY *config_entry = &(config_entry_list[index]);
        if (has_key_in_list(config_entry->key, button_protect_count, button_protect_list)) {
            continue;  // protect
        }
        if (!config_entry->use) {
            continue;  // discard
        }
        ITEM *menu_item = &(screen_page->items[config_entry->key]);
        menu_item->icon = config_entry->icon;
        utility_setup_label_from_data(&(menu_item->label), config_entry->label, default_label_list);
    }
}

// setup page buttons attached to the list view
void lister_setup_pager_icons(  //
        LISTITEMS *list_view_page,  //
        const LISTER_PAGE page_index,  //
        const LISTER_PAGE page_count  //
        ) {
    LISTITEM *button_page_up = &(list_view_page->items[BUTTON_PAGE_UP]);
    LISTITEM *button_page_down = &(list_view_page->items[BUTTON_PAGE_DOWN]);
    if (page_count <= 1) {
        button_page_up->icon = SYMBOL_BLANK;
        button_page_down->icon = SYMBOL_BLANK;
    } else {
        if (page_index <= 0) {
            button_page_up->icon = SYMBOL_BLANK;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        } else if (page_index >= (page_count - 1)) {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_BLANK;
        } else {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        }
    }
}

// process key press for a list view
void lister_process_navigation(  //
        const KEY_VALUE key_num,  //
        LISTER_PAGE *page_index,  //
        const LISTER_PAGE page_count,  //
        void (*react_button_page)(void),  //
        void (*react_button_default)(const KEY_VALUE)  //
        ) {

    switch (key_num) {

    case BUTTON_PAGE_UP:
        if (page_count > 1) {
            if ((*page_index) > 0) {
                (*page_index)--;
                react_button_page();
            }
        }
        break;

    case BUTTON_PAGE_DOWN:
        if (page_count > 1) {
            if ((*page_index) < page_count - 1) {
                (*page_index)++;
                react_button_page();
            }
        }
        break;

    case BUTTON_BACK:
        infoMenu.cur--;
        break;

    default:
        react_button_default(key_num);
        break;
    }

}

// resolve list view (page,index) into model index
int lister_index_from_view(
        LISTER_PAGE lister_page_now,
        LISTER_INDEX lister_index
        ) {
    return lister_page_now * LISTITEM_PER_PAGE + lister_index;
}

// build list view page from config entries
void lister_setup_config_page(
        LISTITEMS *lister_page,
        const LISTER_PAGE lister_page_now,
        const LISTER_PAGE lister_page_count,
        LISTER_ENTRY config_entry_list[],
        const int config_entry_count
        ) {
    for (LISTER_INDEX lister_index = 0; lister_index < LISTITEM_PER_PAGE; lister_index++) {
        const int entry_index = lister_index_from_view(lister_page_now, lister_index);
        const LISTER_ENTRY *config_entry = &(config_entry_list[entry_index]);
        LISTITEM *menu_item = &(lister_page->items[lister_index]);
        if (config_entry->use && entry_index < config_entry_count) {
            menu_item->icon = symbol_index_by_name(config_entry->icon);
            menu_item->titlelabel.index = _LABEL_DYNAMIC_;
            dynamic_label[lister_index] = config_entry->label;
        } else {
            menu_item->icon = _SYMBOL_EMPTY_;
            menu_item->titlelabel.index = _LABEL_EMPTY_;
            dynamic_label[lister_index] = "";
        }
    }
    lister_setup_pager_icons(
            lister_page,
            lister_page_now,
            lister_page_count
            );
}
