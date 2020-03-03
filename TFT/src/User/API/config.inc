//
// define config file structure and default values
//
// format:
// X_ENTRY(SECTION, NAME, DEFAULT_VALUE)
//
// usage:
// https://en.wikipedia.org/wiki/X_Macro

X_ENTRY(leveling_auto, command_probe_drop, "M280 P0 S10\n")
X_ENTRY(leveling_auto, command_probe_stow, "M280 P0 S90\n")
X_ENTRY(leveling_auto, command_probe_test, "M280 P0 S120\n")
X_ENTRY(leveling_auto, command_probe_invoke, "G28\n G29\n")
X_ENTRY(leveling_auto, command_probe_repeat, "G48\n")

// front/left
X_ENTRY(leveling_manual, point_1_key, "0")
X_ENTRY(leveling_manual, point_1_use, "yes")
X_ENTRY(leveling_manual, point_1_X, "10")
X_ENTRY(leveling_manual, point_1_Y, "10")
X_ENTRY(leveling_manual, point_1_icon, "Point_1")
// front/rite
X_ENTRY(leveling_manual, point_2_key, "1")
X_ENTRY(leveling_manual, point_2_use, "yes")
X_ENTRY(leveling_manual, point_2_X, "190")
X_ENTRY(leveling_manual, point_2_Y, "10")
X_ENTRY(leveling_manual, point_2_icon, "Point_2")
// rear/left
X_ENTRY(leveling_manual, point_3_key, "2")
X_ENTRY(leveling_manual, point_3_use, "yes")
X_ENTRY(leveling_manual, point_3_X, "10")
X_ENTRY(leveling_manual, point_3_Y, "190")
X_ENTRY(leveling_manual, point_3_icon, "Point_3")
// rear/rite
X_ENTRY(leveling_manual, point_4_key, "4")
X_ENTRY(leveling_manual, point_4_use, "yes")
X_ENTRY(leveling_manual, point_4_X, "190")
X_ENTRY(leveling_manual, point_4_Y, "190")
X_ENTRY(leveling_manual, point_4_icon, "Point_4")
// front/center
X_ENTRY(leveling_manual, point_5_key, "5")
X_ENTRY(leveling_manual, point_5_use, "yes")
X_ENTRY(leveling_manual, point_5_X, "100")
X_ENTRY(leveling_manual, point_5_Y, "10")
X_ENTRY(leveling_manual, point_5_icon, "Point_5")

X_ENTRY(leveling_manual, command_reset_home, "G28\n")
