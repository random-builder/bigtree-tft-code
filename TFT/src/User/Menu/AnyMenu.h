//
// shared menu layout
//

#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "GUI.h"

#define IDLE_TOUCH	0xFFFF

typedef enum {
    KEY_ICON_0 = 0,
    KEY_ICON_1,
    KEY_ICON_2,
    KEY_ICON_3,
    KEY_ICON_4,
    KEY_ICON_5,
    KEY_ICON_6,
    KEY_ICON_7,
    KEY_LABEL_0,
    KEY_LABEL_1,
    KEY_LABEL_2,
    KEY_LABEL_3,
    KEY_LABEL_4,
    KEY_LABEL_5,
    KEY_LABEL_6,
    KEY_LABEL_7,
    KEY_IDLE = IDLE_TOUCH,
} KEY_VALUE;

typedef enum {
    BUTTON_PAGE_UP = KEY_ICON_5,
    BUTTON_PAGE_DOWN = KEY_ICON_6,
    BUTTON_BACK = KEY_ICON_7,
} KEY_BUTTON;

#define ITEM_PER_PAGE       8
#define LISTITEM_PER_PAGE   5

typedef union {
    uint32_t index;    // language text index, address = language_text(index)
    void *address;  // reference to non-language text, address is explicit
} LABEL;

typedef struct
{
    uint16_t icon;
    LABEL label;
} ITEM;

typedef struct
{
    LABEL title;
    ITEM items[ITEM_PER_PAGE];
} MENUITEMS;

typedef enum
{
    STATUS_IDLE = 0,
    STATUS_BUSY,
    STATUS_UNCONNECT,
    STATUS_NORMAL
} SYS_STATUS;

typedef struct
{
    GUI_RECT rect;
    uint32_t time_stamp;
    uint8_t status_code;
    int16_t label_index;
} REMINDER;

typedef enum
{
    LIST_LABEL = 0,
    LIST_TOGGLE,
    LIST_RADIO,
    LIST_MOREBUTTON,
    LIST_CUSTOMVALUE,
} LISTITEM_TYPE;

typedef struct
{
    uint16_t icon;  // symbol index
    LISTITEM_TYPE itemType;
    LABEL titlelabel;
    LABEL valueLabel;
} LISTITEM;

typedef struct
{
    LABEL title;
    //uint16_t titleIconChar;
    LISTITEM items[ITEM_PER_PAGE];
} LISTITEMS;

extern const GUI_RECT exhibitRect;
#define CENTER_Y  ((exhibitRect.y1 - exhibitRect.y0)/2 + exhibitRect.y0)
#define CENTER_X  ((exhibitRect.x1 - exhibitRect.x0 - BYTE_WIDTH)/2 + exhibitRect.x0)
#define LISTITEM_WIDTH (LCD_WIDTH-(3*START_X)-LIST_ICON_WIDTH)
#define LISTITEM_HEIGHT ((LCD_HEIGHT-ICON_START_Y-START_X)/5)
#define LISTICON_SPACE_Y ((LCD_HEIGHT-ICON_START_Y-START_X-(3*LIST_ICON_HEIGHT))/ 2)

void show_reminder_failure(int16_t label_index, SYS_STATUS status);
void show_reminder_success(int16_t label_index, SYS_STATUS status);

void show_reminder_process(SYS_STATUS status);

void GUI_RestoreColorDefault(void);
void menuDrawItem(const ITEM *menuItem, uint8_t positon);
void menuDrawIconOnly(const ITEM *item, uint8_t positon);
void menuDrawListItem(const LISTITEM *item, uint8_t positon);
void menuRefreshListPage(void);
void menuDrawTitle(const uint8_t *content);  //(const MENUITEMS * menuItems);
void menuDrawPage(const MENUITEMS *menuItems);
void menuDrawListPage(const LISTITEMS *listItems);
void itemDrawIconPress(uint8_t positon, uint8_t is_press);
KEY_VALUE menuKeyGetValue(void);

void loopBackEnd(void);
void loopFrontEnd(void);
void loopProcess(void);
