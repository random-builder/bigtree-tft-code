//
// define config file structure and default values
//
// format:
// X_ENTRY(SECTION, NAME, DEFAULT_VALUE)
//
// usage:
// https://en.wikipedia.org/wiki/X_Macro
//
// note: config values can be only strings
//

//
// AnyMenu.c
//

// [display_behavior]

X_ENTRY(display_behavior, reminder_timeout, "2000")// millisecond

//
// HomingControl.c
//

// [homing_control]

X_ENTRY(homing_control, entry_1_use, "yes")
X_ENTRY(homing_control, entry_1_key, "0")
X_ENTRY(homing_control, entry_1_icon, "Home")
X_ENTRY(homing_control, entry_1_label, "1")// label-code or user-text
X_ENTRY(homing_control, entry_1_gcode, "G28\n")

X_ENTRY(homing_control, entry_2_use, "yes")
X_ENTRY(homing_control, entry_2_key, "1")
X_ENTRY(homing_control, entry_2_icon, "Home_X")
X_ENTRY(homing_control, entry_2_label, "2")// label-code or user-text
X_ENTRY(homing_control, entry_2_gcode, "G28 X")

X_ENTRY(homing_control, entry_3_use, "yes")
X_ENTRY(homing_control, entry_3_key, "2")
X_ENTRY(homing_control, entry_3_icon, "Home_Y")
X_ENTRY(homing_control, entry_3_label, "3")// label-code or user-text
X_ENTRY(homing_control, entry_3_gcode, "G28 Y")

X_ENTRY(homing_control, entry_4_use, "yes")
X_ENTRY(homing_control, entry_4_key, "3")
X_ENTRY(homing_control, entry_4_icon, "Home_Z")
X_ENTRY(homing_control, entry_4_label, "4")// label-code or user-text
X_ENTRY(homing_control, entry_4_gcode, "G28 Z")

X_ENTRY(homing_control, entry_5_use, "no")
X_ENTRY(homing_control, entry_5_key, "4")
X_ENTRY(homing_control, entry_5_icon, "Home")
X_ENTRY(homing_control, entry_5_label, "5")// label-code or user-text
X_ENTRY(homing_control, entry_5_gcode, "M105\n")

X_ENTRY(homing_control, entry_6_use, "no")
X_ENTRY(homing_control, entry_6_key, "5")
X_ENTRY(homing_control, entry_6_icon, "Home")
X_ENTRY(homing_control, entry_6_label, "6")// label-code or user-text
X_ENTRY(homing_control, entry_6_gcode, "M105\n")

X_ENTRY(homing_control, entry_7_use, "no")
X_ENTRY(homing_control, entry_7_key, "6")
X_ENTRY(homing_control, entry_7_icon, "Home")
X_ENTRY(homing_control, entry_7_label, "7")// label-code or user-text
X_ENTRY(homing_control, entry_7_gcode, "M105\n")

//
// PositionControl.c
//

// [position_control]

// general commands

// find head location
X_ENTRY(position_control, command_postion_query, "M114\n")// M114 - Get Current Position

// invoke before move
X_ENTRY(position_control, command_move_prefix, "G91\n")// G91 - Relative Positioning

// invoke after the move
X_ENTRY(position_control, command_move_suffix, "G90\n")// G90 - Absolute Positioning

// motion feed rate
X_ENTRY(position_control, command_feed_rate, "G1 F%d\n")// single parameter: navigation speed

// navigation speed

// "slow"
X_ENTRY(position_control, speed_1_rate, "1000")// mm/min
X_ENTRY(position_control, speed_1_icon, "Slow")// Slow.bmp
X_ENTRY(position_control, speed_1_label, "1")// label-code or user-text

// "norm"
X_ENTRY(position_control, speed_2_rate, "3000")// mm/min
X_ENTRY(position_control, speed_2_icon, "Normal")// Normal.bmp
X_ENTRY(position_control, speed_2_label, "2")// label-code or user-text

// "fast"
X_ENTRY(position_control, speed_3_rate, "5000")// mm/min
X_ENTRY(position_control, speed_3_icon, "Fast")// Fast.bmp
X_ENTRY(position_control, speed_3_label, "3")// label-code or user-text

