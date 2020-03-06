//
//
//

#include "MachineSettings.h"
#include "includes.h"

//
// custom commands
//

static LISTITEMS CustomCommandItems =
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

#define CUSTOM_PAGE_COUNT  (CUSTOM_ENTRY_COUNT + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE

// remember current page
static int custom_page_now = 0;

// a custom menu entry
typedef struct {
    bool use;       // value from config
    u16 key;        // value from config
    char *icon;     // pointer to config, must be icon name from symbol.inc
    char *label;    // pointer to config, can be any user string
    char *gcode;    // pointer to config, must be (multi) line of gcode
} CUSTOM_ENTRY;

// parsed custom menu configuration
static CUSTOM_ENTRY custom_entry_list[CUSTOM_ENTRY_COUNT] =
        {
          { 0 }  // all zero
        };

//perform action on button press
void perform_custom_command(int key_num) {
    int item_index = custom_page_now * LISTITEM_PER_PAGE + key_num;
    if (item_index < CUSTOM_ENTRY_COUNT) {
        CUSTOM_ENTRY custom_entry = custom_entry_list[item_index];
        if (custom_entry.use) {
            config_issue_gcode(custom_entry.gcode);
        }
    }
}

// custom entry config parser
#define PARSE_CUSTOM(NUM) \
    custom_entry_list[NUM].use = config_parse_bool(config->custom_menu__entry_##NUM##_use); \
    custom_entry_list[NUM].key = config_parse_int(config->custom_menu__entry_##NUM##_key); \
    custom_entry_list[NUM].icon = config->custom_menu__entry_##NUM##_icon; \
    custom_entry_list[NUM].label = config->custom_menu__entry_##NUM##_label; \
    custom_entry_list[NUM].gcode = config->custom_menu__entry_##NUM##_gcode; \
// PARSE_CUSTOM

// extract custom menu entries from config.ini
void parse_custom_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_CUSTOM(0)
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
}

// build custom menu from parsed config
void setup_custom_menu(void) {
    // setup dynamic label
    for (int line_index = 0; line_index < LISTITEM_PER_PAGE; line_index++) {
        int item_index = custom_page_now * LISTITEM_PER_PAGE + line_index;
        CUSTOM_ENTRY custom_entry = custom_entry_list[item_index];
        LISTITEM *menu_item = &(CustomCommandItems.items[line_index]);
        if (custom_entry.use && item_index < CUSTOM_ENTRY_COUNT) {
            menu_item->icon = symbol_index_by_name(custom_entry.icon);
            menu_item->titlelabel.index = _LABEL_DYNAMIC_;
            dynamic_label[line_index] = custom_entry.label;
        } else {
            menu_item->icon = _SYMBOL_EMPTY_;
            menu_item->titlelabel.index = _LABEL_EMPTY_;
            dynamic_label[line_index] = "";
        }
    }
    // setup page up/down button
    LISTITEM *button_page_up = &(CustomCommandItems.items[KEY_ICON_5]);
    LISTITEM *button_page_down = &(CustomCommandItems.items[KEY_ICON_6]);
    if (CUSTOM_ENTRY_COUNT <= LISTITEM_PER_PAGE) {
        button_page_up->icon = _SYMBOL_EMPTY_;
        button_page_down->icon = _SYMBOL_EMPTY_;
    } else {
        if (custom_page_now == 0) {
            button_page_up->icon = _SYMBOL_EMPTY_;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        }
        else if (custom_page_now == (CUSTOM_PAGE_COUNT - 1)) {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = _SYMBOL_EMPTY_;
        } else {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        }
    }
}

void menuCustomCommand(void) {
    KEY_VALUES key_num = KEY_IDLE;

    parse_custom_data();
    setup_custom_menu();
    menuDrawListPage(&CustomCommandItems);

    while (infoMenu.menu[infoMenu.cur] == menuCustomCommand) {

        key_num = menuKeyGetValue();

        switch (key_num) {
        case KEY_ICON_5:  // Page Up
            if (CUSTOM_PAGE_COUNT > 1) {
                if (custom_page_now > 0) {
                    custom_page_now--;
                    setup_custom_menu();
                    menuRefreshListPage();
                }
            }
            break;
        case KEY_ICON_6:  // Page Down
            if (CUSTOM_PAGE_COUNT > 1) {
                if (custom_page_now < CUSTOM_PAGE_COUNT - 1) {
                    custom_page_now++;
                    setup_custom_menu();
                    menuRefreshListPage();
                }
            }
            break;
        case KEY_ICON_7:  // Back
            infoMenu.cur--;
            break;
        default:
            perform_custom_command(key_num);
            break;
        }

        loopProcess();
    }
}

//
// light control
//

// dynamic light control menu
static MENUITEMS LightLevelItems =
        {
          // title
          LABEL_CASE_LIGHT,
          // icon, label
          {
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK },
          }
        };

#define LIGHT_LEVEL_COUNT 7

