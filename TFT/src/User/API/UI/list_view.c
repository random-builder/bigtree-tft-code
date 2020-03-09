//
// list view support
//

#include <AnyMenu.h>
#include <list_view.h>
#include "includes.h"
#include "GUI.h"

char *dynamic_label[LISTITEM_PER_PAGE];

// generate symbol color mapper (by symbol index)
static const uint16_t mapper_color[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) COLOR ,
#include "symbol.inc"
#undef  X_STAMP
        };

// generate symbol image mapper (by symbol index)
static const char *const mapper_image[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) IMAGE ,
#include "symbol.inc"
#undef  X_STAMP
        };

// generate symbol name mapper (by symbol index)
static const char *const mapper_name[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) STRINGIFY(SYMBOL_##NAME) ,
#include "symbol.inc"
#undef  X_STAMP
        };

// resolve symbol image text from symbol index
uint8_t* symbol_image_by_index(const uint8_t symbol_index) {
    return (uint8_t*) mapper_image[symbol_index];
}

// resolve symbol enum index given symbol enum name
uint16_t symbol_index_by_name(const char *symbol_name) {
    for (uint16_t symbol_index = 0; symbol_index < _SYMBOL_COUNT_; symbol_index++) {
        if (strcmp(symbol_name, mapper_name[symbol_index]) == 0) {
            return symbol_index;
        }
    }
    return SYMBOL_ERROR;  // display error for invalid symbol name
}

GUI_POINT getTextStartPoint(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ICON_POS pos, const char *textchar) {

    GUI_POINT point_item =
            { sx, sy };
    uint16_t w = ex - sx;
    uint16_t h = ey - sy;

    size_t charIcon_w = strlen(textchar) / 3 * BYTE_HEIGHT;

    switch (pos)
    {
    case TOP_LEFT:
        point_item.x = sx + 1;
        point_item.y = sy + 1;
        break;

    case TOP_CENTER:
        point_item.x = (sx + (w - charIcon_w) / 2);
        point_item.y = sy + 1;
        break;

    case TOP_RIGHT:
        point_item.x = ex - charIcon_w - 1;
        point_item.y = sy + 1;
        break;

    case LEFT_CENTER:
        point_item.x = sx + 1;
        point_item.y = (sy + (h - BYTE_HEIGHT) / 2);
        break;

    case MIDDLE:
        point_item.x = (sx + (w - charIcon_w) / 2);
        point_item.y = (sy + (h - BYTE_HEIGHT) / 2);
        break;

    case RIGHT_CENTER:
        point_item.x = ex - charIcon_w - 1;
        point_item.y = (sy + (h - BYTE_HEIGHT) / 2);
        break;

    case BOTTOM_LEFT:
        point_item.x = sx + 1;
        point_item.y = sy + h - BYTE_HEIGHT - 1;
        break;

    case BOTTOM_CENTER:
        point_item.x = (sx + (w - charIcon_w) / 2);
        point_item.y = sy + h - BYTE_HEIGHT - 1;
        break;

    case BOTTOM_RIGHT:
        point_item.x = sx + w - charIcon_w - 1;
        point_item.y = sy + h - BYTE_HEIGHT - 1;
        break;

    default:
        point_item.x = sx + 1;
        point_item.y = sy + 1;
        break;
    }
    return point_item;
}

void ListDrawIcon(const GUI_RECT *rect, ICON_POS iconalign, uint16_t iconindex, uint16_t btn_color) {

    GUI_POINT icon_p = getTextStartPoint(rect->x0, rect->y0, rect->x1, rect->y1, iconalign, mapper_image[iconindex]);
    GUI_SetColor(mapper_color[iconindex]);
    GUI_SetBkColor(btn_color);
    GUI_ClearPrect(rect);
    GUI_DispString(icon_p.x, icon_p.y, symbol_image_by_index(iconindex));
    GUI_RestoreColorDefault();

}

