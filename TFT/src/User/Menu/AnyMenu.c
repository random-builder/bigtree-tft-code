//
// shared menu layout
//

#include <AnyMenu.h>
#include <list_view.h>
#include "includes.h"

// exhibitRect position: [-X--]
const GUI_RECT exhibitRect = {
   1*ICON_WIDTH+1*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  2*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y
};

const GUI_RECT rect_of_key[ITEM_PER_PAGE*2]={
  //8 icons area
  {0*ICON_WIDTH+0*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  1*ICON_WIDTH+0*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y},
  {1*ICON_WIDTH+1*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  2*ICON_WIDTH+1*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y},
  {2*ICON_WIDTH+2*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  3*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y},
  {3*ICON_WIDTH+3*SPACE_X+START_X,  0*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  4*ICON_WIDTH+3*SPACE_X+START_X,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y},
  {0*ICON_WIDTH+0*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  1*ICON_WIDTH+0*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {1*ICON_WIDTH+1*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  2*ICON_WIDTH+1*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {2*ICON_WIDTH+2*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  3*ICON_WIDTH+2*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {3*ICON_WIDTH+3*SPACE_X+START_X,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  4*ICON_WIDTH+3*SPACE_X+START_X,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},

  //8 labels area
  {0*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  1*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {1*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  2*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {2*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  3*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {3*SPACE_X_PER_ICON,  1*ICON_HEIGHT+0*SPACE_Y+ICON_START_Y,  4*SPACE_X_PER_ICON,  1*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y},
  {0*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  1*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+ICON_START_Y},
  {1*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  2*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+ICON_START_Y},
  {2*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  3*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+ICON_START_Y},
  {3*SPACE_X_PER_ICON,  2*ICON_HEIGHT+1*SPACE_Y+ICON_START_Y,  4*SPACE_X_PER_ICON,  2*ICON_HEIGHT+2*SPACE_Y+ICON_START_Y},
};

const GUI_RECT rect_of_keyListView[ITEM_PER_PAGE]={
  //8 icons area
  {START_X,  0*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  1*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  1*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  2*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  2*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  3*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  3*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  4*LISTITEM_HEIGHT+ICON_START_Y},
  {START_X,  4*LISTITEM_HEIGHT+ICON_START_Y,    LISTITEM_WIDTH+START_X,  5*LISTITEM_HEIGHT+ICON_START_Y},

  {2*START_X + LISTITEM_WIDTH,  0*LIST_ICON_HEIGHT+0*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  1*LIST_ICON_HEIGHT+0*LISTICON_SPACE_Y+ICON_START_Y},
  {2*START_X + LISTITEM_WIDTH,  1*LIST_ICON_HEIGHT+1*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  2*LIST_ICON_HEIGHT+1*LISTICON_SPACE_Y+ICON_START_Y},
  {2*START_X + LISTITEM_WIDTH,  2*LIST_ICON_HEIGHT+2*LISTICON_SPACE_Y+ICON_START_Y,  2*START_X + LISTITEM_WIDTH + 1*LIST_ICON_WIDTH,  3*LIST_ICON_HEIGHT+2*LISTICON_SPACE_Y+ICON_START_Y},
};

//Clean up the gaps outside icons
void menuClearGaps(void)
{
  const GUI_RECT gaps[]={
  {0,                               0,               LCD_WIDTH,                       TITLE_END_Y},
  {0,                               TITLE_END_Y,     LCD_WIDTH,                       ICON_START_Y},
  {0,                               ICON_START_Y,    0*ICON_WIDTH+0*SPACE_X+START_X,  LCD_HEIGHT},
  {1*ICON_WIDTH+0*SPACE_X+START_X,  ICON_START_Y,    1*ICON_WIDTH+1*SPACE_X+START_X,  LCD_HEIGHT},
  {2*ICON_WIDTH+1*SPACE_X+START_X,  ICON_START_Y,    2*ICON_WIDTH+2*SPACE_X+START_X,  LCD_HEIGHT},
  {3*ICON_WIDTH+2*SPACE_X+START_X,  ICON_START_Y,    3*ICON_WIDTH+3*SPACE_X+START_X,  LCD_HEIGHT},
  {4*ICON_WIDTH+3*SPACE_X+START_X,  ICON_START_Y,    LCD_WIDTH,                       LCD_HEIGHT}};

  GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
  GUI_ClearPrect(gaps);
  GUI_SetBkColor(BACKGROUND_COLOR);
  for(uint8_t i = 1; i < COUNT(gaps); i++)
    GUI_ClearPrect(gaps + i);
}

void GUI_RestoreColorDefault(void){  
  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_SetColor(FONT_COLOR);
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetNumMode(GUI_NUMMODE_SPACE);
}

static const MENUITEMS *CurrentMenuItems = NULL;   // current menu

static const LISTITEMS *CurrentListItems = NULL;   // current listmenu

static bool isListview;

uint8_t *labelGetAddress(const LABEL *label)
{
  if (label->index == _LABEL_EMPTY_) {
      // no content in label
      return NULL;
  } else if (label->index < _LABEL_COUNT_) {
      // label index in language
      return language_text(label->index);
  } else {
      // address of non-language string
      return label->address;
  }
}

void menuDrawItem(const ITEM *item, uint8_t positon)
{
  uint8_t *content = labelGetAddress(&item->label);
  const GUI_RECT *rect = rect_of_key + positon;
  if(item->icon != _ICON_EMPTY_)
    ICON_ReadDisplay(rect->x0, rect->y0, item->icon);
  else
    GUI_ClearPrect(rect);

  rect = rect_of_key + ITEM_PER_PAGE + positon;
  GUI_ClearPrect(rect);
  if (content)
    GUI_DispStringInPrect(rect, content);
}

void menuDrawIconOnly(const ITEM *item, uint8_t positon)
{
  const GUI_RECT *rect = rect_of_key + positon;
  if(item->icon != _ICON_EMPTY_)
    ICON_ReadDisplay(rect->x0, rect->y0, item->icon);
  else
    GUI_ClearPrect(rect);
}

 void menuDrawListItem(const LISTITEM *item, uint8_t position)
{
   const GUI_RECT *rect = rect_of_keyListView + position;
  if (item->icon == _SYMBOL_EMPTY_)
  {
    GUI_ClearPrect(rect);
  }
  else
  {
    ListItem_Display(rect, position, item, false);
  }
}
void menuRefreshListPage(void){
 for (uint8_t i = 0; i < ITEM_PER_PAGE; i++)
    {
      menuDrawListItem(&CurrentListItems->items[i], i);
      #ifdef RAPID_SERIAL_COMM
        #ifndef CLEAN_MODE_SWITCHING_SUPPORT
          if(isPrinting() == true)
        #endif
        {
          loopBackEnd();	 //perform backend printing loop between drawing icons to avoid printer idling
        }
      #endif      
    }

}

static REMINDER reminder_failure = {{0, 0, LCD_WIDTH, TITLE_END_Y}, 0, STATUS_UNCONNECT, LABEL_UNCONNECTED};
static REMINDER reminder_success = {{0, 0, LCD_WIDTH, TITLE_END_Y}, 0, STATUS_IDLE, _LABEL_EMPTY_};
static REMINDER reminder_process = {{LCD_WIDTH - BYTE_WIDTH, 0, LCD_WIDTH, BYTE_HEIGHT}, 0, STATUS_BUSY, LABEL_BUSY};

static bool status_has_setup_config = false;
static u16 config_reminder_timeout = 200;

// extract config setting
static void loop_setup_config() {
    if (status_has_setup_config) {
        return;
    }
    status_has_setup_config = true;
    const SYSTEM_CONFIG *config = config_instance();
    config_reminder_timeout = config_parse_int(config->display_behavior__reminder_timeout) / 10; // millis to local
}

static void render_reminder_process(bool has_active) {
    uint16_t color;
    if (has_active) {
        color = RED;
    } else {
        color = BACKGROUND_COLOR;
    }
    GUI_RECT *rect = &(reminder_process.rect);
    GUI_FillRectColor(rect->x0, rect->y0, rect->x1, rect->y1, color);
}

void show_reminder_failure(int16_t label_index, SYS_STATUS status_code)
{
  reminder_failure.label_index = label_index;
  GUI_SetColor(REMINDER_FONT_COLOR);
  GUI_DispStringInPrect(&reminder_failure.rect, language_text(reminder_failure.label_index));
  GUI_SetColor(FONT_COLOR);
  reminder_failure.status_code = status_code;
  reminder_failure.time_stamp = OS_GetTime();
}

void show_reminder_success(int16_t label_index, SYS_STATUS status_code)
{ 
  reminder_success.label_index = label_index;
  GUI_SetColor(VOLUME_REMINDER_FONT_COLOR);
  GUI_DispStringInPrect(&reminder_success.rect, language_text(reminder_success.label_index));
  GUI_SetColor(FONT_COLOR);
  reminder_success.status_code = status_code;
  reminder_success.time_stamp = OS_GetTime();
}

void show_reminder_process(SYS_STATUS status_code)
{
  render_reminder_process(true);
  reminder_process.status_code = status_code;
  reminder_process.time_stamp = OS_GetTime();
}

static void loop_reminder_failure_reset(void)
{	
  switch(reminder_failure.status_code)
  {
    case STATUS_IDLE:
      return;
    
    case STATUS_BUSY:
      if(infoCmd.count == CMD_MAX_LIST)
        return;
      break;
      
    case STATUS_UNCONNECT:  
      if(infoHost.connected == false)
        return;
      break;
      
    case STATUS_NORMAL:
      if(OS_GetTime() < reminder_failure.time_stamp + config_reminder_timeout)
        return;
      break;
    default:
      return;
  }

  /* Clear warning message */		
  reminder_failure.status_code = STATUS_IDLE;
  if(CurrentMenuItems == NULL)
    return;
  menuDrawTitle(labelGetAddress(&CurrentMenuItems->title));
}

static void loop_reminder_success_reset(void)
{	
  switch(reminder_success.status_code)
  {
    case STATUS_NORMAL:
      if(OS_GetTime() < reminder_success.time_stamp + config_reminder_timeout)
        return;
      break;
    default:
      return;
  }

  /* Clear warning message */		
  reminder_success.status_code = STATUS_IDLE;
  if(CurrentMenuItems == NULL)
    return;
  menuDrawTitle(labelGetAddress(&CurrentMenuItems->title));
}

static void loop_reminder_process_reset(void)
{	
  switch(reminder_process.status_code)
  {
    case STATUS_IDLE:
      return;
    
    case STATUS_BUSY:
     if(OS_GetTime() < reminder_process.time_stamp + config_reminder_timeout)
        return;
     break;
  }

  /* End Busy display sing */		
  render_reminder_process(false);
  reminder_process.status_code = STATUS_IDLE;
}

void menuDrawTitle(const uint8_t *content) //(const MENUITEMS * menuItems)
{
  uint16_t start_x = 10;
  uint16_t start_y = (TITLE_END_Y - BYTE_HEIGHT) / 2;
  GUI_FillRectColor(start_x, start_y, LCD_WIDTH-start_x, start_y+BYTE_HEIGHT, TITLE_BACKGROUND_COLOR);
  
  if (content)
  {
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispLenString(start_x, start_y, content, LCD_WIDTH-start_x*2);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  }

  show_GlobalInfo();
  if(reminder_failure.status_code == STATUS_IDLE) return;
  GUI_SetColor(RED);
  GUI_DispStringInPrect(&reminder_failure.rect, language_text(reminder_failure.label_index));
  GUI_SetColor(FONT_COLOR);
}

//Draw the entire interface
void menuDrawPage(const MENUITEMS *menuItems)
{
  u8 item_index = 0;
  isListview = false;
  CurrentMenuItems = menuItems;
  TSC_ReDrawIcon = itemDrawIconPress;

  //GUI_Clear(BLACK);
  menuClearGaps(); //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&menuItems->title));
  for (item_index = 0; item_index < ITEM_PER_PAGE; item_index++)
  {
    menuDrawItem(&menuItems->items[item_index], item_index);
    #ifdef RAPID_SERIAL_COMM
      #ifndef CLEAN_MODE_SWITCHING_SUPPORT
        if(isPrinting() == true)
      #endif
      {
        loopBackEnd();	 //perform backend printing loop between drawing icons to avoid printer idling
      }
    #endif
  }
}

//Draw the entire interface
void menuDrawListPage(const LISTITEMS *listItems)
{
  u8 i = 0;
  isListview = true;
  CurrentListItems = listItems;
  TSC_ReDrawIcon = itemDrawIconPress;
  
  GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
  GUI_ClearRect(0, 0, LCD_WIDTH, TITLE_END_Y);
  GUI_SetBkColor(BACKGROUND_COLOR);
  GUI_ClearRect(0, TITLE_END_Y, LCD_WIDTH, LCD_HEIGHT);

  //menuClearGaps(); //Use this function instead of GUI_Clear to eliminate the splash screen when clearing the screen.
  menuDrawTitle(labelGetAddress(&listItems->title));

  for (i = 0; i < ITEM_PER_PAGE; i++)
  {
    //const GUI_RECT *rect = rect_of_keyListView + i;
   if (CurrentListItems->items[i].icon != _SYMBOL_EMPTY_)    
     menuDrawListItem(&CurrentListItems->items[i], i);
    #ifdef RAPID_SERIAL_COMM
      #ifndef CLEAN_MODE_SWITCHING_SUPPORT
        if(isPrinting() == true)
      #endif
      {
        loopBackEnd();	 //perform backend printing loop between drawing icons to avoid printer idling
      }
    #endif
  }    
//  show_globalinfo();
}

//When there is a button value, the icon changes color and redraws
void itemDrawIconPress(u8 position, u8 is_press)
{
 
  if (position > KEY_ICON_7) return;

  if (isListview == false)
  {
    if (CurrentMenuItems == NULL) return;
    if (CurrentMenuItems->items[position].icon == _ICON_EMPTY_) return;
    
    const GUI_RECT *rect = rect_of_key + position;
    
    if (is_press) // Turn green when pressed
      ICON_PressedDisplay(rect->x0, rect->y0, CurrentMenuItems->items[position].icon);
    else // Redraw normal icon when released
      ICON_ReadDisplay(rect->x0, rect->y0, CurrentMenuItems->items[position].icon);
  }
  else
  { //draw rec over list item if pressed
    if (CurrentListItems == NULL)
    return;

    const GUI_RECT *rect = rect_of_keyListView + position;

    if (CurrentListItems->items[position].icon == _SYMBOL_EMPTY_)
    {
    GUI_ClearPrect(rect);
    return;
    }
    if (is_press){
    ListItem_Display(rect,position,&CurrentListItems->items[position], true);
    }
    else{ 
    ListItem_Display(rect,position,&CurrentListItems->items[position], false);
    }
  
  }
}

// Get button value
KEY_VALUE menuKeyGetValue(void)
{
  if (isListview == false)
  {
    return (KEY_VALUE)KEY_GetValue(sizeof(rect_of_key) / sizeof(rect_of_key[0]), rect_of_key); // for normal menu
  }
  else
  {
    return (KEY_VALUE)KEY_GetValue(sizeof(rect_of_keyListView) / sizeof(rect_of_keyListView[0]), rect_of_keyListView); //for listview
  }
}

void loopBackEnd(void)
{
  getGcodeFromFile();                 //Get Gcode command from the file to be printed
           
  sendQueueCmd();                     //Parse and send Gcode commands in the queue
  
  parseACK();                         //Parse the received slave response information
  
  parseRcvGcode();                    //Parse the received Gcode from other UART, such as: ESP3D, etc...

  loopCheckHeater();			            //Temperature related settings
  

#if defined ONBOARD_SD_SUPPORT && !defined M27_AUTOREPORT
  loopCheckPrinting();                //Check if there is a SD or USB print running.
#endif
  
#ifdef U_DISK_SUPPROT
  USBH_Process(&USB_OTG_Core, &USB_Host);
#endif

#if LCD_ENCODER_SUPPORT
  loopCheckMode();
#endif

#ifdef FIL_RUNOUT_PIN
  loopFILRunoutDetect();
#endif
}

void loopFrontEnd(void)
{
  loop_setup_config();
  loopVolumeSource();             // Check if volume source(SD/U disk) insert
  loop_reminder_failure_reset();  // If there is a message in the status bar, timed clear
  loop_reminder_success_reset();
  loop_reminder_process_reset();  // Busy Indicator clear
}

void loopProcess(void)
{
  temp_Change();
  loopBackEnd();
  loopFrontEnd();
}
