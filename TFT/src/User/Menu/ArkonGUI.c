//
// top level / root screen
//

#include <ArkonGUI.h>
#include "includes.h"
#include "GUI.h"

//1 title, ITEM_PER_PAGE items (icon + label) 
static MENUITEMS StatusItems = {
// title
LABEL_READY,
// icon                       label
 {
  {ICON_StatusNozzle,        _LABEL_EMPTY_}, // 0
  {ICON_StatusBed,           _LABEL_EMPTY_},
  {ICON_StatusFan,           _LABEL_EMPTY_},
  {ICON_StatusSpeed,         _LABEL_EMPTY_},
  {ICON_MainMenu,             LABEL_MAINMENU},   // 4
  {_ICON_EMPTY_,           _LABEL_EMPTY_}, // status window
  {_ICON_EMPTY_,           _LABEL_EMPTY_}, // status window
  {_ICON_EMPTY_,           _LABEL_EMPTY_}, // {ICON_PRINT,                LABEL_PRINT},}
 }
};

static const ITEM ToolItems[3] = {
// icon                       label
  {ICON_StatusNozzle,         _LABEL_EMPTY_},
  {ICON_StatusBed,            _LABEL_EMPTY_},
  {ICON_StatusFan,            _LABEL_EMPTY_},
};
static const ITEM SpeedItems[2] = {
// icon                       label
  {ICON_StatusSpeed,         _LABEL_EMPTY_},
  {ICON_StatusFlow,          _LABEL_EMPTY_},
};

static u32 nowTime = 0;
static u32 update_time = 100; // 1 seconds is 100
static SCROLL     msgScroll;
static int connected_status = -1;

static char msg_head[20];
static char msg_body[512];

//static char msgxyz[512];
static float position_X;
static float position_Y;
static float position_Z;
static bool positionCmdWait = false;

