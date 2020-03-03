//
// walk around coordinates
//

#include <PositionControl.h>
#include "includes.h"


//const GUI_RECT RecXYZ = {START_X + 1*ICON_WIDTH,        STATUS_GANTRY_YOFFSET,
//                         4*ICON_WIDTH+3*SPACE_X+START_X,ICON_START_Y-STATUS_GANTRY_YOFFSET};
#define YGCODE_INC "G1 Y%.1f\n"
#define YGCODE_DEC "G1 Y-%.1f\n"
#define ZGCODE_INC "G1 Z%.1f\n"
#define ZGCODE_DEC "G1 Z-%.1f\n"

//1 title, ITEM_PER_PAGE item
MENUITEMS moveItems = {
//  title
LABEL_MOVE,
// icon                       label
 { 
  #ifdef ALTERNATIVE_MOVE_MENU 
      {ICON_Dec_Z,                LABEL_Z_DEC},
      {ICON_Inc_Y,                LABEL_Y_INC},
      {ICON_Inc_Z,                LABEL_Z_INC},
      {ICON_Mmm_1,                LABEL_1_MM},
      {ICON_Dec_X,                LABEL_X_DEC},
      {ICON_Dec_Y,                LABEL_Y_DEC},
      {ICON_Inc_X,                LABEL_X_INC},
      {ICON_Back,                 LABEL_BACK},
  #else
    {ICON_Inc_X,                LABEL_X_INC},
    {ICON_Inc_Y,                LABEL_Y_INC},
    {ICON_Inc_Z,                LABEL_Z_INC},
    {ICON_Mmm_1,                LABEL_1_MM},
    {ICON_Dec_X,                LABEL_X_DEC},
    {ICON_Dec_Y,                LABEL_Y_DEC},
    {ICON_Dec_Z,                LABEL_Z_DEC},
    {ICON_Back,                 LABEL_BACK},
  #endif
 }
};


#define ITEM_MOVE_LEN_NUM 3
const ITEM itemMoveLen[ITEM_MOVE_LEN_NUM] = {
// icon                       label
  {ICON_Mmm_01,                LABEL_01_MM},
  {ICON_Mmm_1,                 LABEL_1_MM},
  {ICON_Mmm_10,                LABEL_10_MM},
};
const  float item_move_len[ITEM_MOVE_LEN_NUM] = {0.1f, 1, 10};
static u8    item_move_len_i = 1;

static u32 nowTime = 0;
static u32 update_time = 50; // 1 seconds is 100

