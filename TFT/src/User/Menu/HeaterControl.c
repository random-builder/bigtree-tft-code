//
//
//

#include <HeaterControl.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items (icon + label) 
static MENUITEMS HeaterItems =
        {
          // title
          LABEL_HEAT_CTL,
          // icon       label
          {
            { ICON_Dec, LABEL_DEC },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Inc, LABEL_INC },
            { _ICON_EMPTY_, _LABEL_EMPTY_ },
            { ICON_Nozzle, LABEL_NOZZLE },
            { ICON_Temp_5, LABEL_5_DEGREE },
            { ICON_CoolDown, LABEL_ISSUE_RESET },
            { ICON_Back, LABEL_BACK }, }
        };

// number of temperature steps
#define TEMP_STEP_COUNT 3

// menu button for temperature step change
static const ITEM temp_step_item_list[TEMP_STEP_COUNT] =
        {
          // icon        label
          { ICON_Temp_1, LABEL_1_DEGREE },
          { ICON_Temp_5, LABEL_5_DEGREE },
          { ICON_Temp_10, LABEL_10_DEGREE },
        };

// size of temperature change step
static const u8 temp_step_size_list[TEMP_STEP_COUNT] =
        {
          1,
          5,
          10,
        };

// remember selected temperature change step
static u8 current_temp_step_index = 1;

// ### extern start

// FIXME replace and remove this

u16 heat_max_temp[HEATER_COUNT] =  // HEAT_MAX_TEMP;
        { 0 };  // all zero, loaded from config.ini

char *toolID[HEATER_COUNT] =  // HEAT_SIGN_ID;
        { 0 };  // all zero, loaded from config.ini

char *heatDisplayID[HEATER_COUNT] =  // HEAT_DISPLAY_ID;
        { 0 };  // all zero, loaded from config.ini

char *heatCmd[HEATER_COUNT] =  // HEAT_CMD;
        { 0 };  // all zero, loaded from config.ini

char *heatWaitCmd[HEATER_COUNT] =  // HEAT_WAIT_CMD;
        { 0 };  // all zero, loaded from config.ini

// ### extern finish

static HEATER heater_active =
        {
          { 0 },  // zero temps
          TOOL_NOZZLE0,  // tool
          TOOL_NOZZLE0,  // nozzle
        };

static HEATER heater_shadow =
        {
          { 0 },  // zero temps
          TOOL_NOZZLE0,  // tool
          TOOL_NOZZLE0,  // nozzle
        };

static u32 state_update_interval = 100;

static bool state_has_update_waiting = false;

static bool state_has_command_pending[HEATER_COUNT] =
        { false };

//
//
//

// heater config descriptor
typedef struct {
    bool use;          // enable this tool
    //u8 key;          // key for index re-mapping
    u16 icon;          // tool icon index for display
    char *gcid;        // gcode-id - used by gcode parser, not user-visible
    char *nick;        // short tool name shown on top level smart/dynamic icon
    char *label;       // long tool name shown on regular icon and with edit mode
    u16 temp_min;      // minimum allowed tool temperature
    u16 temp_max;      // maximum allowed tool temperature
    char *gcode_init;  // command prefix to set-and-continue temperature
    char *gcode_wait;  // command prefix to set-and-await-ready temperature
} HEATER_ENTRY;

// parsed heater configuration
static HEATER_ENTRY heater_entry_list[HEATER_COUNT] =
        {
          { 0 },
        };

