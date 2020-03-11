//
// system development support
//

#include <malloc.h>

#include "includes.h"

// report memory allocator status
// see https://linux.die.net/man/3/mallinfo
// see https://www.gnu.org/software/libc/manual/html_node/Statistics-of-Malloc.html
static void render_system_memory_report(void) {

    char text_line[64];
    struct mallinfo memory_info;

    memory_info = mallinfo();

    GUI_Clear(BACKGROUND_COLOR);

    GUI_DispString(0, BYTE_HEIGHT * 1, (u8*) "Memory Report (mallinfo)");

    my_sprintf(text_line, "Total size (arena)    : %d", memory_info.arena);
    GUI_DispString(0, BYTE_HEIGHT * 2, (u8*) text_line);

    my_sprintf(text_line, "Total used (uordblks) : %d", memory_info.uordblks);
    GUI_DispString(0, BYTE_HEIGHT * 3, (u8*) text_line);

    my_sprintf(text_line, "Total free (fordblks) : %d", memory_info.fordblks);
    GUI_DispString(0, BYTE_HEIGHT * 4, (u8*) text_line);

    utility_prompt_touch_to_exit();

    infoMenu.cur--;

}

//
//
//

// show memory status
static void show_memory_report(void) {
    infoMenu.menu[++infoMenu.cur] = render_system_memory_report;
}

#define DEBUG_ENTRY_COUNT 3

static LISTITEMS DebugPage =
        {
          // title
          LABEL_DEBUG,
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

#define DEBUG_PAGE_COUNT  (DEBUG_ENTRY_COUNT + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE

// remember last page
static LISTER_PAGE debug_page_now = 0;

// parsed custom menu configuration
static LISTER_ENTRY debug_entry_list[DEBUG_ENTRY_COUNT] =
        {
          { 0 }  // all zero, load from config.ini
        };

typedef void (*FUNC_VOID)(void);

typedef void (*FUNCTION_ARRAY[DEBUG_ENTRY_COUNT])(void);

// command func by index
static FUNCTION_ARRAY debug_function_list =
        {
          show_memory_report,
        };

// function name by index
static char *debug_command_list[DEBUG_ENTRY_COUNT] =
        {
          STRINGIFY(show_memory_report),
        };

// resolve function by its name
static FUNC_VOID debug_func_by_name(const char *command) {
    for (int index = 0; index < DEBUG_ENTRY_COUNT; index++) {
        if (strcmp(debug_command_list[index], command) == 0) {
            return debug_function_list[index];
        }
    }
    return NULL;
}

// index inside custom entry list
typedef int ENTRY_INDEX;

// custom entry config parser
#define PARSE_DEBUG(NUM) \
    debug_entry_list[NUM-1].use = config_parse_bool(config->debug_menu__entry_##NUM##_use); \
    debug_entry_list[NUM-1].icon = config->debug_menu__entry_##NUM##_icon; \
    debug_entry_list[NUM-1].label = config->debug_menu__entry_##NUM##_label; \
    debug_entry_list[NUM-1].command = config->debug_menu__entry_##NUM##_command; \
// PARSE_DEBUG

// extract custom menu entries from config.ini
static void parse_debug_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_DEBUG(1)
//    PARSE_DEBUG(2)
//    PARSE_DEBUG(3)
//    PARSE_DEBUG(4)
//    PARSE_DEBUG(5)
//    PARSE_DEBUG(6)
//    PARSE_DEBUG(7)
//    PARSE_DEBUG(8)
//    PARSE_DEBUG(9)
}

// rebuild menu page
static void render_debug_page(void) {

    utility_setup_lister_page(
            &DebugPage,
            &debug_page_now,
            DEBUG_PAGE_COUNT,
            debug_entry_list,
            DEBUG_ENTRY_COUNT
            );

    menuDrawListPage(&DebugPage);
}

// perform action on button press
static void perform_debug_command(const KEY_VALUE key_num) {
    const ENTRY_INDEX debug_index = utility_lister_index_from_view(debug_page_now, key_num);
    if (debug_index < DEBUG_ENTRY_COUNT) {
        const LISTER_ENTRY *debug_entry = &(debug_entry_list[debug_index]);
        if (debug_entry->use) {
            void (*function)(void) = debug_func_by_name(debug_entry->command);
            if (function != NULL) {
                function();
            }
        }
    }
}

void menu_system_debug() {
    KEY_VALUE key_num = KEY_IDLE;

    parse_debug_data();
    render_debug_page();

    while (utility_has_menu_func(menu_system_debug)) {

        key_num = menuKeyGetValue();

        lister_process_navigation(
                key_num,
                &debug_page_now,
                DEBUG_PAGE_COUNT,
                render_debug_page,
                perform_debug_command
                );

        loopProcess();
    }
}