// default menu layout entries

// [key=0 : Z-][key=1 : Y+][key=2 : Z+][key=3 : Step]
// [key=4 : X-][key=5 : Y-][key=6 : X+][key=7 : Back]

// Z-
X_ENTRY(position_control, entry_1_use, "yes")
X_ENTRY(position_control, entry_1_key, "0")
X_ENTRY(position_control, entry_1_icon, "Dec_Z")
X_ENTRY(position_control, entry_1_label, "1")// label-code or user-text
X_ENTRY(position_control, entry_1_gcode, "G1 Z%.1f\n")// single parameter
X_ENTRY(position_control, entry_1_shift, "Z-")// axis and direction
// Z+
X_ENTRY(position_control, entry_2_use, "yes")
X_ENTRY(position_control, entry_2_key, "2")
X_ENTRY(position_control, entry_2_icon, "Inc_Z")
X_ENTRY(position_control, entry_2_label, "3")// label-code or user-text
X_ENTRY(position_control, entry_2_gcode, "G1 Z%.1f\n")// single parameter: position
X_ENTRY(position_control, entry_2_shift, "Z+")// axis and direction

// Y+
X_ENTRY(position_control, entry_3_use, "yes")
X_ENTRY(position_control, entry_3_key, "1")
X_ENTRY(position_control, entry_3_icon, "Inc_Y")
X_ENTRY(position_control, entry_3_label, "2")// label-code or user-text
X_ENTRY(position_control, entry_3_gcode, "G1 Y%.1f\n")// single parameter: position
X_ENTRY(position_control, entry_3_shift, "Y+")// axis and direction
// Y-
X_ENTRY(position_control, entry_4_use, "yes")
X_ENTRY(position_control, entry_4_key, "5")
X_ENTRY(position_control, entry_4_icon, "Dec_Y")
X_ENTRY(position_control, entry_4_label, "6")// label-code or user-text
X_ENTRY(position_control, entry_4_gcode, "G1 Y%.1f\n")// single parameter: position
X_ENTRY(position_control, entry_4_shift, "Y-")// axis and direction

// X-
X_ENTRY(position_control, entry_5_use, "yes")
X_ENTRY(position_control, entry_5_key, "4")
X_ENTRY(position_control, entry_5_icon, "Dec_X")
X_ENTRY(position_control, entry_5_label, "5")// label-code or user-text
X_ENTRY(position_control, entry_5_gcode, "G1 X%.1f\n")// single parameter: position
X_ENTRY(position_control, entry_5_shift, "X-")// axis and direction
// X+
X_ENTRY(position_control, entry_6_use, "yes")
X_ENTRY(position_control, entry_6_key, "6")
X_ENTRY(position_control, entry_6_icon, "Inc_X")
X_ENTRY(position_control, entry_6_label, "7")// label-code or user-text
X_ENTRY(position_control, entry_6_gcode, "G1 X%.1f\n")// single parameter: position
X_ENTRY(position_control, entry_6_shift, "X+")// axis and direction

//
// HotbedLeveling.c
//

// [leveling_auto]

X_ENTRY(leveling_auto, command_probe_drop, "M280 P0 S10\n")
X_ENTRY(leveling_auto, command_probe_stow, "M280 P0 S90\n")
X_ENTRY(leveling_auto, command_probe_test, "M280 P0 S120\n")
X_ENTRY(leveling_auto, command_probe_reset, "M280 P0 S160\n")
X_ENTRY(leveling_auto, command_probe_invoke, "G28\n G29\n")

// [leveling_manual]

// note: icons in this section use icon names from TFT/src/User/API/UI/icon_list.inc

X_ENTRY(leveling_manual, command_reset_home, "G28\n")

X_ENTRY(leveling_manual, feed_rate_XY, STRINGIFY(LEVELING_FEEDRATE_XY))
X_ENTRY(leveling_manual, feed_rate_ZZ, STRINGIFY(LEVELING_FEEDRATE_ZZ))
X_ENTRY(leveling_manual, point_Z_upper, STRINGIFY(LEVELING_POINT_Z_UPPER))
X_ENTRY(leveling_manual, point_Z_lower, STRINGIFY(LEVELING_POINT_Z_LOWER))

