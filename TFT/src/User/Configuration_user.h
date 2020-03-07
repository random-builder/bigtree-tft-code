//
//
//

#pragma once

//#define SHOW_CONFIG_DEBUG
//#define SHOW_MESSAGE_VERIFY

#undef  SHOW_BTT_BOOTSCREEN

#undef  AUTO_SAVE_LOAD_LEVELING_VALUE

#undef  BAUDRATE
#define BAUDRATE 250000

#undef  DEFAULT_LANGUAGE
#define DEFAULT_LANGUAGE LANG_ENGLISH

//
//
//

#define HEATER_COUNT 7 // limit heater tools: 1 hotbed + 6 nozzle

// effectively discard these parameters
#undef  TOOL_NUM
#undef  EXTRUDER_NUM
#define EXTRUDER_NUM HEATER_COUNT

#undef  X_MAX_POS
#undef  Y_MAX_POS
#undef  Z_MAX_POS
#define X_MAX_POS 200
#define Y_MAX_POS 200
#define Z_MAX_POS 170

#undef  SPEED_ID
#define SPEED_ID         {"M/S", "F/R"}

#undef  HEAT_MAX_TEMP
#define HEAT_MAX_TEMP    {150,    350,       350,       350,       350,       350,       350}

//
// Tuning points for manual leveling (defaults for config.ini)
//

#define LEVELING_MARGIN            10

// front/left
#undef  LEVELING_POINT_1_X         //(X_MIN_POS + LEVELING_MARGIN)
#undef  LEVELING_POINT_1_Y         //(Y_MIN_POS + LEVELING_MARGIN)
#undef  LEVELING_POINT_1_ICON      //"Point_1" // icon-name
#undef  LEVELING_POINT_1_LABEL     //"1" // label-code
// front/rite
#undef  LEVELING_POINT_2_X         //(X_MAX_POS - LEVELING_MARGIN)
#undef  LEVELING_POINT_2_Y         //(Y_MIN_POS + LEVELING_MARGIN)
#undef  LEVELING_POINT_2_ICON      //"Point_2" // icon-name
#undef  LEVELING_POINT_2_LABEL     //"2" // label-code
// rear/rite
#undef  LEVELING_POINT_3_X         //(X_MAX_POS - LEVELING_MARGIN)
#undef  LEVELING_POINT_3_Y         //(Y_MAX_POS - LEVELING_MARGIN)
#undef  LEVELING_POINT_3_ICON      //"Point_3" // icon-name
#undef  LEVELING_POINT_3_LABEL     //"3" // label-code
// rear/left
#undef  LEVELING_POINT_4_X         //(X_MIN_POS + LEVELING_MARGIN)
#undef  LEVELING_POINT_4_Y         //(Y_MAX_POS - LEVELING_MARGIN)
#undef  LEVELING_POINT_4_ICON      //"Point_4" // icon-name
#undef  LEVELING_POINT_4_LABEL     //"4" // label-code
// front/center
#undef  LEVELING_POINT_5_X         //(X_MIN_POS + X_MAX_POS)/2
#undef  LEVELING_POINT_5_Y         //(Y_MIN_POS + LEVELING_MARGIN)
#undef  LEVELING_POINT_5_ICON      //"Point_5" // icon-name
#undef  LEVELING_POINT_5_LABEL     //"5" // label-code

// front/left
#define LEVELING_POINT_1_X         (X_MIN_POS + LEVELING_MARGIN)
#define LEVELING_POINT_1_Y         (Y_MIN_POS + LEVELING_MARGIN)
#define LEVELING_POINT_1_ICON      "Point_1" // icon-name
#define LEVELING_POINT_1_LABEL     "1" // label-code
// front/rite
#define LEVELING_POINT_2_X         (X_MAX_POS - LEVELING_MARGIN)
#define LEVELING_POINT_2_Y         (Y_MIN_POS + LEVELING_MARGIN)
#define LEVELING_POINT_2_ICON      "Point_2" // icon-name
#define LEVELING_POINT_2_LABEL     "2" // label-code
// rear/rite
#define LEVELING_POINT_3_X         (X_MAX_POS - LEVELING_MARGIN)
#define LEVELING_POINT_3_Y         (Y_MAX_POS - LEVELING_MARGIN)
#define LEVELING_POINT_3_ICON      "Point_3" // icon-name
#define LEVELING_POINT_3_LABEL     "3" // label-code
// rear/left
#define LEVELING_POINT_4_X         (X_MIN_POS + LEVELING_MARGIN)
#define LEVELING_POINT_4_Y         (Y_MAX_POS - LEVELING_MARGIN)
#define LEVELING_POINT_4_ICON      "Point_4" // icon-name
#define LEVELING_POINT_4_LABEL     "4" // label-code
// front/center
#define LEVELING_POINT_5_X         (X_MIN_POS + X_MAX_POS)/2
#define LEVELING_POINT_5_Y         (Y_MIN_POS + LEVELING_MARGIN)
#define LEVELING_POINT_5_ICON      "Point_5" // icon-name
#define LEVELING_POINT_5_LABEL     "5" // label-code

#define LEVELING_POINT_Z_UPPER      0.2   // Z-axis position when nozzle stays for leveling
#define LEVELING_POINT_Z_LOWER      9.0   // Z-axis position when nozzle move to next point