void DrawListItemPress(const GUI_RECT *rect, bool pressed) {
    if (pressed != false)
    {
        GUI_SetColor(WHITE);
        GUI_DrawPrect(rect);
    }
    else
    {
        GUI_SetColor(BACKGROUND_COLOR);
        GUI_DrawPrect(rect);

        GUI_SetColor(LISTBTN_BKCOLOR);
        GUI_DrawLine(rect->x0, rect->y0 - 1, rect->x1 - 1, rect->y0 - 1);
        GUI_DrawLine(rect->x0, rect->y1 - 1, rect->x1 - 1, rect->y1 - 1);
    }
    GUI_RestoreColorDefault();
}

void ListItem_Display(const GUI_RECT *rect, uint8_t positon, const LISTITEM *curitem, bool pressed)
{
    //storeCmd("%d\n",curitem->titlelabel.index);
    if (positon > 4) {
        if (curitem->icon != _SYMBOL_EMPTY_) {
            ListDrawIcon(rect, MIDDLE, curitem->icon, LISTBTN_BKCOLOR);
            if (pressed != false) {
                GUI_DrawPrect(rect);
            }
        }
        else {
            GUI_ClearPrect(rect);
        }
    }
    else if (curitem->icon != _SYMBOL_EMPTY_) {

        GUI_POINT pos = getTextStartPoint(rect->x0, rect->y0, rect->x1, rect->y1, LEFT_CENTER, mapper_image[curitem->icon]);
        int textarea_width;
        switch (curitem->itemType)
        {
        case LIST_LABEL:

            if (curitem->icon != SYMBOL_BLANK) {
                ListDrawIcon(rect, LEFT_CENTER, curitem->icon, BACKGROUND_COLOR);
                pos.x += (BYTE_HEIGHT + 1);
            }

            if (curitem->titlelabel.index != _LABEL_EMPTY_)
                    {
                textarea_width = LISTITEM_WIDTH - (pos.x + 1);  //width after removing the width for icon

                if (curitem->titlelabel.index == _LABEL_DYNAMIC_)
                        {
                    GUI_DispLenString(pos.x, pos.y, (u8*) dynamic_label[positon], textarea_width);
                }
                else
                {
                    GUI_DispLenString(pos.x, pos.y, language_text(curitem->titlelabel.index), textarea_width);
                }
            }
            DrawListItemPress(rect, pressed);
            break;

        case LIST_TOGGLE:
            ;
            int16_t wy = (1 + GUI_StrPixelWidth(symbol_image_by_index(SYMBOL_TOGGLE_BODY)) + 1);   //right edge of text area
            GUI_ClearRect(rect->x0, rect->y0, rect->x1 - wy, rect->y1);                     // clear only tect area
            textarea_width = LISTITEM_WIDTH - (pos.x + wy);                                 //width after removing the width for icon

            GUI_DispLenString(pos.x, pos.y, language_text(curitem->titlelabel.index), textarea_width);
            pos = getTextStartPoint(rect->x0, rect->y0, rect->x1, rect->y1, RIGHT_CENTER, mapper_image[SYMBOL_TOGGLE_BODY]);
            GUI_ClearRect(rect->x1 - wy, rect->y0, rect->x1, rect->y1);
            ListItem_DisplayToggle(pos.x, pos.y, curitem->icon);
            DrawListItemPress(rect, pressed);
            break;

        case LIST_MOREBUTTON:

            GUI_ClearPrect(rect);

            if (curitem->icon != SYMBOL_BLANK) {
                ListDrawIcon(rect, LEFT_CENTER, curitem->icon, BLACK);
                pos.x += (BYTE_HEIGHT + 1);
            }
            textarea_width = LISTITEM_WIDTH - (pos.x + BYTE_HEIGHT + 2);  //width after removing the width for icon

            GUI_DispLenString(pos.x, pos.y, language_text(curitem->titlelabel.index), textarea_width);

            pos = getTextStartPoint(rect->x0, rect->y0, rect->x1, rect->y1, RIGHT_CENTER, mapper_image[SYMBOL_DETAIL]);
            GUI_SetColor(mapper_color[SYMBOL_DETAIL]);
            GUI_DispString(pos.x, pos.y, symbol_image_by_index(SYMBOL_DETAIL));

            DrawListItemPress(rect, pressed);
            break;

        case LIST_CUSTOMVALUE:
            if (curitem->icon != SYMBOL_BLANK) {
                ListDrawIcon(rect, LEFT_CENTER, curitem->icon, BLACK);
                pos.x += (BYTE_HEIGHT + 3);
            }
            GUI_DispString(pos.x, pos.y, language_text(curitem->titlelabel.index));

            ListItem_DisplayCustomValue(rect, language_text(curitem->valueLabel.index));

            DrawListItemPress(rect, pressed);
            break;

        default:
            break;
        }

    }
    else
    {
        GUI_ClearPrect(rect);
    }

    GUI_RestoreColorDefault();
}

