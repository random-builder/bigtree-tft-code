#include <HeaterControl.h>
#include "includes.h"

//1 title, ITEM_PER_PAGE items (icon + label) 
MENUITEMS heatItems = {
// title
LABEL_HEAT_CTL,
// icon                       label
 {
  {ICON_Dec,                  LABEL_DEC},
  {ICON_EMPTY,           LABEL_BACKGROUND},
  {ICON_Inc,                  LABEL_INC},
  {ICON_EMPTY,           LABEL_BACKGROUND},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Temp_5,             LABEL_5_DEGREE},
  {ICON_Stop,                 LABEL_ISSUE_RESET},
  {ICON_Back,                 LABEL_BACK},}
};


const ITEM itemTool[] = {
// icon                       label
  {ICON_Hotbed,                  LABEL_BED},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
  {ICON_Nozzle,               LABEL_NOZZLE},
};    

#define ITEM_DEGREE_NUM 3
const ITEM itemDegree[ITEM_DEGREE_NUM] = {
// icon                       label
  {ICON_Temp_1,             LABEL_1_DEGREE},
  {ICON_Temp_5,             LABEL_5_DEGREE},
  {ICON_Temp_10,            LABEL_10_DEGREE},
};
const  u8 item_degree[ITEM_DEGREE_NUM] = {1, 5, 10};
static u8 item_degree_i = 1;

const u16   heat_max_temp[] = HEAT_MAX_TEMP;
const char* toolID[] = HEAT_SIGN_ID;
const char* const heatDisplayID[] = HEAT_DISPLAY_ID;
const char* heatCmd[] = HEAT_CMD;
const char* heatWaitCmd[] = HEAT_WAIT_CMD;

static HEATER  heater = {{}, TOOL_NOZZLE0, TOOL_NOZZLE0};
static HEATER  heater_past = {{}, TOOL_NOZZLE0, TOOL_NOZZLE0};
static u32     update_time = 300;
static bool    update_waiting = false;
static bool    send_waiting[HEATER_NUM];

/*Set target temperature*/
void heatSetTargetTemp(TOOL tool, u16 temp)
{
  heater.T[tool].target = temp;
}
/*Sync target temperature*/
void heatSyncTargetTemp(TOOL tool, u16 temp)
{
  heater_past.T[tool].target = heater.T[tool].target = temp;
}

/*Get target temperature */
u16 heatGetTargetTemp(TOOL tool)
{
  return heater.T[tool].target;
}

/* Set current temperature */
void heatSetCurrentTemp(TOOL tool, s16 temp)
{
  heater.T[tool].current = limitValue(-99, temp, 999);
}

/* Get current temperature */
s16 heatGetCurrentTemp(TOOL tool)
{
  return heater.T[tool].current;
}

/* Is heating waiting to heat up */
bool heatGetIsWaiting(TOOL tool)
{
  return heater.T[tool].waiting;
}

/* Check all heater if there is a heater waiting to be waited */
bool heatHasWaiting(void)
{
  TOOL i;
  for(i = TOOL_HOTBED; i < HEATER_NUM; i++)
  {
    if(heater.T[i].waiting == true)
    return true;
  }
  return false;
}

/* Set heater waiting status */
void heatSetIsWaiting(TOOL tool, bool isWaiting)
{
  heater.T[tool].waiting = isWaiting;
  if(isWaiting == true)
  {
    update_time = 100;
  }
  else if(heatHasWaiting() == false)
  {
    update_time = 300;		
  }
}

void heatClearIsWaiting(void)
{
  for(TOOL i = TOOL_HOTBED; i < HEATER_NUM; i++)
  {
    heater.T[i].waiting = false;
  }
  update_time = 300;
}

/* Set current heater tool, nozzle or hot bed */
void heatSetCurrentTool(TOOL tool)
{
  if(tool >= HEATER_NUM) return;
  heater.tool = tool;
  heater_past.tool = tool;
  heatItems.items[KEY_ICON_4] = itemTool[heater.tool];
}

/* Get current tool, nozzle or hot bed */
TOOL heatGetCurrentTool(void)
{
  return heater.tool;
}

/* Set current nozzle */
void heatSetCurrentToolNozzle(TOOL tool)
{
  if(tool >= HEATER_NUM && tool < TOOL_NOZZLE0) return;
  heater.nozzle = tool;
  heater.tool = tool;
}

/* Get current nozzle*/
TOOL heatGetCurrentToolNozzle(void)
{
  return heater.nozzle;
}

/* Set temperature update time interval */
void heatSetUpdateTime(u32 time)
{
  update_time=time;
}

/* Set whether we need to query the current temperature */
void heatSetUpdateWaiting(bool isWaiting)
{
  update_waiting = isWaiting;
}

/* Set whether the heating command has been sent */
void heatSetSendWaiting(TOOL tool, bool isWaiting)
{
  send_waiting[tool] = isWaiting;
}