#define LEVELING_FEEDRATE_XY        6000  // (mm/min) X and Y axes move feedrate
#define LEVELING_FEEDRATE_ZZ        600   // (mm/min) Z axis move feedrate

//
#define CUSTOM_ENTRY_COUNT 20
//

#undef  CUSTOM_0_LABEL //"Restore EEPROM"
#undef  CUSTOM_0_GCODE //"M501\n"
#undef  CUSTOM_1_LABEL //"Disable Steppers"
#undef  CUSTOM_1_GCODE //"M84\n"
#undef  CUSTOM_2_LABEL //"Init SD Card"
#undef  CUSTOM_2_GCODE //"M21\n"
#undef  CUSTOM_3_LABEL //"Release Sd Card"
#undef  CUSTOM_3_GCODE //"M22\n"
#undef  CUSTOM_4_LABEL //"Custom4"
#undef  CUSTOM_4_GCODE //"M105\n"
#undef  CUSTOM_5_LABEL //"Custom5"
#undef  CUSTOM_5_GCODE //"M105\n"
#undef  CUSTOM_6_LABEL //"Custom6"
#undef  CUSTOM_6_GCODE //"M105\n"
#undef  CUSTOM_7_LABEL //"Custom7"
#undef  CUSTOM_7_GCODE //"M105\n"
#undef  CUSTOM_8_LABEL //"Custom8"
#undef  CUSTOM_8_GCODE //"M105\n"
#undef  CUSTOM_9_LABEL //"Custom9"
#undef  CUSTOM_9_GCODE //"M105\n"
#undef  CUSTOM_10_LABEL //"Custom10"
#undef  CUSTOM_10_GCODE //"M105\n"
#undef  CUSTOM_11_LABEL //"Custom11"
#undef  CUSTOM_11_GCODE //"M105\n"
#undef  CUSTOM_12_LABEL //"Custom12"
#undef  CUSTOM_12_GCODE //"M105\n"
#undef  CUSTOM_13_LABEL //"Custom13"
#undef  CUSTOM_13_GCODE //"M105\n"
#undef  CUSTOM_14_LABEL //"Custom14"
#undef  CUSTOM_14_GCODE //"M105\n"
#undef  CUSTOM_15_LABEL //"Custom15"
#undef  CUSTOM_15_GCODE //"M105\n"
#undef  CUSTOM_16_LABEL //"Custom16"
#undef  CUSTOM_16_GCODE //"M105\n"
#undef  CUSTOM_17_LABEL //"Custom17"
#undef  CUSTOM_17_GCODE //"M105\n"
#undef  CUSTOM_18_LABEL //"Custom18"
#undef  CUSTOM_18_GCODE //"M105\n"
#undef  CUSTOM_19_LABEL //"Custom19"
#undef  CUSTOM_19_GCODE //"M105\n"

#define CUSTOM_0_LABEL "Restore EEPROM"
#define CUSTOM_0_GCODE "M501\n"
#define CUSTOM_1_LABEL "Disable Steppers"
#define CUSTOM_1_GCODE "M84\n"
#define CUSTOM_2_LABEL "Init SD Card"
#define CUSTOM_2_GCODE "M21\n"
#define CUSTOM_3_LABEL "Release Sd Card"
#define CUSTOM_3_GCODE "M22\n"
#define CUSTOM_4_LABEL "Custom4"
#define CUSTOM_4_GCODE "M105\n"
#define CUSTOM_5_LABEL "Custom5"
#define CUSTOM_5_GCODE "M105\n"
#define CUSTOM_6_LABEL "Custom6"
#define CUSTOM_6_GCODE "M105\n"
#define CUSTOM_7_LABEL "Custom7"
#define CUSTOM_7_GCODE "M105\n"
#define CUSTOM_8_LABEL "Custom8"
#define CUSTOM_8_GCODE "M105\n"
#define CUSTOM_9_LABEL "Custom9"
#define CUSTOM_9_GCODE "M105\n"
#define CUSTOM_10_LABEL "Custom10"
#define CUSTOM_10_GCODE "M105\n"
#define CUSTOM_11_LABEL "Custom11"
#define CUSTOM_11_GCODE "M105\n"
#define CUSTOM_12_LABEL "Custom12"
#define CUSTOM_12_GCODE "M105\n"
#define CUSTOM_13_LABEL "Custom13"
#define CUSTOM_13_GCODE "M105\n"
#define CUSTOM_14_LABEL "Custom14"
#define CUSTOM_14_GCODE "M105\n"
#define CUSTOM_15_LABEL "Custom15"
#define CUSTOM_15_GCODE "M105\n"
#define CUSTOM_16_LABEL "Custom16"
#define CUSTOM_16_GCODE "M105\n"
#define CUSTOM_17_LABEL "Custom17"
#define CUSTOM_17_GCODE "M105\n"
#define CUSTOM_18_LABEL "Custom18"
#define CUSTOM_18_GCODE "M105\n"
#define CUSTOM_19_LABEL "Custom19"
#define CUSTOM_19_GCODE "M105\n"

//
// no longer used
//

#undef PREHEAT_BED
#undef PREHEAT_HOTEND
#undef PREHEAT_TEXT

#undef HEAT_MAX_TEMP
#undef HEAT_SIGN_ID
#undef HEAT_DISPLAY_ID
#undef HEAT_CMD
#undef HEAT_WAIT_CMD