// front/left
X_ENTRY(leveling_manual, point_1_key, "0")
X_ENTRY(leveling_manual, point_1_use, "yes")
X_ENTRY(leveling_manual, point_1_icon, LEVELING_POINT_1_ICON)
X_ENTRY(leveling_manual, point_1_label, LEVELING_POINT_1_LABEL)
X_ENTRY(leveling_manual, point_1_X, STRINGIFY(LEVELING_POINT_1_X))
X_ENTRY(leveling_manual, point_1_Y, STRINGIFY(LEVELING_POINT_1_Y))
// front/rite
X_ENTRY(leveling_manual, point_2_key, "1")
X_ENTRY(leveling_manual, point_2_use, "yes")
X_ENTRY(leveling_manual, point_2_icon, LEVELING_POINT_2_ICON)
X_ENTRY(leveling_manual, point_2_label, LEVELING_POINT_2_LABEL)
X_ENTRY(leveling_manual, point_2_X, STRINGIFY(LEVELING_POINT_2_X))
X_ENTRY(leveling_manual, point_2_Y, STRINGIFY(LEVELING_POINT_2_Y))
// rear/left
X_ENTRY(leveling_manual, point_3_key, "2")
X_ENTRY(leveling_manual, point_3_use, "yes")
X_ENTRY(leveling_manual, point_3_icon, LEVELING_POINT_3_ICON)
X_ENTRY(leveling_manual, point_3_label, LEVELING_POINT_3_LABEL)
X_ENTRY(leveling_manual, point_3_X, STRINGIFY(LEVELING_POINT_3_X))
X_ENTRY(leveling_manual, point_3_Y, STRINGIFY(LEVELING_POINT_3_Y))
// rear/rite
X_ENTRY(leveling_manual, point_4_key, "4")
X_ENTRY(leveling_manual, point_4_use, "yes")
X_ENTRY(leveling_manual, point_4_icon, LEVELING_POINT_4_ICON)
X_ENTRY(leveling_manual, point_4_label, LEVELING_POINT_4_LABEL)
X_ENTRY(leveling_manual, point_4_X, STRINGIFY(LEVELING_POINT_4_X))
X_ENTRY(leveling_manual, point_4_Y, STRINGIFY(LEVELING_POINT_4_Y))
// front/center
X_ENTRY(leveling_manual, point_5_key, "5")
X_ENTRY(leveling_manual, point_5_use, "yes")
X_ENTRY(leveling_manual, point_5_icon, LEVELING_POINT_5_ICON)
X_ENTRY(leveling_manual, point_5_label, LEVELING_POINT_5_LABEL)
X_ENTRY(leveling_manual, point_5_X, STRINGIFY(LEVELING_POINT_5_X))
X_ENTRY(leveling_manual, point_5_Y, STRINGIFY(LEVELING_POINT_5_Y))

//
// MachineSettings.c
//

// [light_control]

// note: icons in this section use icon names from TFT/src/User/API/UI/icon_list.inc

