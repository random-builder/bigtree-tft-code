//
//
//

#pragma once

#undef  AUTO_SAVE_LOAD_LEVELING_VALUE

#undef  X_MAX_POS
#undef  Y_MAX_POS
#undef  Z_MAX_POS
#define X_MAX_POS 200
#define Y_MAX_POS 200
#define Z_MAX_POS 170

//#undef  PREHEAT_BED
//#undef  PREHEAT_HOTEND
//#undef  PREHEAT_TEXT
//#define PREHEAT_BED      {  60,       70,     120  }
//#define PREHEAT_HOTEND   {  205,     235,     265  }
//#define PREHEAT_TEXT     { "PLA",   "ABS",   "NYL" }

#undef  SPEED_ID
#define SPEED_ID         {"M/S", "F/R"}

#undef  HEAT_MAX_TEMP
#define HEAT_MAX_TEMP    {150,    350,       350,       350,       350,       350,       350}
