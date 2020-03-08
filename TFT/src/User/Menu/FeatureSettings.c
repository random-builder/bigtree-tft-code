//
// EEPROM settings editor
//

#include "FeatureSettings.h"
#include "includes.h"

// empty feature item
static const LISTITEM FeatureEntryEmpty =
        { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ };

// feature menu page
static LISTITEMS FeatureSettingsItems =
        {
          // title
          LABEL_FEATURE_SETTINGS,
          // icon | item type  | item title | item value text (only for custom value)
          {
            { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEUP, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_PAGEDOWN, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
            { SYMBOL_BACK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ },
          }
        };

#define TOGGLE_ENTRY_COUNT  2
static const uint16_t toggle_entry_symbol[TOGGLE_ENTRY_COUNT] =
        {
          SYMBOL_TOGGLE_OFF,
          SYMBOL_TOGGLE_ON,
        };

#ifdef FIL_RUNOUT_PIN
#define RUNOUT_MODE_COUNT   3
static const LABEL itemRunout[RUNOUT_MODE_COUNT] =
        {
          //item value text (only for custom value)
          LABEL_OFF,
          LABEL_ON,
          LABEL_SMART,
        };

#endif

#define SPEED_RATE_COUNT    3
static const LABEL speed_rate_label[SPEED_RATE_COUNT] =
        {
          //item value text (only for custom value)
          LABEL_NORMAL_SPEED,
          LABEL_SLOW_SPEED,
          LABEL_FAST_SPEED,
        };

// feature option index
typedef enum {
    OPTION_HIDE_ACK = 0,
    OPTION_INVERT_Y,
    OPTION_INVERT_Z,
    //
#ifdef PS_ON_PIN
    OPTION_POWER,
#endif
    //
#ifdef FIL_RUNOUT_PIN
    OPTION_RUNOUT,
#endif
    OPTION_SPEED_MOVE,
    OPTION_GCODE_ON_START,
    OPTION_GCODE_ON_FINSIH,
    OPTION_PERSISTENT_INFO,
    OPTION_FILE_LIST_MODE,
    //
#ifdef LED_color_PIN
    OPTION_KNOB,
#endif
    // keep tail
    FEATURE_COUNT
} FEATURE_INDEX;

#define FEATURE_PAGE_COUNT  ( FEATURE_COUNT + LISTITEM_PER_PAGE - 1 ) / LISTITEM_PER_PAGE

// remember last page
static int feature_page_now = 0;

// feature option model/view
static LISTITEM FeatureEntryList[FEATURE_COUNT] =
        {
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_TERMINAL_ACK, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_INVERT_YAXIS, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_INVERT_ZAXIS, _LABEL_EMPTY_ },
          //
#ifdef PS_ON_PIN
          {SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_AUTO_SHUT_DOWN, _LABEL_EMPTY_},
#endif
          //
#ifdef FIL_RUNOUT_PIN
          {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_FILAMENT_RUNOUT, LABEL_OFF },
#endif
          { SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_MOVE_SPEED, LABEL_NORMAL_SPEED },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_SEND_START_GCODE, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_SEND_END_GCODE, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_PERSISTENT_STATUS_INFO, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_FILE_LISTMODE, _LABEL_EMPTY_ },
        //
#ifdef LED_color_PIN
          {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_OFF },
#endif
        };