// level 1
X_ENTRY(light_control, level_1_key, "0")
X_ENTRY(light_control, level_1_use, "yes")
X_ENTRY(light_control, level_1_icon, "RGB_Red")
X_ENTRY(light_control, level_1_label, "1")// label-code or user-text
X_ENTRY(light_control, level_1_gcode, "M150 R255 U0 B0 P255\n")
// level 2
X_ENTRY(light_control, level_2_key, "1")
X_ENTRY(light_control, level_2_use, "yes")
X_ENTRY(light_control, level_2_icon, "RGB_Green")
X_ENTRY(light_control, level_2_label, "2")// label-code or user-text
X_ENTRY(light_control, level_2_gcode, "M150 R0 U255 B0 P255\n")
// level 3
X_ENTRY(light_control, level_3_key, "2")
X_ENTRY(light_control, level_3_use, "yes")
X_ENTRY(light_control, level_3_icon, "RGB_Blue")
X_ENTRY(light_control, level_3_label, "3")// label-code or user-text
X_ENTRY(light_control, level_3_gcode, "M150 R0 U0 B255 P255\n")
// level 4
X_ENTRY(light_control, level_4_key, "3")
X_ENTRY(light_control, level_4_use, "yes")
X_ENTRY(light_control, level_4_icon, "RGB_White")
X_ENTRY(light_control, level_4_label, "4")// label-code or user-text
X_ENTRY(light_control, level_4_gcode, "M150 R255 U255 B255 P255\n")
// level 5
X_ENTRY(light_control, level_5_key, "4")
X_ENTRY(light_control, level_5_use, "no")
X_ENTRY(light_control, level_5_icon, "")
X_ENTRY(light_control, level_5_label, "5")// label-code or user-text
X_ENTRY(light_control, level_5_gcode, "")
// level 6
X_ENTRY(light_control, level_6_key, "5")
X_ENTRY(light_control, level_6_use, "no")
X_ENTRY(light_control, level_6_icon, "")
X_ENTRY(light_control, level_6_label, "6")// label-code or user-text
X_ENTRY(light_control, level_6_gcode, "")
// level 7
X_ENTRY(light_control, level_7_key, "6")
X_ENTRY(light_control, level_7_use, "yes")
X_ENTRY(light_control, level_7_icon, "RGB_Off")
X_ENTRY(light_control, level_7_label, "7")// label-code or user-text
X_ENTRY(light_control, level_7_gcode, "M150 R0 U0 B0 P0\n")

//
// CustomCommand.c
//

// [custom_command]

// note: icons in ths section use symbol names from TFT/src/User/API/UI/symbol.inc

// custom 1
X_ENTRY(custom_command, entry_1_use, "yes")
X_ENTRY(custom_command, entry_1_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_1_label, "Custom 1")
X_ENTRY(custom_command, entry_1_gcode, "M105\n")
// custom 2
X_ENTRY(custom_command, entry_2_use, "yes")
X_ENTRY(custom_command, entry_2_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_2_label, "Custom 2")
X_ENTRY(custom_command, entry_2_gcode, "M105\n")
// custom 3
X_ENTRY(custom_command, entry_3_use, "yes")
X_ENTRY(custom_command, entry_3_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_3_label, "Custom 3")
X_ENTRY(custom_command, entry_3_gcode, "M105\n")
// custom 4
X_ENTRY(custom_command, entry_4_use, "yes")
X_ENTRY(custom_command, entry_4_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_4_label, "Custom 4")
X_ENTRY(custom_command, entry_4_gcode, "M105\n")
// custom 5
X_ENTRY(custom_command, entry_5_use, "yes")
X_ENTRY(custom_command, entry_5_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_5_label, "Custom 5")
X_ENTRY(custom_command, entry_5_gcode, "M105\n")
// custom 6
X_ENTRY(custom_command, entry_6_use, "yes")
X_ENTRY(custom_command, entry_6_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_6_label, "Custom 6")
X_ENTRY(custom_command, entry_6_gcode, "M105\n")
// custom 7
X_ENTRY(custom_command, entry_7_use, "yes")
X_ENTRY(custom_command, entry_7_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_7_label, "Custom 7")
X_ENTRY(custom_command, entry_7_gcode, "M105\n")
// custom 8
X_ENTRY(custom_command, entry_8_use, "yes")
X_ENTRY(custom_command, entry_8_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_8_label, "Custom 8")
X_ENTRY(custom_command, entry_8_gcode, "M105\n")
// custom 9
X_ENTRY(custom_command, entry_9_use, "yes")
X_ENTRY(custom_command, entry_9_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_9_label, "Custom 9")
X_ENTRY(custom_command, entry_9_gcode, "M105\n")
// custom 10
X_ENTRY(custom_command, entry_10_use, "yes")
X_ENTRY(custom_command, entry_10_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_10_label, "Custom 10")
X_ENTRY(custom_command, entry_10_gcode, "M105\n")
// custom 11
X_ENTRY(custom_command, entry_11_use, "yes")
X_ENTRY(custom_command, entry_11_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_11_label, "Custom 11")
X_ENTRY(custom_command, entry_11_gcode, "M105\n")
// custom 12
X_ENTRY(custom_command, entry_12_use, "yes")
X_ENTRY(custom_command, entry_12_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_12_label, "Custom 12")
X_ENTRY(custom_command, entry_12_gcode, "M105\n")
// custom 13
X_ENTRY(custom_command, entry_13_use, "yes")
X_ENTRY(custom_command, entry_13_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_13_label, "Custom 13")
X_ENTRY(custom_command, entry_13_gcode, "M105\n")
// custom 14
X_ENTRY(custom_command, entry_14_use, "yes")
X_ENTRY(custom_command, entry_14_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_14_label, "Custom 14")
X_ENTRY(custom_command, entry_14_gcode, "M105\n")
// custom 15
X_ENTRY(custom_command, entry_15_use, "yes")
X_ENTRY(custom_command, entry_15_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_15_label, "Custom 15")
X_ENTRY(custom_command, entry_15_gcode, "M105\n")
// custom 16
X_ENTRY(custom_command, entry_16_use, "yes")
X_ENTRY(custom_command, entry_16_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_16_label, "Custom 16")
X_ENTRY(custom_command, entry_16_gcode, "M105\n")
// custom 17
X_ENTRY(custom_command, entry_17_use, "yes")
X_ENTRY(custom_command, entry_17_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_17_label, "Custom 17")
X_ENTRY(custom_command, entry_17_gcode, "M105\n")
// custom 18
X_ENTRY(custom_command, entry_18_use, "yes")
X_ENTRY(custom_command, entry_18_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_18_label, "Custom 18")
X_ENTRY(custom_command, entry_18_gcode, "M105\n")
// custom 19
X_ENTRY(custom_command, entry_19_use, "yes")
X_ENTRY(custom_command, entry_19_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_19_label, "Custom 19")
X_ENTRY(custom_command, entry_19_gcode, "M105\n")
// custom 20
X_ENTRY(custom_command, entry_20_use, "yes")
X_ENTRY(custom_command, entry_20_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_command, entry_20_label, "Custom 20")
X_ENTRY(custom_command, entry_20_gcode, "M105\n")