// heater entry config parser
#define HEATER_PARSE(NUM) \
    heater_entry_list[NUM].use = config_parse_bool(config->heater_control__tool_##NUM##_use); \
    heater_entry_list[NUM].icon = config_find_icon(config->heater_control__tool_##NUM##_icon); \
    heater_entry_list[NUM].gcid = config->heater_control__tool_##NUM##_gcid; \
    heater_entry_list[NUM].nick = config->heater_control__tool_##NUM##_nick; \
    heater_entry_list[NUM].label = config->heater_control__tool_##NUM##_label; \
    heater_entry_list[NUM].temp_min = (int) config_parse_expr(config->heater_control__tool_##NUM##_temp_min); \
    heater_entry_list[NUM].temp_max = (int) config_parse_expr(config->heater_control__tool_##NUM##_temp_max); \
    heater_entry_list[NUM].gcode_init = config->heater_control__tool_##NUM##_gcode_init; \
    heater_entry_list[NUM].gcode_wait = config->heater_control__tool_##NUM##_gcode_wait; \
// HEATER_PARSE

// extract heater settings from config.ini
static void parse_heater_data(void) {
    const SYSTEM_CONFIG *config = config_instance();
    // hotbed
    HEATER_PARSE(0)
    // nozzle 0 ... 5
    HEATER_PARSE(1)
    HEATER_PARSE(2)
    HEATER_PARSE(3)
    HEATER_PARSE(4)
    HEATER_PARSE(5)
    HEATER_PARSE(6)
}

// report if tool is enabled in config.ini
bool heater_has_use(TOOL tool) {
    return heater_entry_list[tool].use;
}

// select next enabled tool via round robin
TOOL heater_next_tool(const TOOL this_tool) {
    const u8 base_tool = this_tool + 1;
    for (u8 index = base_tool; index < base_tool + HEATER_COUNT; index++) {
        TOOL next_tool = index % HEATER_COUNT;
        if (heater_has_use(next_tool)) {
            return next_tool;
        }
    }
    return this_tool;
}

// select next enabled nozzle via round robin (ignore hotbed)
TOOL heater_next_nozzle(const TOOL this_tool) {
    const u8 base_tool = this_tool + 1;
    for (u8 index = base_tool; index < base_tool + HEATER_COUNT; index++) {
        TOOL next_tool = index % HEATER_COUNT;
        if (next_tool == TOOL_HOTBED) {
            continue;  // ignore hotbed
        }
        if (heater_has_use(next_tool)) {
            return next_tool;
        }
    }
    return this_tool;
}

// populate from config arrays-to-be-removed
static void setup_legacy_data(void) {
    for (TOOL tool = 0; tool < HEATER_COUNT; tool++) {
        const HEATER_ENTRY *heater_entry = &(heater_entry_list[tool]);
        heat_max_temp[tool] = heater_entry->temp_max;
        toolID[tool] = heater_entry->gcid;
        heatDisplayID[tool] = heater_entry->nick;
        heatCmd[tool] = heater_entry->gcode_init;
        heatWaitCmd[tool] = heater_entry->gcode_wait;
    }
}

// customize tool menu item icon and label
static void setup_heater_icon(TOOL tool, KEY_VALUE key_num) {
    HEATER_ENTRY *heater_entry = &(heater_entry_list[tool]);
    ITEM *menu_item = &(HeaterItems.items[key_num]);
    menu_item->icon = heater_entry->icon;
    menu_item->label.address = (uint8_t*) heater_entry->label;
}

// setup config expected by other modules
void heater_ensure_config(void) {
    parse_heater_data();
    setup_legacy_data();
    setup_heater_icon(heater_active.tool, KEY_ICON_4);
}

//
//
//

/*Set target temperature*/
void heatSetTargetTemp(TOOL tool, u16 temp) {
    heater_active.T[tool].target = temp;
}

/*Sync target temperature*/
void heatSyncTargetTemp(TOOL tool, u16 temp) {
    heater_active.T[tool].target = temp;
    heater_shadow.T[tool].target = temp;
}

/*Get target temperature */
u16 heatGetTargetTemp(TOOL tool) {
    return heater_active.T[tool].target;
}

/* Set current temperature */
void heatSetCurrentTemp(TOOL tool, s16 temp) {
    heater_active.T[tool].actual = limitValue(-99, temp, 999);
}

/* Get current temperature */
s16 heatGetCurrentTemp(TOOL tool) {
    return heater_active.T[tool].actual;
}

/* Is heating waiting to heat up */
bool heatGetIsWaiting(TOOL tool) {
    return heater_active.T[tool].waiting;
}

/* Check all heater if there is a heater to be waited */
bool heatHasWaiting(void) {
    for (TOOL tool = 0; tool < HEATER_COUNT; tool++) {
        if (heater_active.T[tool].waiting == true) {
            return true;
        }
    }
    return false;
}

/* Set heater waiting status */
void heatSetIsWaiting(TOOL tool, bool has_waiting) {
    heater_active.T[tool].waiting = has_waiting;
    if (has_waiting == true) {
        state_update_interval = 100;
    } else if (heatHasWaiting() == false) {
        state_update_interval = 300;
    }
}

void heatClearIsWaiting(void) {
    for (TOOL tool = 0; tool < HEATER_COUNT; tool++) {
        heater_active.T[tool].waiting = false;
    }
    state_update_interval = 300;
}

/* Set current heater tool, nozzle or hot bed */
void heatSetCurrentTool(TOOL tool) {
    heater_active.tool = tool;
    heater_shadow.tool = tool;
    setup_heater_icon(heater_active.tool, KEY_ICON_4);
}

/* Get current tool, nozzle or hot bed */
TOOL heatGetCurrentTool(void) {
    return heater_active.tool;
}

/* Set current nozzle */
void heatSetCurrentToolNozzle(TOOL tool) {
    if (tool < TOOL_NOZZLE0 || tool >= HEATER_COUNT) {
        return;
    }
    heater_active.nozzle = tool;
    heater_active.tool = tool;
}

/* Get current nozzle*/
TOOL heatGetCurrentToolNozzle(void) {
    return heater_active.nozzle;
}

/* Set temperature update time interval */
void heatSetUpdateTime(u32 time) {
    state_update_interval = time;
}

/* Set whether we need to query the current temperature */
void heatSetUpdateWaiting(bool isWaiting) {
    state_has_update_waiting = isWaiting;
}

/* Set whether the heating command has been sent */
void heatSetSendWaiting(TOOL tool, bool isWaiting) {
    state_has_command_pending[tool] = isWaiting;
}

static void render_current_temp(void) {
    GUI_DispDec(CENTER_X - BYTE_WIDTH * 3, CENTER_Y, heater_active.T[heater_active.tool].actual, 3, RIGHT);
}

static void render_target_temp(void) {
    GUI_DispDec(CENTER_X + BYTE_WIDTH * 1, CENTER_Y, heater_active.T[heater_active.tool].target, 3, LEFT);
}

static void render_both_temp(void) {

    const GUI_RECT rect =
            {
              exhibitRect.x0, CENTER_Y - BYTE_HEIGHT,
              exhibitRect.x1, CENTER_Y,
            };

    GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
    GUI_DispStringInPrect(&rect, (u8*) heater_entry_list[heater_active.tool].label);

    render_current_temp();
    GUI_DispString(CENTER_X, CENTER_Y, (u8*) "/");
    render_target_temp();
}

void menuHeaterControl(void) {
    KEY_VALUE key_num = KEY_IDLE;

    heater_shadow = heater_active;  // copy struct
    state_update_interval = 100;  // 1 second

    heater_ensure_config();
    menuDrawPage(&HeaterItems);

    render_both_temp();

    while (infoMenu.menu[infoMenu.cur] == menuHeaterControl) {
        key_num = menuKeyGetValue();

        const u16 temp_min = heater_entry_list[heater_active.tool].temp_min;
        const u16 temp_max = heater_entry_list[heater_active.tool].temp_max;
        const u16 temp_step = temp_step_size_list[current_temp_step_index];
        int16_t *temp_target = &(heater_active.T[heater_active.tool].target);
        TOOL next_tool = 0;

        switch (key_num) {

        case KEY_ICON_0:  // Decrease
            *temp_target = limitValue(temp_min, *temp_target - temp_step, temp_max);
            break;

        case KEY_ICON_1:  // TODO heater detail popup
            break;

        case KEY_ICON_2:  // Increase
            *temp_target = limitValue(temp_min, *temp_target + temp_step, temp_max);
            break;

        case KEY_ICON_3:  // TODO heater detail popup
            break;

        case KEY_ICON_4:  // Switch Tool
            next_tool = heater_next_tool(heater_active.tool);
            heater_active.tool = next_tool;
            heater_shadow.tool = next_tool;
            setup_heater_icon(next_tool, key_num);
            menuDrawItem(&HeaterItems.items[key_num], key_num);
            render_both_temp();
            break;

        case KEY_ICON_5:  // Switch Step
            current_temp_step_index = (current_temp_step_index + 1) % TEMP_STEP_COUNT;
            HeaterItems.items[key_num] = temp_step_item_list[current_temp_step_index];
            menuDrawItem(&HeaterItems.items[key_num], key_num);
            break;

        case KEY_ICON_6:  // Stop
            *temp_target = 0;
            break;

        case KEY_ICON_7:  // Back
            infoMenu.cur--;
            break;

        default:
            break;
        }

        if (heater_shadow.tool != heater_active.tool) {
            heater_shadow.tool = heater_active.tool;
            render_both_temp();
        }
        if (heater_shadow.T[heater_active.tool].actual != heater_active.T[heater_active.tool].actual) {
            heater_shadow.T[heater_active.tool].actual = heater_active.T[heater_active.tool].actual;
            render_current_temp();
        }
        if (heater_shadow.T[heater_active.tool].target != heater_active.T[heater_active.tool].target) {
            render_target_temp();
        }

        loopProcess();
    }

    if (heatHasWaiting() == false) {
        state_update_interval = 300;
    }
}

u32 lastHeatCheckTime = 0;

void updateLastHeatCheckTime(void) {
    lastHeatCheckTime = OS_GetTime();
}

void loopCheckHeater(void) {

    do {  // send M105 query temperature continuously
        if (state_has_update_waiting == true) {
            updateLastHeatCheckTime();
            break;
        }
        if (OS_GetTime() - lastHeatCheckTime < state_update_interval) {
            break;
        }
        if (RequestCommandInfoIsRunning()) {
            break;  // to avoid colision in gcode response processing
        }
        if (storeCmd("M105\n") == false) {
            break;
        }
        updateLastHeatCheckTime();
        state_has_update_waiting = true;
    } while (0);

    // query the heater that needs to wait for the temperature to rise, whether it reaches the set temperature
    for (TOOL tool = 0; tool < HEATER_COUNT; tool++) {
        if (heater_has_use(tool) == false) {
            continue;  // ignore disabled heater
        }
        if (heater_active.T[tool].waiting == false) {
            continue;  // ignore non-wating heater
        }
        if (tool == TOOL_HOTBED) {
            if (heater_active.T[TOOL_HOTBED].actual + 2 <= heater_active.T[TOOL_HOTBED].target) {
                continue;  // ignore small hotbed hysteresis
            }
        } else {
            if (inRange(heater_active.T[tool].actual, heater_active.T[tool].target, 2) != true) {
                continue;  // ignore small nozzle hysteresis
            }
        }
        heater_active.T[tool].waiting = false;
        if (heatHasWaiting() == true) {
            continue;
        }
        if (infoMenu.menu[infoMenu.cur] == menuHeaterControl) {
            break;
        }
        state_update_interval = 300;
    }

    // if the target temperature changes, send a gcode to printer
    for (TOOL tool = 0; tool < HEATER_COUNT; tool++) {
        if (heater_has_use(tool) == false) {
            continue;  // ignore disabled heater
        }
        if (heater_shadow.T[tool].target != heater_active.T[tool].target) {
            heater_shadow.T[tool].target = heater_active.T[tool].target;
            if (state_has_command_pending[tool] != true) {
                state_has_command_pending[tool] = true;
                storeCmd("%s ", heater_entry_list[tool].gcode_init);  // keep space
            }
        }
    }
}
