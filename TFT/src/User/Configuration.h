//
//
//

#pragma once

//#define SHOW_CONFIG_DEBUG
//#define SHOW_MESSAGE_VERIFY

//===========================================================================
//=========================== Marlin Mode Settings ===========================
//===========================================================================

/**
 * Default Marlin Mode Background & Font Color Options
 *
 * These colors can be changed in TFT mode, but can also be set here.
 *
 * Current color options from lcd.h: BLACK, BLUE, BROWN, BRRED, CYAN, GBLUE, GRAY, GREEN, MAGENTA, RED, WHITE, YELLOW
 */

// Marlin Mode Background & Font Color Options
// Current color options from lcd.h: BLACK, BLUE, BROWN, BRRED, CYAN, GBLUE, GRAY, GREEN, MAGENTA, RED, WHITE, YELLOW
#define ST7920_BKCOLOR BLACK
#define ST7920_FNCOLOR YELLOW

// Text displayed at the top of the LCD in Marlin Mode.
//#define ST7920_BANNER_TEXT "LCD12864 Simulator"

// Run Marlin Mode fullscreen. Not recommended for TFT24.
//#define ST7920_FULLSCREEN

/**
 * CLEAN MODE SWITCHING SUPPORT
 * Support for keeping UART(Serial communication) alive in Marlin Mode
 * Gives the ability to connect Octoprint over UART to the TFT UART/serial 
 * expansion port and have it work seamlessly no matter which mode the TFT is in. 
 * Only for TFT24 V1.1, TFT35 V3.0 or TFT28 V3.0
 */
//#define CLEAN_MODE_SWITCHING_SUPPORT  // Enable CLEAN MODE SWITCHING SUPPORT
//===========================================================================
//============================ TFT Mode Settings ============================
//===========================================================================
// Show BTT bootscreen when starting up
//#define SHOW_LOGO
// TFT mode color
#define TITLE_BACKGROUND_COLOR      BLACK  // Title background color // 0xD928
#define BACKGROUND_COLOR            BLACK  // Background color // 0x0A29
#define FONT_COLOR                  WHITE  // Font foreground color
#define REMINDER_FONT_COLOR         RED    // Reminder font color, such as: "No print attached", "Busy process", etc...
#define VOLUME_REMINDER_FONT_COLOR  GBLUE  // Volume reminder font color,such as: "Card inserted", "Card removed"

#define TOOL_NUM     1    // set in 1~6
#define EXTRUDER_NUM 1    // set in 1~6
#define FAN_NUM      1    // set in 1~6

#define HEAT_MAX_TEMP    {150,    275,       275,       275,       275,       275,       275}    //max temperature can be set
#define HEAT_SIGN_ID     {"B:",   "T0:",     "T1:",     "T2:",     "T3:",     "T4:",     "T5:"}
#define HEAT_DISPLAY_ID  {"Bed",  "T0",      "T1",      "T2",      "T3",      "T4",      "T5"}
#define HEAT_CMD         {"M140", "M104 T0", "M104 T1", "M104 T2", "M104 T3", "M104 T4", "M104 T5" };
#define HEAT_WAIT_CMD    {"M190", "M109 T0", "M109 T1", "M109 T2", "M109 T3", "M109 T4", "M109 T5" };

#define TOOL_CHANGE      {"T0",   "T1",      "T2",      "T3",      "T4",      "T5"}
#define EXTRUDER_ID      {"E0",   "E1",      "E2",      "E3",      "E4",      "E5"}

#define FAN_MAX_PWM      {255,       255,       255,       255,       255,       255}
#define FAN_ID           {"F0",    "F1",    "F2",    "F3",    "F4",    "F5"}
#define FAN_CMD          {"M106 P0", "M106 P1", "M106 P2", "M106 P3", "M106 P4", "M106 P5" };

#define SPEED_ID         {"Sp.", "Fr."}

// Default move speed mm/min
#define DEFAULT_SPEED_MOVE      3000
#define SPEED_MOVE_SLOW         1000
#define SPEED_MOVE_FAST         5000
// Extrude speed mm/min
#define EXTRUDE_SLOW_SPEED      60
#define EXTRUDE_NORMAL_SPEED    600
#define EXTRUDE_FAST_SPEED      1200

// Size of machine
#define X_MIN_POS 0
#define Y_MIN_POS 0
#define Z_MIN_POS 0
#define X_MAX_POS 235

#define Y_MAX_POS 235
#define Z_MAX_POS 250