//
// HeaterPreset.c
//

// [heater_preset]

// preheat 1
X_ENTRY(heater_preset, preheat_1_key, "0")
X_ENTRY(heater_preset, preheat_1_use, "yes")
X_ENTRY(heater_preset, preheat_1_icon, "Preheat_1")
X_ENTRY(heater_preset, preheat_1_label, "PLA")
X_ENTRY(heater_preset, preheat_1_hotbed, "60")
X_ENTRY(heater_preset, preheat_1_nozzle, "200")
// preheat 2
X_ENTRY(heater_preset, preheat_2_key, "1")
X_ENTRY(heater_preset, preheat_2_use, "yes")
X_ENTRY(heater_preset, preheat_2_icon, "Preheat_2")
X_ENTRY(heater_preset, preheat_2_label, "PET")
X_ENTRY(heater_preset, preheat_2_hotbed, "70")
X_ENTRY(heater_preset, preheat_2_nozzle, "250")
// preheat 3
X_ENTRY(heater_preset, preheat_3_key, "2")
X_ENTRY(heater_preset, preheat_3_use, "yes")
X_ENTRY(heater_preset, preheat_3_icon, "Preheat_3")
X_ENTRY(heater_preset, preheat_3_label, "ABS")
X_ENTRY(heater_preset, preheat_3_hotbed, "100")
X_ENTRY(heater_preset, preheat_3_nozzle, "230")
// preheat 4
X_ENTRY(heater_preset, preheat_4_key, "4")
X_ENTRY(heater_preset, preheat_4_use, "yes")
X_ENTRY(heater_preset, preheat_4_icon, "Preheat_4")
X_ENTRY(heater_preset, preheat_4_label, "NYL")
X_ENTRY(heater_preset, preheat_4_hotbed, "100")
X_ENTRY(heater_preset, preheat_4_nozzle, "260")
// preheat 5
X_ENTRY(heater_preset, preheat_5_key, "5")
X_ENTRY(heater_preset, preheat_5_use, "yes")
X_ENTRY(heater_preset, preheat_5_icon, "Preheat_5")
X_ENTRY(heater_preset, preheat_5_label, "PEK")
X_ENTRY(heater_preset, preheat_5_hotbed, "120")
X_ENTRY(heater_preset, preheat_5_nozzle, "280")

