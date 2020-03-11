//
// EEPROM settings editor
//

#include "FeatureSettings.h"
#include "includes.h"

// empty feature item
static const LISTITEM FeatureEntryEmpty =
        { SYMBOL_BLANK, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_ };

// feature menu page
static LISTITEMS FeatureSettingsPage =
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
    OPTION_INVERT_X,
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
} FEATURE_ENUM;

// position inside feature enum
typedef int FEATURE_INDEX;

#define FEATURE_PAGE_COUNT  ( FEATURE_COUNT + LISTITEM_PER_PAGE - 1 ) / LISTITEM_PER_PAGE

// remember last page
static LISTER_PAGE feature_page_now = 0;

// feature option model/view
static LISTITEM FeatureEntryList[FEATURE_COUNT] =
        {
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_TERMINAL_ACK, _LABEL_EMPTY_ },
          { SYMBOL_TOGGLE_ON, LIST_TOGGLE, LABEL_INVERT_XAXIS, _LABEL_EMPTY_ },
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

// feature index safety guard
static bool has_valid_feature(const FEATURE_INDEX feature_index) {
    return 0 <= feature_index && feature_index < FEATURE_COUNT;
}

// resolve list view page+index into model index
static FEATURE_INDEX feature_index_from_view(const LISTER_INDEX entry_index) {
    return feature_page_now * LISTITEM_PER_PAGE + entry_index;
}

// copy model state into view with optional model update
static void setup_feature_entry(const FEATURE_INDEX feature_index, const bool with_update) {

    if (!has_valid_feature(feature_index)) {
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

    case OPTION_INVERT_X:
        if (with_update) {
            infoSettings.invert_xaxis = (infoSettings.invert_xaxis + 1) % TOGGLE_ENTRY_COUNT;
        }
        feature_entry->icon = toggle_entry_symbol[infoSettings.invert_xaxis];
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
static void setup_feature_page(void) {
    for (LISTER_INDEX entry_index = 0; entry_index < LISTITEM_PER_PAGE; entry_index++) {
        const FEATURE_INDEX feature_index = feature_index_from_view(entry_index);
        LISTITEM *feature_entry = &(FeatureSettingsPage.items[entry_index]);
        if (has_valid_feature(feature_index)) {
            setup_feature_entry(feature_index, false);  // without update
            *feature_entry = FeatureEntryList[feature_index];  // copy
        } else {
            *feature_entry = FeatureEntryEmpty;  // copy
        }
    }
    lister_setup_pager_icons(
            &FeatureSettingsPage,
            feature_page_now,
            FEATURE_PAGE_COUNT
            );
}

// rebuild feature menu
static void render_feature_page(void) {
    setup_feature_page();
    menuDrawListPage(&FeatureSettingsPage);
}

// action on button press: update and render option value
static void perform_feature_change(const KEY_VALUE key_num) {
    const FEATURE_INDEX feature_index = feature_index_from_view(key_num);
    if (!has_valid_feature(feature_index)) {
        return;  // ignore empty
    }
    setup_feature_entry(feature_index, true);  // with update
    FeatureSettingsPage.items[key_num] = FeatureEntryList[feature_index];  // copy
    menuDrawListItem(&FeatureSettingsPage.items[key_num], key_num);
}

void menuFeatureSettings(void) {

    KEY_VALUE key_num = KEY_IDLE;
    SETTINGS settings = infoSettings;  // copy

    render_feature_page();

    while (utility_has_menu_func(menuFeatureSettings)) {

        key_num = menuKeyGetValue();

        lister_process_navigation(
                key_num,
                &feature_page_now,
                FEATURE_PAGE_COUNT,
                render_feature_page,
                perform_feature_change
                );

        loopProcess();

    }

    if (memcmp(&settings, &infoSettings, sizeof(SETTINGS))) {
        storePara();
    }

}