void menuPosition(void)
{
  KEY_VALUES  key_num = KEY_IDLE;


  #ifdef ALTERNATIVE_MOVE_MENU
      if(infoSettings.invert_yaxis == 1){
        moveItems.items[1].label.index = LABEL_Y_DEC;
        moveItems.items[5].label.index = LABEL_Y_INC;
      }
      else{
        moveItems.items[1].label.index = LABEL_Y_INC;
        moveItems.items[5].label.index = LABEL_Y_DEC;
      }
        if(infoSettings.invert_zaxis == 1){
        moveItems.items[0].label.index = LABEL_Z_INC;
        moveItems.items[2].label.index = LABEL_Z_DEC;
      }
      else{
        moveItems.items[0].label.index = LABEL_Z_DEC;
        moveItems.items[2].label.index = LABEL_Z_INC;
      }
  #else
      if(infoSettings.invert_yaxis == 1){
        moveItems.items[1].label.index = LABEL_Y_DEC;
        moveItems.items[5].label.index = LABEL_Y_INC;
      }
      else{
        moveItems.items[1].label.index = LABEL_Y_INC;
        moveItems.items[5].label.index = LABEL_Y_DEC;
      }
        if(infoSettings.invert_zaxis == 1){
        moveItems.items[2].label.index = LABEL_Z_DEC;
        moveItems.items[6].label.index = LABEL_Z_INC;
      }
      else{
        moveItems.items[2].label.index = LABEL_Z_INC;
        moveItems.items[6].label.index = LABEL_Z_DEC;
      }
  #endif

  menuDrawPage(&moveItems);
  mustStoreCmd("G91\n");

    switch (infoSettings.move_speed)
    {
    case 1:
      mustStoreCmd("G1 F%d\n",SPEED_MOVE_SLOW);
      break;
    case 2:
      mustStoreCmd("G1 F%d\n",SPEED_MOVE_FAST);
      break;
    default:
      mustStoreCmd("G1 F%d\n",DEFAULT_SPEED_MOVE);
      break;
    }
  mustStoreCmd("M114\n");
  render_position();

  while(infoMenu.menu[infoMenu.cur] == menuPosition)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      #ifdef ALTERNATIVE_MOVE_MENU
          
            case KEY_ICON_0:
                    if(infoSettings.invert_zaxis == 1){
                      storeCmd(ZGCODE_INC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(ZGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_1:
                    if(infoSettings.invert_yaxis == 1){
                      storeCmd(YGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(YGCODE_INC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_2: 
                    if(infoSettings.invert_zaxis == 1){
                      storeCmd(ZGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(ZGCODE_INC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_3: 
                  item_move_len_i = (item_move_len_i+1)%ITEM_MOVE_LEN_NUM;            
                  moveItems.items[key_num] = itemMoveLen[item_move_len_i];
                  menuDrawItem(&moveItems.items[key_num], key_num);
                  break;

            case KEY_ICON_4: storeCmd("G1 X-%.1f\n", item_move_len[item_move_len_i]);  break;

            case KEY_ICON_5:
                    if(infoSettings.invert_yaxis == 1){
                      storeCmd(YGCODE_INC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(YGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_6: storeCmd("G1 X%.1f\n",  item_move_len[item_move_len_i]);  break;
            case KEY_ICON_7: infoMenu.cur--; break;
            default:break; 
        
      #else

            case KEY_ICON_0: storeCmd("G1 X%.1f\n",  item_move_len[item_move_len_i]);  break;

            case KEY_ICON_1:
                    if(infoSettings.invert_yaxis == 1){
                      storeCmd(YGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(YGCODE_INC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_2:
                    if(infoSettings.invert_zaxis == 1){
                      storeCmd(ZGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(ZGCODE_INC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_3: 
                  item_move_len_i = (item_move_len_i+1)%ITEM_MOVE_LEN_NUM;            
                  moveItems.items[key_num] = itemMoveLen[item_move_len_i];
                  menuDrawItem(&moveItems.items[key_num], key_num);
                  break;

            case KEY_ICON_4: storeCmd("G1 X-%.1f\n", item_move_len[item_move_len_i]);  break;

            case KEY_ICON_5:
                    if(infoSettings.invert_yaxis == 1){
                      storeCmd(YGCODE_INC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(YGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_6: 
                    if(infoSettings.invert_zaxis == 1){
                      storeCmd(ZGCODE_INC, item_move_len[item_move_len_i]);
                    }
                    else{
                      storeCmd(ZGCODE_DEC, item_move_len[item_move_len_i]);
                    }
                  break;

            case KEY_ICON_7: infoMenu.cur--; break;
            default:break; 
      #endif    
    }
    loopProcess();
    update_position();
  }
  mustStoreCmd("G90\n");
}

void update_position(void)
{
  if (OS_GetTime() > nowTime + update_time)
  {
    if (infoHost.connected == true && infoHost.wait == false){
      storeCmd("M114\n");
    }
    render_position();
    nowTime = OS_GetTime();
  }
}
void render_position(void){
  char tempstr[100];
  //GUI_SetColor(GANTRYLBL_BKCOLOR);
  //GUI_FillPrect(&RecXYZ);
  my_sprintf(tempstr, "X:%.1f  ", getAxisLocation(0));  
  GUI_DispString(START_X+1*SPACE_X+1*ICON_WIDTH,(ICON_START_Y-BYTE_HEIGHT)/2,(u8 *)tempstr);
  my_sprintf(tempstr, "Y:%.1f  ", getAxisLocation(1));
  GUI_DispString(START_X+2*SPACE_X+2*ICON_WIDTH,(ICON_START_Y-BYTE_HEIGHT)/2,(u8 *)tempstr);
  my_sprintf(tempstr, "Z:%.1f  ", getAxisLocation(2));
  GUI_DispString(START_X+3*SPACE_X+3*ICON_WIDTH,(ICON_START_Y-BYTE_HEIGHT)/2,(u8 *)tempstr);
  
  //GUI_SetBkColor(BACKGROUND_COLOR);
  //GUI_SetColor(FONT_COLOR);
}
