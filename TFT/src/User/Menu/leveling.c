#include "leveling.h"
#include "includes.h"

const MENUITEMS autoLevelingItems = {
// title
LABEL_AUTO_LEVEL,
// icon                        label
 {{ICON_LEVELING,              LABEL_AUTO_LEVEL},
  {ICON_BLTOUCH_DEPLOY,        LABEL_BLTOUCH_DEPLOY},
  {ICON_BLTOUCH_STOW,          LABEL_BLTOUCH_STOW},
  {ICON_BLTOUCH_TEST,          LABEL_BLTOUCH_TEST},
  {ICON_BLTOUCH_REPEAT,        LABEL_BLTOUCH_REPEAT},
  {ICON_PROBE_OFFSET,          LABEL_PROBE_OFFSET},
  {ICON_BABYSTEP,              LABEL_BABYSTEP},
  {ICON_BACK,                  LABEL_BACK},}
};

void menuAutoLeveling(void)
{
  KEY_VALUES key_num=KEY_IDLE;
  menuDrawPage(&autoLevelingItems);
  while(infoMenu.menu[infoMenu.cur] == menuAutoLeveling)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
        storeCmd("G28\n"); // home/reset
        storeCmd("G29 P1\n"); // measure
        storeCmd("G29 P3.1\n"); // populate
        storeCmd("G29 P5 C0.75\n"); // adjust
        storeCmd("G29 T\n"); // report
        storeCmd("G29 A\n"); // activate
        #ifdef AUTO_SAVE_LOAD_LEVELING_VALUE
          storeCmd("M500\n");
        #endif
        break;
      case KEY_ICON_1:
        storeCmd("M280 P0 S10\n");
        break;
      case KEY_ICON_2:
        storeCmd("M280 P0 S90\n");
        break;
      case KEY_ICON_3:
        storeCmd("M280 P0 S120\n");
        break;
      case KEY_ICON_4:
        storeCmd("M48\n");
        break;
      case KEY_ICON_5:
        storeCmd("M851\n");
        infoMenu.menu[++infoMenu.cur] = menuProbeOffset;
        break;      
      case KEY_ICON_6:
        infoMenu.menu[++infoMenu.cur] = menuBabyStep;
        break; 
      case KEY_ICON_7:
        infoMenu.cur--; break;
      default:break;
    }
    loopProcess();
  }
}


const MENUITEMS manualLevelingItems = {
// title
LABEL_HAND_LEVEL,
// icon                        label
 {
  {ICON_POINT_4,               LABEL_POINT_4},
  {ICON_BACKGROUND,            LABEL_BACKGROUND}, // {ICON_POINT_2,               LABEL_POINT_2},
  {ICON_POINT_3,               LABEL_POINT_3},
  {ICON_BACKGROUND,            LABEL_BACKGROUND}, // {ICON_POINT_4,               LABEL_POINT_4},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_POINT_5,               LABEL_POINT_5},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACK,                  LABEL_BACK},}
};

void moveToLevelingPoint(u8 point)
{
  static const s16 pointPosition[][2] = {
    {LEVELING_POINT_1_X, LEVELING_POINT_1_Y},
    {LEVELING_POINT_2_X, LEVELING_POINT_2_Y},
    {LEVELING_POINT_3_X, LEVELING_POINT_3_Y},
    {LEVELING_POINT_4_X, LEVELING_POINT_4_Y},
    {LEVELING_POINT_5_X, LEVELING_POINT_5_Y},
  };
  if(coordinateIsClear() == false)
  {
    storeCmd("G28\n");
  }
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_MOVE_Z, LEVELING_POINT_Z_FEEDRATE);
  storeCmd("G0 X%d Y%d F%d\n", pointPosition[point][0], pointPosition[point][1], LEVELING_POINT_XY_FEEDRATE);
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_Z, LEVELING_POINT_Z_FEEDRATE);
}

void menuManualLeveling(void)
{
  KEY_VALUES key_num=KEY_IDLE;
  menuDrawPage(&manualLevelingItems);
  while(infoMenu.menu[infoMenu.cur] == menuManualLeveling)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: moveToLevelingPoint(0); break; // Point_1
      case KEY_ICON_1: moveToLevelingPoint(1); break; // Point_2
      case KEY_ICON_2: moveToLevelingPoint(2); break; // Point_3
      case KEY_ICON_3: moveToLevelingPoint(3); break; // Point_4
      case KEY_ICON_4: moveToLevelingPoint(4); break; // Point_5
      case KEY_ICON_7:
        infoMenu.cur--; break;
      default:break;
    }
    loopProcess();
  }
}
