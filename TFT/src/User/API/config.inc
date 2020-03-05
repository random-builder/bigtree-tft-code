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

X_ENTRY(leveling_manual, feed_rate_XY, STRINGIFY(LEVELING_FEEDRATE_XY))
X_ENTRY(leveling_manual, feed_rate_ZZ, STRINGIFY(LEVELING_FEEDRATE_ZZ))
X_ENTRY(leveling_manual, point_Z_upper, STRINGIFY(LEVELING_POINT_Z_UPPER))
X_ENTRY(leveling_manual, point_Z_lower, STRINGIFY(LEVELING_POINT_Z_LOWER))

// front/left
X_ENTRY(leveling_manual, point_1_key, "0")
X_ENTRY(leveling_manual, point_1_use, "yes")
X_ENTRY(leveling_manual, point_1_X, STRINGIFY(LEVELING_POINT_1_X))
X_ENTRY(leveling_manual, point_1_Y, STRINGIFY(LEVELING_POINT_1_Y))
X_ENTRY(leveling_manual, point_1_icon, LEVELING_POINT_1_icon)
// front/rite
X_ENTRY(leveling_manual, point_2_key, "1")
X_ENTRY(leveling_manual, point_2_use, "yes")
X_ENTRY(leveling_manual, point_2_X, STRINGIFY(LEVELING_POINT_2_X))
X_ENTRY(leveling_manual, point_2_Y, STRINGIFY(LEVELING_POINT_2_Y))
X_ENTRY(leveling_manual, point_2_icon, LEVELING_POINT_2_icon)
// rear/left
X_ENTRY(leveling_manual, point_3_key, "2")
X_ENTRY(leveling_manual, point_3_use, "yes")
X_ENTRY(leveling_manual, point_3_X, STRINGIFY(LEVELING_POINT_3_X))
X_ENTRY(leveling_manual, point_3_Y, STRINGIFY(LEVELING_POINT_3_Y))
X_ENTRY(leveling_manual, point_3_icon, LEVELING_POINT_3_icon)
// rear/rite
X_ENTRY(leveling_manual, point_4_key, "4")
X_ENTRY(leveling_manual, point_4_use, "yes")
X_ENTRY(leveling_manual, point_4_X, STRINGIFY(LEVELING_POINT_4_X))
X_ENTRY(leveling_manual, point_4_Y, STRINGIFY(LEVELING_POINT_4_Y))
X_ENTRY(leveling_manual, point_4_icon, LEVELING_POINT_4_icon)
// front/center
X_ENTRY(leveling_manual, point_5_key, "5")
X_ENTRY(leveling_manual, point_5_use, "yes")
X_ENTRY(leveling_manual, point_5_X, STRINGIFY(LEVELING_POINT_5_X))
X_ENTRY(leveling_manual, point_5_Y, STRINGIFY(LEVELING_POINT_5_Y))
X_ENTRY(leveling_manual, point_5_icon, LEVELING_POINT_5_icon)

X_ENTRY(leveling_manual, command_reset_home, "G28\n")

//
// MachineSettings.c
//

// [custom_menu]