void ListItem_DisplayToggle(uint16_t sx, uint16_t sy, uint8_t iconchar_state)
{
//   const GUI_RECT *rect = rect_of_keyListView + positon;
//   GUI_POINT pos = getTextStartPoint(rect_of_keyListView.x0, rect->y0, rect->x1,rect->y1, pos, iconchar);

    //GUI_ClearPrect(&rect_item);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    GUI_SetColor(LISTBTN_BKCOLOR);
    GUI_DispString(sx, sy, (uint8_t*) mapper_image[SYMBOL_TOGGLE_BODY]);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);

    GUI_SetColor(mapper_color[iconchar_state]);
    if (iconchar_state == SYMBOL_TOGGLE_OFF)
            {
        GUI_DispString(sx, sy, symbol_image_by_index(SYMBOL_TOGGLE_SWITCH));
    }
    else
    {
        GUI_DispString(sx + BYTE_HEIGHT, sy, symbol_image_by_index(SYMBOL_TOGGLE_SWITCH));
    }

    GUI_RestoreColorDefault();

}

void ListItem_DisplayCustomValue(const GUI_RECT *rect, uint8_t *value)
{

    const GUI_RECT rectVal =
            { rect->x1 - BYTE_WIDTH * 8 - 1, rect->y0 + (LISTITEM_HEIGHT - BYTE_HEIGHT) / 2, rect->x1 - 1, rect->y1 - (LISTITEM_HEIGHT - BYTE_HEIGHT) / 2 };

//  GUI_POINT pos = getTextStartPoint(rect_of_keyListView.x0, rect->y0, rect->x1,rect->y1, pos, iconchar);

    GUI_ClearPrect(&rectVal);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);

    GUI_SetColor(LISTBTN_BKCOLOR);

    GUI_DrawPrect(&rectVal);

    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetBkColor(LISTBTN_BKCOLOR);
    GUI_SetColor(MAT_LOWWHITE);
    GUI_DispStringInPrect(&rectVal, value);

    GUI_RestoreColorDefault();
}

// setup page buttons attached to the list view
void lister_setup_pager_icons(  //
        const LISTER_PAGE page_index,  //
        const LISTER_PAGE page_count,  //
        LISTITEMS *list_view_page  //
        ) {
    LISTITEM *button_page_up = &(list_view_page->items[BUTTON_PAGE_UP]);
    LISTITEM *button_page_down = &(list_view_page->items[BUTTON_PAGE_DOWN]);
    if (page_count <= 1) {
        button_page_up->icon = SYMBOL_BLANK;
        button_page_down->icon = SYMBOL_BLANK;
    } else {
        if (page_index <= 0) {
            button_page_up->icon = SYMBOL_BLANK;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        } else if (page_index >= (page_count - 1)) {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_BLANK;
        } else {
            button_page_up->icon = SYMBOL_PAGEUP;
            button_page_down->icon = SYMBOL_PAGEDOWN;
        }
    }
}

// process key press for a list view
void lister_process_navigation(  //
        const KEY_VALUE key_num,  //
        LISTER_PAGE *page_index,  //
        const LISTER_PAGE page_count,  //
        void (*react_button_page)(void),  //
        void (*react_button_default)(const KEY_VALUE)  //
        ) {

    switch (key_num) {

    case BUTTON_PAGE_UP:
        if (page_count > 1) {
            if ((*page_index) > 0) {
                (*page_index)--;
                react_button_page();
            }
        }
        break;

    case BUTTON_PAGE_DOWN:
        if (page_count > 1) {
            if ((*page_index) < page_count - 1) {
                (*page_index)++;
                react_button_page();
            }
        }
        break;

    case BUTTON_BACK:
        infoMenu.cur--;
        break;

    default:
        react_button_default(key_num);
        break;
    }

}