// light control menu entry
typedef struct {
    bool use;       // enable/disable, value from config.ini
    u16 key;        // menu item position, value from config.ini
    u16 icon;       // icon index from icon_list.inc resolved from icon name in config.ini
    char *label;    // label-code or user-text from config.ini
    char *gcode;    // pointer to config, must be (multi) line of gcode
} LIGHT_ENTRY;

// parsed light control menu configuration
static LIGHT_ENTRY light_entry_list[LIGHT_LEVEL_COUNT] =
        {
          { 0 }  // all zero
        };

// light control level config parser
#define PARSE_LIGHT(NUM) \
    light_entry_list[NUM-1].use = config_parse_bool(config->light_control__level_##NUM##_use); \
    light_entry_list[NUM-1].key = config_parse_int(config->light_control__level_##NUM##_key); \
    light_entry_list[NUM-1].icon = config_find_icon(config->light_control__level_##NUM##_icon); \
    light_entry_list[NUM-1].label = config->light_control__level_##NUM##_label; \
    light_entry_list[NUM-1].gcode = config->light_control__level_##NUM##_gcode; \
// PARSE_LIGHT

// extract light control menu entries from config.ini
void parse_light_data() {
    const SYSTEM_CONFIG *config = config_instance();
    PARSE_LIGHT(1)
    PARSE_LIGHT(2)
    PARSE_LIGHT(3)
    PARSE_LIGHT(4)
    PARSE_LIGHT(5)
    PARSE_LIGHT(6)
    PARSE_LIGHT(7)
}

// find language label-index by provided label-code
static const u16 default_light_label_list[LIGHT_LEVEL_COUNT + 1] =
        {
          0,  // invalid user entry
          LABEL_RED,
          LABEL_GREEN,
          LABEL_BLUE,
          LABEL_WHITE,
          LABEL_WHITE,
          LABEL_WHITE,
          LABEL_RGB_OFF,
        };

// build light control menu from parsed config
void setup_light_menu(void) {
    for (int index = 0; index < LIGHT_LEVEL_COUNT; index++) {
        LIGHT_ENTRY *light_entry = &(light_entry_list[index]);
        if (light_entry->key == KEY_ICON_7) {
            continue;  // protect "Back"
        }
        if (light_entry->use) {
            ITEM *menu_item = &(LightLevelItems.items[light_entry->key]);
            menu_item->icon = light_entry->icon;
            const int label_code = config_parse_int(light_entry->label);  // 0 for non-number
            if (label_code == 0) {
                // has text: custom user label from config.ini
                menu_item->label.address = (uint8_t*) light_entry->label;
            } else if (0 < label_code && label_code < LIGHT_LEVEL_COUNT + 1) {
                // has number: lookup label index for the language
                const uint32_t label_index = default_light_label_list[label_code];
                menu_item->label.index = (uint32_t) label_index;
            } else {
                // FIXME: render error
            }
        }
    }

}

// perform light level apply for enabled key
void perform_light_control(const KEY_VALUES key_num) {
    for (int index = 0; index < LIGHT_LEVEL_COUNT; index++) {
        LIGHT_ENTRY *light_entry = &(light_entry_list[index]);
        if (light_entry->use && light_entry->key == key_num) {
            config_issue_gcode(light_entry->gcode);
            return;  // use only first match
        }
    }
}

void menuLightControl(void) {
    KEY_VALUES key_num = KEY_IDLE;

    parse_light_data();
    setup_light_menu();
    menuDrawPage(&LightLevelItems);

    while (infoMenu.menu[infoMenu.cur] == menuLightControl) {
        key_num = menuKeyGetValue();
        switch (key_num) {
        case KEY_ICON_7:
            infoMenu.cur--;
            break;
        default:
            perform_light_control(key_num);
            break;
        }
        loopProcess();
    }
}

//
// top level machine settings menu
//

static MENUITEMS MachineSettingsItems =
        {
          // title
          LABEL_MACHINE_SETTINGS,
          // icon                       label
          {
            { ICON_Custom, LABEL_CUSTOM },
            { ICON_RGB_Settings, LABEL_RGB_SETTINGS },
            { ICON_Gcode, LABEL_GCODE },
            { ICON_ShutDown, LABEL_SHUT_DOWN },
            { ICON_parametersetting, LABEL_PARAMETER_SETTING },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK }, }
        };

void menuMachineSettings(void)
{
    KEY_VALUES key_num = KEY_IDLE;

    menuDrawPage(&MachineSettingsItems);

    while (infoMenu.menu[infoMenu.cur] == menuMachineSettings)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:
            infoMenu.menu[++infoMenu.cur] = menuCustomCommand;
            break;

        case KEY_ICON_1:
            infoMenu.menu[++infoMenu.cur] = menuLightControl;
            break;

        case KEY_ICON_2:
            infoMenu.menu[++infoMenu.cur] = menuInvokeGcode;
            break;

        case KEY_ICON_3:
            storeCmd("M81\n");
            break;

        case KEY_ICON_4:
            infoMenu.menu[++infoMenu.cur] = parametersetting;
            break;

        case KEY_ICON_7:
            infoMenu.cur--;
            break;

        default:
            break;
        }

        loopProcess();
    }
}