void showTemperature(void)
{
  
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y-BYTE_HEIGHT, exhibitRect.x1, CENTER_Y};
  GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
  GUI_DispStringInPrect(&rect, (u8*)heatDisplayID[heater.tool]);
  GUI_DispDec(CENTER_X-BYTE_WIDTH*3, CENTER_Y, heater.T[heater.tool].current, 3, RIGHT);
  GUI_DispString(CENTER_X, CENTER_Y, (u8*)"/");
  GUI_DispDec(CENTER_X+BYTE_WIDTH*1, CENTER_Y, heater.T[heater.tool].target, 3, LEFT);
}

void currentReDraw(void)
{
  GUI_DispDec(CENTER_X-BYTE_WIDTH*3, CENTER_Y, heater.T[heater.tool].current, 3, RIGHT);
}

void targetReDraw(void)
{
  GUI_DispDec(CENTER_X+BYTE_WIDTH*1, CENTER_Y, heater.T[heater.tool].target, 3, LEFT);
}

void menuHeaterControl(void)
{
  KEY_VALUES  key_num = KEY_IDLE;

  heater_past = heater;
  update_time = 100;

  menuDrawPage(&heatItems);
  showTemperature();

  while(infoMenu.menu[infoMenu.cur] == menuHeaterControl)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: // Decrease
        if(heater.T[heater.tool].target > 0)
        {
          heater.T[heater.tool].target = 
            limitValue( 0, 
                        heater.T[heater.tool].target - item_degree[item_degree_i], 
                        heat_max_temp[heater.tool]);
        }
        break;
      
      case KEY_ICON_2: // Increase
        if(heater.T[heater.tool].target < heat_max_temp[heater.tool])
        {
          heater.T[heater.tool].target = 
            limitValue( 0, 
                        heater.T[heater.tool].target + item_degree[item_degree_i], 
                        heat_max_temp[heater.tool]);
        }
        break;
        
      case KEY_ICON_4: // Select Tool
        heater_past.tool = heater.tool = (TOOL)((heater.tool+1) % HEATER_NUM);
        heatItems.items[key_num] = itemTool[heater.tool];
        menuDrawItem(&heatItems.items[key_num], key_num);
        showTemperature();
        break;
      
      case KEY_ICON_5: // Delta
        item_degree_i = (item_degree_i+1) % ITEM_DEGREE_NUM;
        heatItems.items[key_num] = itemDegree[item_degree_i];
        menuDrawItem(&heatItems.items[key_num], key_num);
        break;
      
      case KEY_ICON_6: // Stop
        heater.T[heater.tool].target = 0;
        break;
      
      case KEY_ICON_7: // Back
        infoMenu.cur--;
        break;
      
      default :
        break;
    }

    if(heater_past.tool != heater.tool)
    {
      heater_past.tool = heater.tool;
      showTemperature();
    }
    if(heater_past.T[heater.tool].current != heater.T[heater.tool].current)
    {      
      heater_past.T[heater.tool].current = heater.T[heater.tool].current;
      currentReDraw();
    }
    if(heater_past.T[heater.tool].target != heater.T[heater.tool].target)
    {
      targetReDraw();
    }
    
    loopProcess();
  }
  
  if(heatHasWaiting()==false)
    update_time=300;
}


void loopCheckHeater(void)
{
  u8 i;
  static u32  nowTime=0;

  do
  {  /* Send M105 query temperature continuously	*/
    if(update_waiting == true)                {nowTime=OS_GetTime();break;}
    if(OS_GetTime()<nowTime+update_time)       break;
    if(RequestCommandInfoIsRunning())          break; //to avoid colision in Gcode response processing
    if(storeCmd("M105\n")==false)              break;

    nowTime=OS_GetTime();
    update_waiting=true;
  }while(0);

  /* Query the heater that needs to wait for the temperature to rise, whether it reaches the set temperature */
  for(i=0; i<HEATER_NUM; i++)
  {
    if (heater.T[i].waiting == false)                                   continue;
    if (i==TOOL_HOTBED)
    {
      if (heater.T[TOOL_HOTBED].current+2 <= heater.T[TOOL_HOTBED].target)              continue;
    }
    else
    {
      if (inRange(heater.T[i].current, heater.T[i].target, 2) != true)  continue;
    }

    heater.T[i].waiting = false;
    if(heatHasWaiting() == true)                                        continue;

    if(infoMenu.menu[infoMenu.cur] == menuHeaterControl)                         break;
    update_time=300;
  }
    
  for(TOOL i = TOOL_HOTBED; i < HEATER_NUM; i++) // If the target temperature changes, send a Gcode to set the motherboard
  {
    if(heater_past.T[i].target != heater.T[i].target)
    {
      heater_past.T[i].target = heater.T[i].target;
      if(send_waiting[i] != true)
      {
        send_waiting[i] = true;
        storeCmd("%s ",heatCmd[i]);
      }
    }
  }
}





