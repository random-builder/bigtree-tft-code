//
//
//

#define UNIFIED_MENU

#undef  SHOW_BTT_BOOTSCREEN

#undef  AUTO_SAVE_LOAD_LEVELING_VALUE

#undef  X_MAX_POS
#undef  Y_MAX_POS
#undef  Z_MAX_POS
#define X_MAX_POS 200
#define Y_MAX_POS 200
#define Z_MAX_POS 170

#undef  PREHEAT_BED
#undef  PREHEAT_HOTEND
#undef  PREHEAT_TEXT
#define PREHEAT_BED      {  60,       70,     120  }
#define PREHEAT_HOTEND   {  205,     235,     265  }
#define PREHEAT_TEXT     { "PLA",   "ABS",   "NYL" }

#undef  CUSTOM_0_LABEL
#undef  CUSTOM_0_GCODE
#define CUSTOM_0_LABEL "Custom0"
#define CUSTOM_0_GCODE "M105\n"

#undef  CUSTOM_1_LABEL
#undef  CUSTOM_1_GCODE
#define CUSTOM_1_LABEL "Custom1"
#define CUSTOM_1_GCODE "M105\n"

#undef  CUSTOM_2_LABEL
#undef  CUSTOM_2_GCODE
#define CUSTOM_2_LABEL "Custom2"
#define CUSTOM_2_GCODE "M105\n"

#undef  CUSTOM_3_LABEL
#undef  CUSTOM_3_GCODE
#define CUSTOM_3_LABEL "Custom3"
#define CUSTOM_3_GCODE "M105\n"

#undef  CUSTOM_4_LABEL
#undef  CUSTOM_4_GCODE
#define CUSTOM_4_LABEL "Custom4"
#define CUSTOM_4_GCODE "M105\n"

#undef  CUSTOM_5_LABEL
#undef  CUSTOM_5_GCODE
#define CUSTOM_5_LABEL "Custom5"
#define CUSTOM_5_GCODE "M105\n"

#undef  CUSTOM_6_LABEL
#undef  CUSTOM_6_GCODE
#define CUSTOM_6_LABEL "Custom6"
#define CUSTOM_6_GCODE "M105\n"


#undef  LEVELING_POINT_1_X
#undef  LEVELING_POINT_1_Y
#undef  LEVELING_POINT_2_X
#undef  LEVELING_POINT_2_Y
#undef  LEVELING_POINT_3_X
#undef  LEVELING_POINT_3_Y
#undef  LEVELING_POINT_4_X
#undef  LEVELING_POINT_4_Y
#undef  LEVELING_POINT_5_X
#undef  LEVELING_POINT_5_Y
//
#define LEVELING_POINT_1_X         (X_MIN_POS + 10)
#define LEVELING_POINT_1_Y         (Y_MIN_POS + 10)
#define LEVELING_POINT_2_X         (X_MAX_POS - 10)
#define LEVELING_POINT_2_Y         (Y_MIN_POS + 10)
#define LEVELING_POINT_3_X         (X_MAX_POS - 10)
#define LEVELING_POINT_3_Y         (Y_MAX_POS - 10)
#define LEVELING_POINT_4_X         (X_MIN_POS + 10)
#define LEVELING_POINT_4_Y         (Y_MAX_POS - 10)
#define LEVELING_POINT_5_X         (X_MIN_POS + X_MAX_POS)/2
#define LEVELING_POINT_5_Y         (Y_MIN_POS + 10)

#undef  SPEED_ID
#define SPEED_ID         {"M/S", "F/R"}

#undef  HEAT_MAX_TEMP
#define HEAT_MAX_TEMP    {150,    350,       350,       350,       350,       350,       350}