//
// HeaterControl.c
//

// [heater_control]

// fields:
// gcid :: gcode-id - used by gcode parser, not user-visible
// nick :: short tool name shown on top level smart/dynamic icon
// label :: long tool name shown on regular icon and with edit mode
// temp_min :: minimum allowed tool temperature
// temp_max :: maximum allowed tool temperature
// gcode_init :: command prefix to set-and-continue temperature
// gcode_wait :: command prefix to set-and-await-ready temperature

// note: gcode in this section is prefix-only, does not end with "\n"

// tool 0
X_ENTRY(heater_control, tool_0_use, "yes")
X_ENTRY(heater_control, tool_0_icon, "Hotbed")
X_ENTRY(heater_control, tool_0_gcid, "B:")
X_ENTRY(heater_control, tool_0_nick, "Bed")
X_ENTRY(heater_control, tool_0_label, "HotBed")
X_ENTRY(heater_control, tool_0_temp_min, "20")
X_ENTRY(heater_control, tool_0_temp_max, "110")
X_ENTRY(heater_control, tool_0_gcode_init, "M140")
X_ENTRY(heater_control, tool_0_gcode_wait, "M190")
// tool 1
X_ENTRY(heater_control, tool_1_use, "yes")
X_ENTRY(heater_control, tool_1_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_1_gcid, "T0:")
X_ENTRY(heater_control, tool_1_nick, "H-0")
X_ENTRY(heater_control, tool_1_label, "Heat-0")
X_ENTRY(heater_control, tool_1_temp_min, "20")
X_ENTRY(heater_control, tool_1_temp_max, "275")
X_ENTRY(heater_control, tool_1_gcode_init, "M104 T0")
X_ENTRY(heater_control, tool_1_gcode_wait, "M109 T0")
// tool 2
X_ENTRY(heater_control, tool_2_use, "yes")
X_ENTRY(heater_control, tool_2_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_2_gcid, "T1:")
X_ENTRY(heater_control, tool_2_nick, "H-1")
X_ENTRY(heater_control, tool_2_label, "Heat-1")
X_ENTRY(heater_control, tool_2_temp_min, "20")
X_ENTRY(heater_control, tool_2_temp_max, "275")
X_ENTRY(heater_control, tool_2_gcode_init, "M104 T1")
X_ENTRY(heater_control, tool_2_gcode_wait, "M109 T1")
// tool 3
X_ENTRY(heater_control, tool_3_use, "yes")
X_ENTRY(heater_control, tool_3_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_3_gcid, "T2:")
X_ENTRY(heater_control, tool_3_nick, "H-2")
X_ENTRY(heater_control, tool_3_label, "Heat-2")
X_ENTRY(heater_control, tool_3_temp_min, "20")
X_ENTRY(heater_control, tool_3_temp_max, "275")
X_ENTRY(heater_control, tool_3_gcode_init, "M104 T2")
X_ENTRY(heater_control, tool_3_gcode_wait, "M109 T2")
// tool 4
X_ENTRY(heater_control, tool_4_use, "yes")
X_ENTRY(heater_control, tool_4_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_4_gcid, "T3:")
X_ENTRY(heater_control, tool_4_nick, "H-3")
X_ENTRY(heater_control, tool_4_label, "Heat-3")
X_ENTRY(heater_control, tool_4_temp_min, "20")
X_ENTRY(heater_control, tool_4_temp_max, "275")
X_ENTRY(heater_control, tool_4_gcode_init, "M104 T3")
X_ENTRY(heater_control, tool_4_gcode_wait, "M109 T3")
// tool 4
X_ENTRY(heater_control, tool_5_use, "yes")
X_ENTRY(heater_control, tool_5_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_5_gcid, "T4:")
X_ENTRY(heater_control, tool_5_nick, "H-4")
X_ENTRY(heater_control, tool_5_label, "Heat-4")
X_ENTRY(heater_control, tool_5_temp_min, "20")
X_ENTRY(heater_control, tool_5_temp_max, "275")
X_ENTRY(heater_control, tool_5_gcode_init, "M104 T4")
X_ENTRY(heater_control, tool_5_gcode_wait, "M109 T4")
// tool 6
X_ENTRY(heater_control, tool_6_use, "yes")
X_ENTRY(heater_control, tool_6_icon, "NozzleHeat")
X_ENTRY(heater_control, tool_6_gcid, "T5:")
X_ENTRY(heater_control, tool_6_nick, "H-5")
X_ENTRY(heater_control, tool_6_label, "Heat-5")
X_ENTRY(heater_control, tool_6_temp_min, "20")
X_ENTRY(heater_control, tool_6_temp_max, "275")
X_ENTRY(heater_control, tool_6_gcode_init, "M104 T5")
X_ENTRY(heater_control, tool_6_gcode_wait, "M109 T5")

