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