//
// list view support
//

#pragma once

#include "stdint.h"
#include "stdbool.h"

#include <AnyMenu.h>
#include "GUI.h"

typedef enum {
    TOP_LEFT = 0,
    TOP_CENTER,
    TOP_RIGHT,
    LEFT_CENTER,
    MIDDLE,
    RIGHT_CENTER,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
} ICON_POS;

extern char *dynamic_label[LISTITEM_PER_PAGE];

#define LISTBTN_BKCOLOR     0x2187
#define MAT_RED             0xE124
#define MAT_YELLOW          0xED80
#define MAT_GREEN           0x1DC4
#define MAT_BLUE            0x24BD
#define MAT_ORANGE          0xF3A0
#define MATT_DARKGRAY       0x52AA
#define MAT_LOWWHITE        0xCE79
#define MATT_PURPLE         0x9135

//#define _LABEL_DYNAMIC_ 12345 //just random number for reference

void ListItem_Display(const GUI_RECT *rect, uint8_t positon, const LISTITEM *curitem, bool pressed);

void ListMenuSetItem(const LISTITEM *menuItem, uint8_t positon);

void ListItem_DisplayToggle(uint16_t sx, uint16_t sy, uint8_t iconchar_state);

void ListItem_DisplayCustomValue(const GUI_RECT *rect, uint8_t *value);

GUI_POINT getTextStartPoint(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ICON_POS pos, const char *textchar);
