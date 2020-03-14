//
// list view support
//

#include <AnyMenu.h>
#include <list_view.h>
#include "includes.h"
#include "symbol.h"
#include "GUI.h"

char * dynamic_label[LISTITEM_PER_PAGE];

float dynamic_value[LISTITEM_PER_PAGE];

uint8_t * IconCharSelect(uint8_t sel)
{
return (uint8_t *)symbol_mapper_image[sel];
}
char * IconChar(uint8_t sel)
{
return (char *)symbol_mapper_image[sel];
}

// save dynamic text label ( i : index of the label position, label: char * to the text)
void setDynamicLabel(uint8_t i, char *label){
  dynamic_label[i] = label;
}

// get dynamic text label ( i : index of the label position)
char * getDynamicLabel(uint8_t i){
  return dynamic_label[i];
}

// save dynamic value ( i : index of the value position, value:float value)
void setDynamicValue(uint8_t i,float value){
dynamic_value[i] = value;
}

// get dynamic numerical value ( i : index of the value position)
float getDynamicValue(uint8_t i){
  return dynamic_value[i];
}

// get the text starting point on screen based on rectangle edges and desired icon position
GUI_POINT getTextStartPoint(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ICON_POS pos, const char * textchar){

  GUI_POINT point_item = {sx, sy};
  uint16_t w = ex- sx;
  uint16_t h = ey- sy;

  size_t charIcon_w = strlen(textchar)/3 * BYTE_HEIGHT;

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
    point_item.x = (sx +(w - charIcon_w) / 2);
    point_item.y = (sy +(h - BYTE_HEIGHT) / 2);
    break;

  case RIGHT_CENTER:
    point_item.x = ex - charIcon_w - 1;
    point_item.y = (sy +(h - BYTE_HEIGHT) / 2);
    break;

  case BOTTOM_LEFT:
    point_item.x = sx + 1;
    point_item.y = sy + h - BYTE_HEIGHT - 1;
    break;

  case BOTTOM_CENTER:
    point_item.x = (sx +(w - charIcon_w) / 2);
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

//draw icons in item
void DrawCharIcon(const GUI_RECT * rect,ICON_POS iconalign, uint16_t iconindex, uint16_t btn_color){

      GUI_POINT icon_p = getTextStartPoint(rect->x0, rect->y0, rect->x1,rect->y1,iconalign,symbol_mapper_image[iconindex]);
      GUI_SetColor(symbol_mapper_color[iconindex]);
      GUI_SetBkColor(btn_color);
      GUI_ClearPrect(rect);
      GUI_DispString(icon_p.x, icon_p.y, IconCharSelect(iconindex));
      GUI_RestoreColorDefault();

}

//draw item pressed feedback
void DrawListItemPress(const GUI_RECT * rect, bool pressed){
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
        GUI_DrawLine(rect->x0, rect->y0-1 , rect->x1-1, rect->y0-1 );
        GUI_DrawLine(rect->x0, rect->y1-1 , rect->x1-1, rect->y1-1 );
      }
      GUI_RestoreColorDefault();
}