static TOOL current_Ext = TOOL_NOZZLE0;
static int current_fan = 0;
static int current_speedID = 0;
static const char* SpeedID[2] = SPEED_ID;
// text position rectangles for Live icons 
//icon 0
static const GUI_POINT pointID[4] = {
  {1*ICON_WIDTH+0*SPACE_X+START_X - BYTE_WIDTH/2,    ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {2*ICON_WIDTH+1*SPACE_X+START_X - BYTE_WIDTH/2,    ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {3*ICON_WIDTH+2*SPACE_X+START_X - BYTE_WIDTH/2,    ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
  {4*ICON_WIDTH+3*SPACE_X+START_X - BYTE_WIDTH/2,    ICON_START_Y + 0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_NAME_Y0},
};
static const GUI_RECT rectB[4] = {
  {START_X + 0 * ICON_WIDTH + 0 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 1 * ICON_WIDTH + 0 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

  {START_X + 1 * ICON_WIDTH + 1 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 2 * ICON_WIDTH + 1 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

  {START_X + 2 * ICON_WIDTH + 2 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 3 * ICON_WIDTH + 2 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},

  {START_X + 3 * ICON_WIDTH + 3 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0,
   START_X + 4 * ICON_WIDTH + 3 * SPACE_X,  ICON_START_Y +  0 * ICON_HEIGHT + 0 * SPACE_Y + SSICON_VAL_Y0 + BYTE_HEIGHT},
};

//info rectangle          
static const GUI_RECT RectInfo = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X,  ICON_START_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X,  ICON_START_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y};

static const  GUI_RECT msgRect ={START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2,   ICON_START_Y +  1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                          START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2,   ICON_START_Y +  2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

static const GUI_RECT RecGantry = {START_X,                        1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y + STATUS_GANTRY_YOFFSET,
                            4*ICON_WIDTH+3*SPACE_X+START_X, 1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y - STATUS_GANTRY_YOFFSET};


/*set status icons */
/* void set_status_icon(void)
{
  StatusItems.items[0] = ToolItems[0];
  StatusItems.items[1] = ToolItems[1];
  StatusItems.items[2] = ToolItems[2];
  StatusItems.items[3] = SpeedItems[0];

} */

static void drawTemperature(void)
{
  //icons and their values are updated one by one to reduce flicker/clipping

  char tempstr[100];
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_SetColor(HEADING_COLOR);
  menuDrawIconOnly(&ToolItems[0],0);                                                                //Ext icon
  GUI_DispStringRight(pointID[0].x, pointID[0].y, (u8 *)heatDisplayID[current_Ext]);                           //Ext label

  GUI_SetColor(VAL_COLOR);
  my_sprintf(tempstr, "%d/%d", heatGetCurrentTemp(current_Ext), heatGetTargetTemp(current_Ext));
  GUI_DispStringInPrect(&rectB[0], (u8 *)tempstr);                                                //Ext value

  GUI_SetColor(HEADING_COLOR);
  menuDrawIconOnly(&ToolItems[1],1);                                          //Bed icon
  GUI_DispStringRight(pointID[1].x, pointID[1].y, (u8 *)heatDisplayID[TOOL_HOTBED]);             //Bed label
  GUI_SetColor(VAL_COLOR);
  my_sprintf(tempstr, "%d/%d", heatGetCurrentTemp(TOOL_HOTBED), heatGetTargetTemp(TOOL_HOTBED));
  GUI_DispStringInPrect(&rectB[1], (u8 *)tempstr);                        //Bed value

  GUI_SetColor(HEADING_COLOR);
  menuDrawIconOnly(&ToolItems[2],2);                                          //Fan icon
  GUI_DispStringRight(pointID[2].x, pointID[2].y, (u8 *)fanID[current_fan]);              //Fan label
  GUI_SetColor(VAL_COLOR);

  u8 fs;
    fs = (fanGetSpeed(current_fan)*100)/255;
    my_sprintf(tempstr, "%d%%", fs);

  GUI_DispStringInPrect(&rectB[2], (u8 *)tempstr);                        //Fan value

  GUI_SetColor(HEADING_COLOR);
  menuDrawIconOnly(&SpeedItems[current_speedID],3);                           //Speed / flow icon
  GUI_DispStringRight(pointID[3].x, pointID[3].y, (u8 *)SpeedID[current_speedID]);       //Speed / flow label
  GUI_SetColor(VAL_COLOR);
  my_sprintf(tempstr, "%d%s", speedGetPercent(current_speedID),"%");
  GUI_DispStringInPrect(&rectB[3], (u8 *)tempstr);                        //Speed / Flow value

  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetColor(GANTRYLBL_COLOR);
  GUI_SetBkColor(GANTRYLBL_BKCOLOR);
  my_sprintf(tempstr, "   X: %.2f   Y: %.2f   Z: %.2f   ", position_X, position_Y, position_Z);
  GUI_DispStringInPrect(&RecGantry,(u8 *)tempstr);

  GUI_RestoreColorDefault();
}

void persistPosition(int n, float val){
    //float* px = &val;
  switch (n)
  {
  case 0:
    position_X = val;
    break;
  case 1:
    position_Y = val;
    break;
  case 2:
    position_Z = val;
    break;
  default:
    break;
  }
  positionCmdWait = false;
}

//static void gantry_inc(int n, float val){
//    //float* px = &val;
//  switch (n)
//  {
//  case 0:
//    position_X += val;
//    if ( position_X > X_MAX_POS){
//      position_X = X_MAX_POS;
//    }
//    break;
//  case 1:
//    position_Y += val;
//    if ( position_Y > Y_MAX_POS){
//      position_Y = Y_MAX_POS;
//    }
//    break;
//  case 2:
//    position_Z += val;
//    if ( position_Z > Z_MAX_POS){
//      position_Z = Z_MAX_POS;
//    }
//    break;
//  default:
//    break;
//  }
//}

//static void gantry_dec(int n, float val){
//    //float* px = &val;
//  switch (n)
//  {
//  case 0:
//    position_X -= val;
//    if ( position_X < X_MIN_POS){
//      position_X = X_MIN_POS;
//    }
//    break;
//  case 1:
//    position_Y -= val;
//    if ( position_Y < Y_MIN_POS){
//      position_Y = Y_MIN_POS;
//    }
//    break;
//  case 2:
//    position_Z -= val;
//    if ( position_Z < Z_MIN_POS){
//      position_Z = Z_MIN_POS;
//    }
//    break;
//  default:
//    break;
//  }
//}

float getAxisLocation(u8 n){
  switch (n)
  {
  case 0:
    return position_X;
  case 1:
    return position_Y;
  case 2:
    return position_Z;
  default:
    return position_X;
  }
}


void statusScreen_setMsg(const uint8_t *title, const uint8_t *msg)
{
  memcpy(msg_head, (char *)title, sizeof(msg_head));
  memcpy(msg_body, (char *)msg, sizeof(msg_body));

  if (infoMenu.menu[infoMenu.cur] == menuStatus)
  {
    drawStatusScreenMsg();
  }
}

void drawStatusScreenMsg(void)
{
//GUI_ClearRect(RectInfo.x0,RectInfo.y0,RectInfo.x1,RectInfo.y1);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  ICON_CustomReadDisplay(RectInfo.x0,RectInfo.y0,INFOBOX_WIDTH,INFOBOX_HEIGHT,INFOBOX_ADDR);
  GUI_SetColor(INFOMSG_BKCOLOR);
  GUI_DispString(RectInfo.x0 + STATUS_MSG_ICON_XOFFSET, RectInfo.y0 + STATUS_MSG_ICON_YOFFSET,symbol_image_by_index(SYMBOL_INFO));

  GUI_DispString(RectInfo.x0 + BYTE_HEIGHT+ STATUS_MSG_TITLE_XOFFSET,RectInfo.y0 + STATUS_MSG_ICON_YOFFSET,(u8*)msg_head);
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_FillPrect(&msgRect);

  Scroll_CreatePara(&msgScroll, (u8 *)msg_body, &msgRect);

  GUI_RestoreColorDefault();
}

void scrollMsg(void){
  GUI_SetBkColor(INFOMSG_BKCOLOR);
  GUI_SetColor(INFOMSG_COLOR);
  Scroll_DispString(&msgScroll,CENTER);
  GUI_RestoreColorDefault();
}

void toggleTool(void)
{
  if (OS_GetTime() > nowTime + update_time)
  {
    // FIXME decouple extruder from heater
    {
      current_Ext = heater_next_nozzle(current_Ext);
      if (current_Ext == 0)
      {
        current_Ext += 1;
      }
    }
    if (FAN_NUM > 1)
    {
      current_fan = (current_fan + 1) % FAN_NUM;
    }
    current_speedID = (current_speedID + 1) % 2;
    nowTime = OS_GetTime();
    drawTemperature();

    if (infoHost.connected == true)
    {
      if (positionCmdWait != true)
      {
        positionCmdWait = true;
        storeCmd("M114\n"); // query position
      }
    }
    else
    {
      positionCmdWait = false;
    }
  }
}

void menuStatus(void)
{
  KEY_VALUE key_num = KEY_IDLE;

  heater_ensure_config();
  GUI_SetBkColor(BACKGROUND_COLOR);
//set_status_icon();
  menuDrawPage(&StatusItems);
  GUI_SetColor(GANTRYLBL_BKCOLOR);
//GUI_ClearPrect(&RecGantry);
  GUI_FillPrect(&RecGantry);
  drawTemperature();
  drawStatusScreenMsg();

  while (infoMenu.menu[infoMenu.cur] == menuStatus)
  {
    if(infoHost.connected != connected_status){
      if(infoHost.connected == false){
        statusScreen_setMsg(language_text(LABEL_SCREEN_INFO), language_text(LABEL_UNCONNECTED));
      }
      else{
        statusScreen_setMsg(language_text(LABEL_SCREEN_INFO), language_text(LABEL_READY));
      }
      connected_status = infoHost.connected;
    }
    scrollMsg();
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0: // dynamic nozzle
        heatSetCurrentTool(TOOL_NOZZLE0);
        infoMenu.menu[++infoMenu.cur] = menuHeaterControl;
        break;
      case KEY_ICON_1: // dynamic hotbed
        heatSetCurrentTool(TOOL_HOTBED);
        infoMenu.menu[++infoMenu.cur] = menuHeaterControl;
        break;
      case KEY_ICON_2: // dynamic fan
        infoMenu.menu[++infoMenu.cur] = menuCoolerControl;
        break;
      case KEY_ICON_3: // dynamic motion speed / feed rate
        infoMenu.menu[++infoMenu.cur] = menuFeedSpeed;
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuMain;
        break;

//      case KEY_ICON_7:
//        infoMenu.menu[++infoMenu.cur] = menuMain; // menuPrint;
//        break;

      default:break;
    }
    toggleTool();
    loopProcess();
  }
}