//
// FilamentControl.c
//

// [filament_control]

// no tool 0

// tool 1
X_ENTRY(filament_control, tool_1_use, "yes")
X_ENTRY(filament_control, tool_1_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_1_label, "Moto-0")
X_ENTRY(filament_control, tool_1_gcode_tool, "T0\n")
// tool 2
X_ENTRY(filament_control, tool_2_use, "yes")
X_ENTRY(filament_control, tool_2_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_2_label, "Moto-1")
X_ENTRY(filament_control, tool_2_gcode_tool, "T1\n")
// tool 3
X_ENTRY(filament_control, tool_3_use, "yes")
X_ENTRY(filament_control, tool_3_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_3_label, "Moto-2")
X_ENTRY(filament_control, tool_3_gcode_tool, "T2\n")
// tool 5
X_ENTRY(filament_control, tool_4_use, "yes")
X_ENTRY(filament_control, tool_4_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_4_label, "Moto-3")
X_ENTRY(filament_control, tool_4_gcode_tool, "T3\n")
// tool 5
X_ENTRY(filament_control, tool_5_use, "yes")
X_ENTRY(filament_control, tool_5_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_5_label, "Moto-4")
X_ENTRY(filament_control, tool_5_gcode_tool, "T4\n")
// tool 6
X_ENTRY(filament_control, tool_6_use, "yes")
X_ENTRY(filament_control, tool_6_icon, "NozzleMoto")
X_ENTRY(filament_control, tool_6_label, "Moto-5")
X_ENTRY(filament_control, tool_6_gcode_tool, "T5\n")

// extrusion speed

// "slow"
X_ENTRY(filament_control, speed_1_rate, "60")
X_ENTRY(filament_control, speed_1_icon, "Slow")
X_ENTRY(filament_control, speed_1_label, "1")// label-code or user-text

// "norm"
X_ENTRY(filament_control, speed_2_rate, "600")
X_ENTRY(filament_control, speed_2_icon, "Normal")
X_ENTRY(filament_control, speed_2_label, "2")// label-code or user-text

// "fast"
X_ENTRY(filament_control, speed_3_rate, "1200")
X_ENTRY(filament_control, speed_3_icon, "Fast")
X_ENTRY(filament_control, speed_3_label, "3")// label-code or user-text

//
// SystemDebug.c
//

// [debug_menu]

X_ENTRY(debug_menu, entry_1_use, "yes")
X_ENTRY(debug_menu, entry_1_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(debug_menu, entry_1_label, "System Version")
X_ENTRY(debug_menu, entry_1_gcode, "show_system_version")


X_ENTRY(debug_menu, entry_2_use, "yes")
X_ENTRY(debug_menu, entry_2_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(debug_menu, entry_2_label, "Memory Report")
X_ENTRY(debug_menu, entry_2_gcode, "show_memory_report")

//
//
//
