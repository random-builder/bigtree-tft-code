//
//
//

#include <HotbedLeveling.h>
#include "includes.h"

const MENUITEMS autoLevelingItems = {
// title
LABEL_AUTO_LEVEL,
// icon                        label
 {
  {ICON_BLTouchDeploy,        LABEL_BLTOUCH_DEPLOY}, //0
  {ICON_BLTouchStow,          LABEL_BLTOUCH_STOW},
  {ICON_BLTouchTest,          LABEL_BLTOUCH_TEST},
  {ICON_BLTouch,              LABEL_AUTO_PROBE},     // 3
  {ICON_BLTouchRepeat,        LABEL_BLTOUCH_REPEAT},
  {ICON_ProbeOffset,          LABEL_PROBE_OFFSET},
  {ICON_BabyStep,             LABEL_BABYSTEP},
  {ICON_Back,                 LABEL_BACK},}
};

void menuAutoLeveling(void)
{
  KEY_VALUES key_num=KEY_IDLE;
  menuDrawPage(&autoLevelingItems);
  SYSTEM_CONFIG config = config_instance();
  while(infoMenu.menu[infoMenu.cur] == menuAutoLeveling)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: // Drop
        config_issue_gcode(config.leveling_automatic_command_probe_drop);
        break;
      case KEY_ICON_1: // Stow
          config_issue_gcode(config.leveling_automatic_command_probe_stow);
        break;
      case KEY_ICON_2: // Test
          config_issue_gcode(config.leveling_automatic_command_probe_test);
        break;

      case KEY_ICON_3: // Levelingt
          config_issue_gcode(config.leveling_automatic_command_probe_invoke);
        break;

      case KEY_ICON_4: // TODO
          config_issue_gcode(config.leveling_automatic_command_probe_repeat);
        break;
      case KEY_ICON_5: // Offset Z
        infoMenu.menu[++infoMenu.cur] = menuProbeOffset;
        break;
      case KEY_ICON_6: // Baby Step
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
  {ICON_Point_4,               LABEL_POINT_4},    // 0
  {ICON_EMPTY,            LABEL_BACKGROUND},
  {ICON_Point_3,               LABEL_POINT_3},    // 2
  {ICON_Home,                  LABEL_HOME_ALL},   // 3
  {ICON_EMPTY,            LABEL_BACKGROUND},
  {ICON_Point_5,               LABEL_POINT_5},    // 5
  {ICON_EMPTY,            LABEL_BACKGROUND},
  {ICON_Back,                  LABEL_BACK},}      // 7
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
      case KEY_ICON_0: moveToLevelingPoint(3); break; // Point_4 Rear/Left
      case KEY_ICON_2: moveToLevelingPoint(2); break; // Point_3 Rear/Rite
      case KEY_ICON_5: moveToLevelingPoint(4); break; // Point_5 Frnt/Cntr

      case KEY_ICON_3: // Home All
          storeCmd("G28\n");
          break;

      case KEY_ICON_7:
        infoMenu.cur--; break;
      default:break;
    }
    loopProcess();
  }
}
