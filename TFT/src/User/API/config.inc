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

// [custom_menu]

// note: icons in ths section use symbol names from TFT/src/User/API/UI/symbol.inc

// custom 0
X_ENTRY(custom_menu, entry_0_key, "0")
X_ENTRY(custom_menu, entry_0_use, "yes")
X_ENTRY(custom_menu, entry_0_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_0_label, CUSTOM_0_LABEL)
X_ENTRY(custom_menu, entry_0_gcode, CUSTOM_0_GCODE)
// custom 1
X_ENTRY(custom_menu, entry_1_key, "1")
X_ENTRY(custom_menu, entry_1_use, "yes")
X_ENTRY(custom_menu, entry_1_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_1_label, CUSTOM_1_LABEL)
X_ENTRY(custom_menu, entry_1_gcode, CUSTOM_1_GCODE)
// custom 2
X_ENTRY(custom_menu, entry_2_key, "2")
X_ENTRY(custom_menu, entry_2_use, "yes")
X_ENTRY(custom_menu, entry_2_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_2_label, CUSTOM_2_LABEL)
X_ENTRY(custom_menu, entry_2_gcode, CUSTOM_2_GCODE)
// custom 3
X_ENTRY(custom_menu, entry_3_key, "3")
X_ENTRY(custom_menu, entry_3_use, "yes")
X_ENTRY(custom_menu, entry_3_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_3_label, CUSTOM_3_LABEL)
X_ENTRY(custom_menu, entry_3_gcode, CUSTOM_3_GCODE)
// custom 4
X_ENTRY(custom_menu, entry_4_key, "4")
X_ENTRY(custom_menu, entry_4_use, "yes")
X_ENTRY(custom_menu, entry_4_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_4_label, CUSTOM_4_LABEL)
X_ENTRY(custom_menu, entry_4_gcode, CUSTOM_4_GCODE)
// custom 5
X_ENTRY(custom_menu, entry_5_key, "5")
X_ENTRY(custom_menu, entry_5_use, "yes")
X_ENTRY(custom_menu, entry_5_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_5_label, CUSTOM_5_LABEL)
X_ENTRY(custom_menu, entry_5_gcode, CUSTOM_5_GCODE)
// custom 6
X_ENTRY(custom_menu, entry_6_key, "6")
X_ENTRY(custom_menu, entry_6_use, "yes")
X_ENTRY(custom_menu, entry_6_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_6_label, CUSTOM_6_LABEL)
X_ENTRY(custom_menu, entry_6_gcode, CUSTOM_6_GCODE)
// custom 7
X_ENTRY(custom_menu, entry_7_key, "7")
X_ENTRY(custom_menu, entry_7_use, "yes")
X_ENTRY(custom_menu, entry_7_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_7_label, CUSTOM_7_LABEL)
X_ENTRY(custom_menu, entry_7_gcode, CUSTOM_7_GCODE)
// custom 8
X_ENTRY(custom_menu, entry_8_key, "8")
X_ENTRY(custom_menu, entry_8_use, "yes")
X_ENTRY(custom_menu, entry_8_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_8_label, CUSTOM_8_LABEL)
X_ENTRY(custom_menu, entry_8_gcode, CUSTOM_8_GCODE)
// custom 9
X_ENTRY(custom_menu, entry_9_key, "9")
X_ENTRY(custom_menu, entry_9_use, "yes")
X_ENTRY(custom_menu, entry_9_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_9_label, CUSTOM_9_LABEL)
X_ENTRY(custom_menu, entry_9_gcode, CUSTOM_9_GCODE)
// custom 10
X_ENTRY(custom_menu, entry_10_key, "10")
X_ENTRY(custom_menu, entry_10_use, "yes")
X_ENTRY(custom_menu, entry_10_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_10_label, CUSTOM_10_LABEL)
X_ENTRY(custom_menu, entry_10_gcode, CUSTOM_10_GCODE)
// custom 11
X_ENTRY(custom_menu, entry_11_key, "11")
X_ENTRY(custom_menu, entry_11_use, "yes")
X_ENTRY(custom_menu, entry_11_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_11_label, CUSTOM_11_LABEL)
X_ENTRY(custom_menu, entry_11_gcode, CUSTOM_11_GCODE)
// custom 12
X_ENTRY(custom_menu, entry_12_key, "12")
X_ENTRY(custom_menu, entry_12_use, "yes")
X_ENTRY(custom_menu, entry_12_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_12_label, CUSTOM_12_LABEL)
X_ENTRY(custom_menu, entry_12_gcode, CUSTOM_12_GCODE)
// custom 13
X_ENTRY(custom_menu, entry_13_key, "13")
X_ENTRY(custom_menu, entry_13_use, "yes")
X_ENTRY(custom_menu, entry_13_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_13_label, CUSTOM_13_LABEL)
X_ENTRY(custom_menu, entry_13_gcode, CUSTOM_13_GCODE)
// custom 14
X_ENTRY(custom_menu, entry_14_key, "14")
X_ENTRY(custom_menu, entry_14_use, "yes")
X_ENTRY(custom_menu, entry_14_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_14_label, CUSTOM_14_LABEL)
X_ENTRY(custom_menu, entry_14_gcode, CUSTOM_14_GCODE)
// custom 15
X_ENTRY(custom_menu, entry_15_key, "15")
X_ENTRY(custom_menu, entry_15_use, "yes")
X_ENTRY(custom_menu, entry_15_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_15_label, CUSTOM_15_LABEL)
X_ENTRY(custom_menu, entry_15_gcode, CUSTOM_15_GCODE)
// custom 16
X_ENTRY(custom_menu, entry_16_key, "16")
X_ENTRY(custom_menu, entry_16_use, "yes")
X_ENTRY(custom_menu, entry_16_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_16_label, CUSTOM_16_LABEL)
X_ENTRY(custom_menu, entry_16_gcode, CUSTOM_16_GCODE)
// custom 17
X_ENTRY(custom_menu, entry_17_key, "17")
X_ENTRY(custom_menu, entry_17_use, "yes")
X_ENTRY(custom_menu, entry_17_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_17_label, CUSTOM_17_LABEL)
X_ENTRY(custom_menu, entry_17_gcode, CUSTOM_17_GCODE)
// custom 18
X_ENTRY(custom_menu, entry_18_key, "18")
X_ENTRY(custom_menu, entry_18_use, "yes")
X_ENTRY(custom_menu, entry_18_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_18_label, CUSTOM_18_LABEL)
X_ENTRY(custom_menu, entry_18_gcode, CUSTOM_18_GCODE)
// custom 19
X_ENTRY(custom_menu, entry_19_key, "19")
X_ENTRY(custom_menu, entry_19_use, "yes")
X_ENTRY(custom_menu, entry_19_icon, STRINGIFY(SYMBOL_BULLET))
X_ENTRY(custom_menu, entry_19_label, CUSTOM_19_LABEL)
X_ENTRY(custom_menu, entry_19_gcode, CUSTOM_19_GCODE)

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
X_ENTRY(heater_control, tool_1_icon, "Nozzle")
X_ENTRY(heater_control, tool_1_gcid, "T0:")
X_ENTRY(heater_control, tool_1_nick, "T-0")
X_ENTRY(heater_control, tool_1_label, "Tool-0")
X_ENTRY(heater_control, tool_1_temp_min, "20")
X_ENTRY(heater_control, tool_1_temp_max, "275")
X_ENTRY(heater_control, tool_1_gcode_init, "M104 T0")
X_ENTRY(heater_control, tool_1_gcode_wait, "M109 T0")
// tool 2
X_ENTRY(heater_control, tool_2_use, "yes")
X_ENTRY(heater_control, tool_2_icon, "Nozzle")
X_ENTRY(heater_control, tool_2_gcid, "T1:")
X_ENTRY(heater_control, tool_2_nick, "T-1")
X_ENTRY(heater_control, tool_2_label, "Tool-1")
X_ENTRY(heater_control, tool_2_temp_min, "20")
X_ENTRY(heater_control, tool_2_temp_max, "275")
X_ENTRY(heater_control, tool_2_gcode_init, "M104 T1")
X_ENTRY(heater_control, tool_2_gcode_wait, "M109 T1")
// tool 3
X_ENTRY(heater_control, tool_3_use, "yes")
X_ENTRY(heater_control, tool_3_icon, "Nozzle")
X_ENTRY(heater_control, tool_3_gcid, "T2:")
X_ENTRY(heater_control, tool_3_nick, "T-2")
X_ENTRY(heater_control, tool_3_label, "Tool-2")
X_ENTRY(heater_control, tool_3_temp_min, "20")
X_ENTRY(heater_control, tool_3_temp_max, "275")
X_ENTRY(heater_control, tool_3_gcode_init, "M104 T2")
X_ENTRY(heater_control, tool_3_gcode_wait, "M109 T2")
// tool 4
X_ENTRY(heater_control, tool_4_use, "yes")
X_ENTRY(heater_control, tool_4_icon, "Nozzle")
X_ENTRY(heater_control, tool_4_gcid, "T3:")
X_ENTRY(heater_control, tool_4_nick, "T-3")
X_ENTRY(heater_control, tool_4_label, "Tool-3")
X_ENTRY(heater_control, tool_4_temp_min, "20")
X_ENTRY(heater_control, tool_4_temp_max, "275")
X_ENTRY(heater_control, tool_4_gcode_init, "M104 T3")
X_ENTRY(heater_control, tool_4_gcode_wait, "M109 T3")
// tool 4
X_ENTRY(heater_control, tool_5_use, "yes")
X_ENTRY(heater_control, tool_5_icon, "Nozzle")
X_ENTRY(heater_control, tool_5_gcid, "T4:")
X_ENTRY(heater_control, tool_5_nick, "T-4")
X_ENTRY(heater_control, tool_5_label, "Tool-4")
X_ENTRY(heater_control, tool_5_temp_min, "20")
X_ENTRY(heater_control, tool_5_temp_max, "275")
X_ENTRY(heater_control, tool_5_gcode_init, "M104 T4")
X_ENTRY(heater_control, tool_5_gcode_wait, "M109 T4")
// tool 6
X_ENTRY(heater_control, tool_6_use, "yes")
X_ENTRY(heater_control, tool_6_icon, "Nozzle")
X_ENTRY(heater_control, tool_6_gcid, "T5:")
X_ENTRY(heater_control, tool_6_nick, "T-5")
X_ENTRY(heater_control, tool_6_label, "Tool-5")
X_ENTRY(heater_control, tool_6_temp_min, "20")
X_ENTRY(heater_control, tool_6_temp_max, "275")
X_ENTRY(heater_control, tool_6_gcode_init, "M104 T5")
X_ENTRY(heater_control, tool_6_gcode_wait, "M109 T5")