void ListItem_Display(const GUI_RECT* rect, uint8_t position, const LISTITEM * curitem, bool pressed)
{
  //draw navigation icons
  if(position >= LISTITEM_PER_PAGE){
    if(curitem->icon != _SYMBOL_EMPTY_){
      DrawCharIcon(rect,MIDDLE,curitem->icon,LISTBTN_BKCOLOR);
      if (pressed != false){
        GUI_DrawPrect(rect);
      }
    }
    else{
      GUI_ClearPrect(rect);
    }
  }
  //draw list tiems
  else if (curitem->icon != _SYMBOL_EMPTY_){

    GUI_POINT pos = getTextStartPoint(rect->x0, rect->y0, rect->x1,rect->y1,LEFT_CENTER,symbol_mapper_image[curitem->icon]);
    int textarea_width;
    switch (curitem->itemType)
    {
    case LIST_LABEL:

      if(curitem->icon != SYMBOL_BLANK) {
        DrawCharIcon(rect,LEFT_CENTER,curitem->icon, BACKGROUND_COLOR);
        pos.x += (BYTE_HEIGHT + 1);
       }
      textarea_width = LISTITEM_WIDTH - (pos.x + 1); //width after removing the width for icon
      draw_itemtitle(pos,curitem->titlelabel,position,textarea_width);

      DrawListItemPress(rect, pressed);
      break;

    case LIST_TOGGLE:;
      int16_t wy = (1 + GUI_StrPixelWidth(IconCharSelect(SYMBOL_TOGGLE_BODY)) + 1);   //right edge of text area
      GUI_ClearRect(rect->x0, rect->y0, rect->x1 - wy, rect->y1);                     // clear only text area
      textarea_width = LISTITEM_WIDTH - (pos.x + wy);                                 //width after removing the width for icon
      draw_itemtitle(pos,curitem->titlelabel,position,textarea_width);

      pos = getTextStartPoint(rect->x0, rect->y0, rect->x1,rect->y1,RIGHT_CENTER,symbol_mapper_image[SYMBOL_TOGGLE_BODY]);
      GUI_ClearRect(rect->x1-wy,rect->y0,rect->x1,rect->y1);
      ListItem_DisplayToggle(pos.x, pos.y, curitem->icon);
      DrawListItemPress(rect,pressed);
      break;

    case LIST_MOREBUTTON:

      GUI_ClearPrect(rect);

      if(curitem->icon != SYMBOL_BLANK) {
        DrawCharIcon(rect,LEFT_CENTER,curitem->icon,BLACK);
        pos.x += (BYTE_HEIGHT + 1);
       }
      textarea_width = LISTITEM_WIDTH - (pos.x + BYTE_HEIGHT + 2);  //width after removing the width for icon
      draw_itemtitle(pos,curitem->titlelabel,position,textarea_width);

      pos = getTextStartPoint(rect->x0, rect->y0, rect->x1,rect->y1,RIGHT_CENTER,symbol_mapper_image[SYMBOL_DETAIL2]);
      GUI_SetColor(symbol_mapper_color[SYMBOL_DETAIL2]);
      GUI_DispString(pos.x, pos.y,IconCharSelect(SYMBOL_DETAIL2));


      DrawListItemPress(rect,pressed);
      break;

    case LIST_CUSTOMVALUE:
      if(curitem->icon != SYMBOL_BLANK) {
        DrawCharIcon(rect,LEFT_CENTER,curitem->icon,BLACK);
        pos.x += (BYTE_HEIGHT + 3);
      }
      GUI_ClearRect(pos.x, rect->y0, rect->x1 - BYTE_WIDTH*8 -1, rect->y1);        // clear only text area
      textarea_width = LISTITEM_WIDTH - (pos.x + 1); //width after removing the width for icon
      draw_itemtitle(pos,curitem->titlelabel,position,textarea_width);

      ListItem_DisplayCustomValue(rect,curitem->valueLabel,position);

      DrawListItemPress(rect,pressed);
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

//draw title text of list item
void draw_itemtitle(GUI_POINT pos,LABEL label, uint8_t position, int textarea_width){
  if (label.index != _LABEL_EMPTY_)
  {
    int textarea_width = LISTITEM_WIDTH - (pos.x + 1); //width after removing the width for icon
    if (label.index == _LABEL_DYNAMIC_)
    {GUI_DispLenString(pos.x, pos.y, (u8*)getDynamicLabel(position),textarea_width);
    }
    else
    {GUI_DispLenString(pos.x, pos.y, language_text(label.index),textarea_width);
    }
  }
}

//display toggle button
void ListItem_DisplayToggle(uint16_t sx, uint16_t sy, uint8_t iconchar_state)
{
  //const GUI_RECT *rect = rect_of_keyListView + position;
  //GUI_POINT pos = getTextStartPoint(rect_of_keyListView.x0, rect->y0, rect->x1,rect->y1, pos, iconchar);

  //GUI_ClearPrect(&rect_item);
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetColor(LISTBTN_BKCOLOR);
  GUI_DispString(sx, sy, (uint8_t*)symbol_mapper_image[SYMBOL_TOGGLE_BODY]);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  GUI_SetColor(symbol_mapper_color[iconchar_state]);
  if (iconchar_state == SYMBOL_TOGGLE_OFF)
  {
    GUI_DispString(sx, sy, IconCharSelect(SYMBOL_TOGGLE_SWITCH));
  }
  else
  {
    GUI_DispString(sx+BYTE_HEIGHT, sy, IconCharSelect(SYMBOL_TOGGLE_SWITCH));
  }

  GUI_RestoreColorDefault();

}

//draw custom value for list item
void ListItem_DisplayCustomValue(const GUI_RECT* rect,LABEL value,int i)
{

  const GUI_RECT rectVal = {rect->x1-BYTE_WIDTH*8 -1,rect->y0+(LISTITEM_HEIGHT-BYTE_HEIGHT)/2,rect->x1-1,rect->y1-(LISTITEM_HEIGHT-BYTE_HEIGHT)/2};
//GUI_POINT pos = getTextStartPoint(rect_of_keyListView.x0, rect->y0, rect->x1,rect->y1, pos, iconchar);

  GUI_ClearPrect(&rectVal);
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);

  GUI_SetColor(LISTBTN_BKCOLOR);

  GUI_DrawPrect(&rectVal);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_SetColor(MAT_LOWWHITE);

  if(value.index == _LABEL_DYNAMIC_){
    GUI_DispFloat(rectVal.x0,rectVal.y0,dynamic_value[i],4,1,RIGHT);
  }
  else{
    GUI_DispStringInPrect(&rectVal,language_text(value.index));
  }

  GUI_RestoreColorDefault();
}
