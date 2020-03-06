//
//
//

#include "MachineSettings.h"
#include "includes.h"

LISTITEMS customItems =
        {
          // title
          LABEL_CUSTOM,
          //  icon                 ItemType      Item Title        item value text
          {
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { _SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEUP, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEDOWN, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BACK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ }, }
        };

#define CUSTOM_PAGE_COUNT  (CUSTOM_ENTRY_COUNT + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE

// remember current page
int custom_page_now = 0;

// a custom menu entry
typedef struct {
    bool use;       // value from config
    u16 key;        // value from config
    char *icon;     // pointer to config, must be icon name from symbol.inc
    char *label;    // pointer to config, can be any user string
    char *gcode;    // pointer to config, must be (multi) line of gcode
} CUSTOM_ENTRY;

// parsed custom menu configuration
CUSTOM_ENTRY custom_entry_list[CUSTOM_ENTRY_COUNT] =
        {
          { 0 }  // all zero
        };

//
//perform action on button press
//
void sendCustomGcode(int key_num) {
    int item_index = custom_page_now * LISTITEM_PER_PAGE + key_num;
    if (item_index < CUSTOM_ENTRY_COUNT) {
        CUSTOM_ENTRY custom_entry = custom_entry_list[item_index];
        if (custom_entry.use) {
            config_issue_gcode(custom_entry.gcode);
        }
    }
}

// custom entry config parser
#define ENTRY_PARSE(NUM) \
    custom_entry_list[NUM].use = config_parse_bool(config->custom_menu__entry_##NUM##_use); \
    custom_entry_list[NUM].key = config_parse_int(config->custom_menu__entry_##NUM##_key); \
    custom_entry_list[NUM].icon = config->custom_menu__entry_##NUM##_icon; \
    custom_entry_list[NUM].label = config->custom_menu__entry_##NUM##_label; \
    custom_entry_list[NUM].gcode = config->custom_menu__entry_##NUM##_gcode; \
// ENTRY_PARSE

// extract custom menu entries from config.ini
void parse_custom_data() {
    const SYSTEM_CONFIG *config = config_instance();
    ENTRY_PARSE(0)
    ENTRY_PARSE(1)
    ENTRY_PARSE(2)
    ENTRY_PARSE(3)
    ENTRY_PARSE(4)
    ENTRY_PARSE(5)
    ENTRY_PARSE(6)
    ENTRY_PARSE(7)
    ENTRY_PARSE(8)
    ENTRY_PARSE(9)
    ENTRY_PARSE(10)
    ENTRY_PARSE(11)
    ENTRY_PARSE(12)
    ENTRY_PARSE(13)
    ENTRY_PARSE(14)
    ENTRY_PARSE(15)
    ENTRY_PARSE(16)
    ENTRY_PARSE(17)
    ENTRY_PARSE(18)
    ENTRY_PARSE(19)
}

//
// build custom menu from parsed config
//
void setup_custom_menu(void) {
    // setup dynamic label
    for (int line_index = 0; line_index < LISTITEM_PER_PAGE; line_index++) {
        int item_index = custom_page_now * LISTITEM_PER_PAGE + line_index;
        CUSTOM_ENTRY custom_entry = custom_entry_list[item_index];
        LISTITEM *menu_item = &(customItems.items[line_index]);
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
    LISTITEM *button_page_up = &(customItems.items[KEY_ICON_5]);
    LISTITEM *button_page_down = &(customItems.items[KEY_ICON_6]);
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

void menuCustom(void) {
    KEY_VALUES key_num = KEY_IDLE;

    parse_custom_data();
    setup_custom_menu();
    menuDrawListPage(&customItems);

    while (infoMenu.menu[infoMenu.cur] == menuCustom) {

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
            sendCustomGcode(key_num);
            break;
        }

        loopProcess();
    }
}

MENUITEMS RGBItems =
        {
          // title
          LABEL_RGB_SETTINGS,
          // icon, label
          {
            { ICON_RGB_Red, LABEL_RED },
            { ICON_RGB_Green, LABEL_GREEN },
            { ICON_RGB_Blue, LABEL_BLUE },
            { ICON_RGB_White, LABEL_WHITE },
            { ICON_RGB_Off, LABEL_OFF },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Back, LABEL_BACK }, }
        };

void menuRGBSettings(void)
{
    KEY_VALUES key_num = KEY_IDLE;

    menuDrawPage(&RGBItems);

    while (infoMenu.menu[infoMenu.cur] == menuRGBSettings)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:  // Red
            storeCmd("M150 R255 U0 B0 P255\n");
            break;

        case KEY_ICON_1:  // Green
            storeCmd("M150 R0 U255 B0 P255\n");
            break;

        case KEY_ICON_2:  // Blue
            storeCmd("M150 R0 U0 B255 P255\n");
            break;

        case KEY_ICON_3:  // White
            storeCmd("M150 R255 U255 B255 P255\n");
            break;

        case KEY_ICON_4:  // Turn Off
            storeCmd("M150 R0 U0 B0 P0\n");
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

MENUITEMS machineSettingsItems =
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

    menuDrawPage(&machineSettingsItems);

    while (infoMenu.menu[infoMenu.cur] == menuMachineSettings)
    {
        key_num = menuKeyGetValue();
        switch (key_num)
        {
        case KEY_ICON_0:
            infoMenu.menu[++infoMenu.cur] = menuCustom;
            break;

        case KEY_ICON_1:
            infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
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
