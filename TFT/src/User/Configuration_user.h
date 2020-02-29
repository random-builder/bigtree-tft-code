//
//
//

#undef AUTO_SAVE_LOAD_LEVELING_VALUE

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
#define PREHEAT_HOTEND   {  200,     230,     260  }
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