// custom 0
X_ENTRY(custom_menu, entry_0_key, "0")
X_ENTRY(custom_menu, entry_0_use, "yes")
X_ENTRY(custom_menu, entry_0_label, CUSTOM_0_LABEL)
X_ENTRY(custom_menu, entry_0_gcode, CUSTOM_0_GCODE)
// custom 1
X_ENTRY(custom_menu, entry_1_key, "1")
X_ENTRY(custom_menu, entry_1_use, "yes")
X_ENTRY(custom_menu, entry_1_label, CUSTOM_1_LABEL)
X_ENTRY(custom_menu, entry_1_gcode, CUSTOM_1_GCODE)
// custom 2
X_ENTRY(custom_menu, entry_2_key, "2")
X_ENTRY(custom_menu, entry_2_use, "yes")
X_ENTRY(custom_menu, entry_2_label, CUSTOM_2_LABEL)
X_ENTRY(custom_menu, entry_2_gcode, CUSTOM_2_GCODE)
// custom 3
X_ENTRY(custom_menu, entry_3_key, "3")
X_ENTRY(custom_menu, entry_3_use, "yes")
X_ENTRY(custom_menu, entry_3_label, CUSTOM_3_LABEL)
X_ENTRY(custom_menu, entry_3_gcode, CUSTOM_3_GCODE)
// custom 4
X_ENTRY(custom_menu, entry_4_key, "4")
X_ENTRY(custom_menu, entry_4_use, "yes")
X_ENTRY(custom_menu, entry_4_label, CUSTOM_4_LABEL)
X_ENTRY(custom_menu, entry_4_gcode, CUSTOM_4_GCODE)
// custom 5
X_ENTRY(custom_menu, entry_5_key, "5")
X_ENTRY(custom_menu, entry_5_use, "yes")
X_ENTRY(custom_menu, entry_5_label, CUSTOM_5_LABEL)
X_ENTRY(custom_menu, entry_5_gcode, CUSTOM_5_GCODE)
// custom 6
X_ENTRY(custom_menu, entry_6_key, "6")
X_ENTRY(custom_menu, entry_6_use, "yes")
X_ENTRY(custom_menu, entry_6_label, CUSTOM_6_LABEL)
X_ENTRY(custom_menu, entry_6_gcode, CUSTOM_6_GCODE)
// custom 7
X_ENTRY(custom_menu, entry_7_key, "7")
X_ENTRY(custom_menu, entry_7_use, "yes")
X_ENTRY(custom_menu, entry_7_label, CUSTOM_7_LABEL)
X_ENTRY(custom_menu, entry_7_gcode, CUSTOM_7_GCODE)
// custom 8
X_ENTRY(custom_menu, entry_8_key, "8")
X_ENTRY(custom_menu, entry_8_use, "yes")
X_ENTRY(custom_menu, entry_8_label, CUSTOM_8_LABEL)
X_ENTRY(custom_menu, entry_8_gcode, CUSTOM_8_GCODE)
// custom 9
X_ENTRY(custom_menu, entry_9_key, "9")
X_ENTRY(custom_menu, entry_9_use, "yes")
X_ENTRY(custom_menu, entry_9_label, CUSTOM_9_LABEL)
X_ENTRY(custom_menu, entry_9_gcode, CUSTOM_9_GCODE)
// custom 10
X_ENTRY(custom_menu, entry_10_key, "10")
X_ENTRY(custom_menu, entry_10_use, "yes")
X_ENTRY(custom_menu, entry_10_label, CUSTOM_10_LABEL)
X_ENTRY(custom_menu, entry_10_gcode, CUSTOM_10_GCODE)
// custom 11
X_ENTRY(custom_menu, entry_11_key, "11")
X_ENTRY(custom_menu, entry_11_use, "yes")
X_ENTRY(custom_menu, entry_11_label, CUSTOM_11_LABEL)
X_ENTRY(custom_menu, entry_11_gcode, CUSTOM_11_GCODE)
// custom 12
X_ENTRY(custom_menu, entry_12_key, "12")
X_ENTRY(custom_menu, entry_12_use, "yes")
X_ENTRY(custom_menu, entry_12_label, CUSTOM_12_LABEL)
X_ENTRY(custom_menu, entry_12_gcode, CUSTOM_12_GCODE)
// custom 13
X_ENTRY(custom_menu, entry_13_key, "13")
X_ENTRY(custom_menu, entry_13_use, "yes")
X_ENTRY(custom_menu, entry_13_label, CUSTOM_13_LABEL)
X_ENTRY(custom_menu, entry_13_gcode, CUSTOM_13_GCODE)
// custom 14
X_ENTRY(custom_menu, entry_14_key, "14")
X_ENTRY(custom_menu, entry_14_use, "yes")
X_ENTRY(custom_menu, entry_14_label, CUSTOM_14_LABEL)
X_ENTRY(custom_menu, entry_14_gcode, CUSTOM_14_GCODE)
// custom 15
X_ENTRY(custom_menu, entry_15_key, "15")
X_ENTRY(custom_menu, entry_15_use, "yes")
X_ENTRY(custom_menu, entry_15_label, CUSTOM_15_LABEL)
X_ENTRY(custom_menu, entry_15_gcode, CUSTOM_15_GCODE)
// custom 16
X_ENTRY(custom_menu, entry_16_key, "16")
X_ENTRY(custom_menu, entry_16_use, "yes")
X_ENTRY(custom_menu, entry_16_label, CUSTOM_16_LABEL)
X_ENTRY(custom_menu, entry_16_gcode, CUSTOM_16_GCODE)
// custom 17
X_ENTRY(custom_menu, entry_17_key, "17")
X_ENTRY(custom_menu, entry_17_use, "yes")
X_ENTRY(custom_menu, entry_17_label, CUSTOM_17_LABEL)
X_ENTRY(custom_menu, entry_17_gcode, CUSTOM_17_GCODE)
// custom 18
X_ENTRY(custom_menu, entry_18_key, "18")
X_ENTRY(custom_menu, entry_18_use, "yes")
X_ENTRY(custom_menu, entry_18_label, CUSTOM_18_LABEL)
X_ENTRY(custom_menu, entry_18_gcode, CUSTOM_18_GCODE)
// custom 19
X_ENTRY(custom_menu, entry_19_key, "19")
X_ENTRY(custom_menu, entry_19_use, "yes")
X_ENTRY(custom_menu, entry_19_label, CUSTOM_19_LABEL)
X_ENTRY(custom_menu, entry_19_gcode, CUSTOM_19_GCODE)