// copy model state into view with optional model update
static void setup_feature_entry(const FEATURE_INDEX feature_index, const bool with_update) {

    if (feature_index < 0 || feature_index >= FEATURE_COUNT) {
        return;  // failure
    }

    LISTITEM *feature_entry = &(FeatureEntryList[feature_index]);  // edit

    switch (feature_index) {

    case OPTION_HIDE_ACK:
        if (with_update) {
            infoSettings.terminalACK = (infoSettings.terminalACK + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.terminalACK];
        break;

    case OPTION_INVERT_Y:
        if (with_update) {
            infoSettings.invert_yaxis = (infoSettings.invert_yaxis + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.invert_yaxis];
        break;

    case OPTION_INVERT_Z:
        if (with_update) {
            infoSettings.invert_zaxis = (infoSettings.invert_zaxis + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.invert_zaxis];
        break;

#ifdef PS_ON_PIN
    case OPTION_POWER:
        if (with_update) {
            infoSettings.auto_off = (infoSettings.auto_off + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.auto_off];
        break;
#endif

#ifdef FIL_RUNOUT_PIN
    case OPTION_RUNOUT:
        if (with_update) {
            infoSettings.runout = (infoSettings.runout + 1) % RUNOUT_MODE_COUNT;
        }
        feature_entry->valueLabel = itemRunout[infoSettings.runout];
        break;
#endif

    case OPTION_SPEED_MOVE:
        if (with_update) {
            infoSettings.move_speed = (infoSettings.move_speed + 1) % SPEED_RATE_COUNT;
        }
        feature_entry->valueLabel = speed_rate_label[infoSettings.move_speed];
        break;

    case OPTION_GCODE_ON_START:
        if (with_update) {
            infoSettings.send_start_gcode = (infoSettings.send_start_gcode + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.send_start_gcode];
        break;

    case OPTION_GCODE_ON_FINSIH:
        if (with_update) {
            infoSettings.send_end_gcode = (infoSettings.send_end_gcode + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.send_end_gcode];
        break;

    case OPTION_PERSISTENT_INFO:
        if (with_update) {
            infoSettings.persistent_info = (infoSettings.persistent_info + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.persistent_info];
        break;

    case OPTION_FILE_LIST_MODE:
        if (with_update) {
            infoSettings.file_listmode = (infoSettings.file_listmode + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.file_listmode];
        break;

#ifdef LED_color_PIN
    case OPTION_KNOB:
        if (with_update) {
            infoSettings.knob_led_color = (infoSettings.knob_led_color + 1 ) % LED_color_NUM;
            ws2812_send_DAT(led_color[infoSettings.knob_led_color]);
        }
        feature_entry->valueLabel = itemLedcolor[infoSettings.knob_led_color];
        break;
#endif

    default:
        break;
    }

}

// load values on page change and reload
static void setup_feature_screen() {

    for (uint8_t entry_index = 0; entry_index < LISTITEM_PER_PAGE; entry_index++) {
        const FEATURE_INDEX feature_index = feature_page_now * LISTITEM_PER_PAGE + entry_index;
        if (feature_index >= FEATURE_COUNT) {
            FeatureSettingsItems.items[entry_index] = FeatureEntryEmpty;  // copy
        } else {
            setup_feature_entry(feature_index, false);  // no update
            FeatureSettingsItems.items[entry_index] = FeatureEntryList[feature_index];  // copy
        }
    }

    LISTITEM *button_page_up = &(FeatureSettingsItems.items[KEY_ICON_5]);
    LISTITEM *button_page_down = &(FeatureSettingsItems.items[KEY_ICON_6]);
    if (FEATURE_COUNT <= LISTITEM_PER_PAGE) {
        button_page_up->icon = SYMBOL_BLANK;
        button_page_down->icon = SYMBOL_BLANK;
    } else {
        if (feature_page_now <= 0) {
            button_page_up->icon = SYMBOL_BLANK;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        } else if (feature_page_now >= (FEATURE_PAGE_COUNT - 1)) {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_BLANK;
        } else {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        }
    }

}

// action on button press: update and render option value
static void perform_feature_update(const uint8_t key_num) {

    const FEATURE_INDEX feature_index = feature_page_now * LISTITEM_PER_PAGE + key_num;

    if (feature_index < 0 || feature_index >= FEATURE_COUNT) {
        return;  // ignore empty
    }

    setup_feature_entry(feature_index, true);  // with update
    FeatureSettingsItems.items[key_num] = FeatureEntryList[feature_index];  // copy
    menuDrawListItem(&FeatureSettingsItems.items[key_num], key_num);

}

static void render_feature_screen(void) {
    setup_feature_screen();
    menuDrawListPage(&FeatureSettingsItems);
}

void menuFeatureSettings(void) {

    KEY_VALUE key_num = KEY_IDLE;
    SETTINGS settings = infoSettings;  // copy

    render_feature_screen();

    while (infoMenu.menu[infoMenu.cur] == menuFeatureSettings) {

        key_num = menuKeyGetValue();

        switch (key_num) {

        case KEY_ICON_5:  // Page Up
            if (FEATURE_PAGE_COUNT > 1) {
                if (feature_page_now > 0) {
                    feature_page_now--;
                    render_feature_screen();
                }
            }
            break;

        case KEY_ICON_6:  // Page Down
            if (FEATURE_PAGE_COUNT > 1) {
                if (feature_page_now < FEATURE_PAGE_COUNT - 1) {
                    feature_page_now++;
                    render_feature_screen();
                }
            }
            break;

        case KEY_ICON_7:  // Back
            infoMenu.cur--;
            break;

        default:  // Entry change
            perform_feature_update(key_num);
            break;
        }

        loopProcess();

    }

    if (memcmp(&settings, &infoSettings, sizeof(SETTINGS))) {
        storePara();
    }

}
