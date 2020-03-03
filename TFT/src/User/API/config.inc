//
// define config file structure and default values
//
// format:
// X_ENTRY(SECTION, NAME, DEFAULT_VALUE)
//
// usage:
// https://en.wikipedia.org/wiki/X_Macro

X_ENTRY(leveling_automatic, command_probe_drop, "M280 P0 S10\n")
X_ENTRY(leveling_automatic, command_probe_stow, "M280 P0 S90\n")
X_ENTRY(leveling_automatic, command_probe_test, "M280 P0 S120\n")
X_ENTRY(leveling_automatic, command_probe_invoke, "G28\n G29\n")
X_ENTRY(leveling_automatic, command_probe_repeat, "G48\n")

// front/left
X_ENTRY(leveling_manual, point_1_use, "true")
X_ENTRY(leveling_manual, point_1_x_y, "10,10")
X_ENTRY(leveling_manual, point_1_icon, "Point_1")
// front/rite
X_ENTRY(leveling_manual, point_2_use, "true")
X_ENTRY(leveling_manual, point_2_x_y, "190,10")
X_ENTRY(leveling_manual, point_2_icon, "Point_2")
// rear/left
X_ENTRY(leveling_manual, point_3_use, "true")
X_ENTRY(leveling_manual, point_3_x_y, "190,10")
X_ENTRY(leveling_manual, point_3_icon, "Point_3")
// rear/rite
X_ENTRY(leveling_manual, point_4_use, "true")
X_ENTRY(leveling_manual, point_4_x_y, "190,10")
X_ENTRY(leveling_manual, point_4_icon, "Point_4")
// front/center
X_ENTRY(leveling_manual, point_5_use, "true")
X_ENTRY(leveling_manual, point_5_x_y, "10,100")
X_ENTRY(leveling_manual, point_5_icon, "Point_5")
