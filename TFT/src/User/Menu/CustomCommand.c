//
//
//

#include <CustomCommand.h>
#include "includes.h"

//
static LISTITEMS CustomCommandPage =
        {
          // title
          LABEL_CUSTOM,
          //  icon            ItemType     Item Title    item value text
          {
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEUP, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEDOWN, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BACK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
          }
        };

//
#define CUSTOM_ENTRY_COUNT 20

//
#define CUSTOM_PAGE_COUNT  (CUSTOM_ENTRY_COUNT + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE

// remember last page
static LISTER_PAGE custom_page_now = 0;

// parsed custom menu configuration
static LISTER_ENTRY custom_entry_list[CUSTOM_ENTRY_COUNT] =
        {
          { 0 }  // all zero
        };

// index inside custom entry list
typedef int CUSTOM_INDEX;

// custom entry config parser
#define PARSE_CUSTOM(NUM) \
    custom_entry_list[NUM-1].use = config_parse_bool(config->custom_command__entry_##NUM##_use); \
    custom_entry_list[NUM-1].icon = config->custom_command__entry_##NUM##_icon; \
    custom_entry_list[NUM-1].label = config->custom_command__entry_##NUM##_label; \
    custom_entry_list[NUM-1].gcode = config->custom_command__entry_##NUM##_gcode; \
// PARSE_CUSTOM

// extract custom menu entries from config.ini
static void parse_custom_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_CUSTOM(1)
    PARSE_CUSTOM(2)
    PARSE_CUSTOM(3)
    PARSE_CUSTOM(4)
    PARSE_CUSTOM(5)
    PARSE_CUSTOM(6)
    PARSE_CUSTOM(7)
    PARSE_CUSTOM(8)
    PARSE_CUSTOM(9)
    PARSE_CUSTOM(10)
    PARSE_CUSTOM(11)
    PARSE_CUSTOM(12)
    PARSE_CUSTOM(13)
    PARSE_CUSTOM(14)
    PARSE_CUSTOM(15)
    PARSE_CUSTOM(16)
    PARSE_CUSTOM(17)
    PARSE_CUSTOM(18)
    PARSE_CUSTOM(19)
    PARSE_CUSTOM(20)
}

// resolve list view page+index into model index
static CUSTOM_INDEX custom_index_from_view(const LISTER_INDEX entry_index) {
    return custom_page_now * LISTITEM_PER_PAGE + entry_index;
}

// rebuild menu page
static void render_custom_page(void) {
    lister_setup_config_page(
            &CustomCommandPage,
            custom_page_now,
            CUSTOM_PAGE_COUNT,
            custom_entry_list,
            CUSTOM_ENTRY_COUNT
            );
    menuDrawListPage(&CustomCommandPage);
}

// perform action on button press
static void perform_custom_command(const KEY_VALUE key_num) {
    const CUSTOM_INDEX custom_index = custom_index_from_view(key_num);
    if (custom_index < CUSTOM_ENTRY_COUNT) {
        LISTER_ENTRY *custom_entry = &(custom_entry_list[custom_index]);
        if (custom_entry->use) {
            config_issue_gcode(custom_entry->gcode);
        }
    }
}

void menuCustomCommand(void) {
    KEY_VALUE key_num = KEY_IDLE;

    parse_custom_data();
    render_custom_page();

    while (utility_has_menu_func(menuCustomCommand)) {

        key_num = menuKeyGetValue();

        lister_process_navigation(
                key_num,
                &custom_page_now,
                CUSTOM_PAGE_COUNT,
                render_custom_page,
                perform_custom_command
                );

        loopProcess();
    }
}