// Specify a pause position as { X, Y, Z_raise }
#define NOZZLE_PAUSE_RETRACT_LENGTH 15   // (mm)
#define NOZZLE_RESUME_PURGE_LENGTH  16   // (mm)
#define NOZZLE_PAUSE_X_POSITION     (X_MIN_POS + 10)  // (mm) Must be an integer
#define NOZZLE_PAUSE_Y_POSITION     (Y_MIN_POS + 10)  // (mm) Must be an integer
#define NOZZLE_PAUSE_Z_RAISE        20   // (mm)
#define NOZZLE_PAUSE_E_FEEDRATE     6000 // (mm/min) retract & purge feedrate
#define NOZZLE_PAUSE_XY_FEEDRATE    6000 // (mm/min) X and Y axes feedrate
#define NOZZLE_PAUSE_Z_FEEDRATE     600  // (mm/min) Z axis feedrate

// Send G29 for auto bed leveling
#define AUTO_BED_LEVELING
#ifdef AUTO_BED_LEVELING
// Enable this will send "M500" after "G29" to store leveling value
// and send "M420 S1" to enable leveling state after startup
#define AUTO_SAVE_LOAD_LEVELING_VALUE
#endif

//
// Tuning points for manual leveling (defaults for config.ini)
//

#define LEVELING_MARGIN            10

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

// Power Supply
#define PS_ON_ACTIVE_HIGH    true   // Set 'false' for ATX (1), 'true' for X-Box (2)

// Filament runout detection
#define FIL_RUNOUT_INVERTING true  // Set to false to invert the logic of the sensor.
#define FIL_NOISE_THRESHOLD  10     // 10*10 = 100ms,  Pause print when filament runout is detected for 100ms.

// Smart filament runout detection
// For use with an encoder disc that toggles runout pin as filament moves
#define FILAMENT_RUNOUT_DISTANCE_MM 7

// Enable alternative Move Menu Buttons Layout matching the direction of actual printer axis.
// update the icons from alternate icon folder
#define ALTERNATIVE_MOVE_MENU

// Invert the Y Axis move Direction
// this does not work if LIST MODE is enabled. To invert y axis in LIST MODE go to setting->feature settings
//#define INVERT_YAXIS

//Invert the Z Axis move Direction
// this does not work if LIST MODE is enabled. To invert z axis in LIST MODE go to setting->feature settings
//#define INVERT_ZAXIS

//-------RESET SETTINGS & TOUCH SCREEN CALIBRATION------||
// To reset the touch screen create a text file with name 'reset.txt' in root folder of the sd card and press reset button.

// SD support
#define ONBOARD_SD_SUPPORT
#ifdef ONBOARD_SD_SUPPORT
#define M27_AUTOREPORT                      // Disable M27 polling if you enable enable AUTO_REPORT_SD_STATUS in Marlin
#define M27_REFRESH                3        // Time in sec for M27 command
#define M27_WATCH_OTHER_SOURCES    true     // if true the polling on M27 report is always active. Case: SD print started not from TFT35
#endif

/**
 * Home before power loss recovery
 * Many printer will crash printed model when homing, which is not suitable for home before PLR.
 * This function is suitable for Delta Printer.
 */
//#define HOME_BEFORE_PLR
//#define BTT_MINI_UPS // Backup power / UPS to move the Z axis steppers on power loss
#define POWER_LOSS_ZRAISE 10 // (mm) Z axis raise on resume (on power loss with UPS)

// Prevent extrusion if the temperature is below set temperature
#define PREVENT_COLD_EXTRUSION_MINTEMP 170

/**
 * Maximum hotend temperature of automatic shut down after printing.
 * When enable automatic shutdown(Auto Power), when the hotend temperature is higher than this value
 * turn on the fan to cool down, wait for the hotend temperature to be lower than this value, then turn off the power automatically
 */
#define AUTO_SHUT_DOWN_MAXTEMP 50

#define EXTRUDE_STEPS  100.0f

/**
 * Rapid Serial Communication:More frequent Serial communicaiton while printing.
 * The controller will send and parse  gcodes more frequently  while drawing on 
 * screen to prevent printer idling and stuttering  due to empty printer buffer.
 * Note: this might make the  graphics slow when switching menus while printing.
 */
#define RAPID_SERIAL_COMM

/**
 * Enable list mode in Custom G-Code menu
 * 7 codes in icon mode, 15 items in list mode
 */
#define CUSTOM_GCODE_LIST_MODE

/**
 * CUSTOM GCODE COMMANDS (defaults for config.ini)
 * CUSTOM_X_LABEL is the name of the custom button, CUSTOM_X_GCODE
 * CUSTOM_X_GCODE is the gcode to be sent by the custom button, separator: '\n'
 */
//
#define CUSTOM_ENTRY_COUNT 20
//
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

#define CANCEL_PRINT_GCODE "G28 X0 Y0\n"

// user override
#include "Configuration_user.h"
