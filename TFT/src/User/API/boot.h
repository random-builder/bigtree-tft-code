//
//
//

#pragma once

#include "variants.h"
#include "stdbool.h"

#define W25QXX_PAGE_SIZE      256 // BY25Q64AS spec
#define W25QXX_SECTOR_SIZE 0x1000 // BY25Q64AS spec, 4K

#ifndef FLASH_TOTAL_SIZE
#define FLASH_TOTAL_SIZE 0x800000 // BY25Q64AS spec
#endif

// native image uses 2 bytes per pixel
#define FLASH_IMAGE_SIZE(image_width, image_height) \
    ( (image_width) * (image_height) * 2 )

// native image size, rounded to sector boundary
#define FLASH_IMAGE_BLOCK(image_width, image_height) \
    ( ( ( (FLASH_IMAGE_SIZE(image_width, image_height)) / W25QXX_SECTOR_SIZE) + 1) * W25QXX_SECTOR_SIZE )

//
// address in spiflash W25Qxx, rounded to sector boundary
//

#define BASE_ADDR               0x0 // memory start

#define BYTE_ASCII_SIZE         0x1000 // byte_ascii.fon exact size 3420<=0x1000
#define BYTE_ASCII_ADDR         BASE_ADDR
#define BYTE_ASCII_TAIL         BYTE_ASCII_ADDR + BYTE_ASCII_SIZE

#define CONFIG_FILE_SIZE        0x3000 // allow 12k
#define CONFIG_FILE_ADDR        BYTE_ASCII_TAIL
#define CONFIG_FILE_TAIL        CONFIG_FILE_ADDR + CONFIG_FILE_SIZE

#define WORD_UNICODE_SIZE       0x480000  // word_unicode.fon exact size 4718592==0x480000
#define WORD_UNICODE_ADDR       CONFIG_FILE_TAIL
#define WORD_UNICODE_TAIL       WORD_UNICODE_ADDR + WORD_UNICODE_SIZE

#ifdef SHOW_LOGO
#define LOGO_SIZE               FLASH_IMAGE_BLOCK(LCD_WIDTH,LCD_HEIGHT)
#else
#define LOGO_SIZE               0
#endif
#define LOGO_ADDR               WORD_UNICODE_TAIL
#define LOGO_TAIL               LOGO_ADDR + LOGO_SIZE

#define INFOBOX_SIZE            FLASH_IMAGE_BLOCK(INFOBOX_WIDTH,INFOBOX_HEIGHT)
#define INFOBOX_ADDR            LOGO_TAIL
#define INFOBOX_TAIL            INFOBOX_ADDR + INFOBOX_SIZE

#define ICON_SIZE               FLASH_IMAGE_BLOCK(ICON_WIDTH,ICON_HEIGHT)
#define ICON_ADDR(index)        INFOBOX_TAIL + (ICON_SIZE * index)

//

#define HAS_BMP     (1<<1)
#define HAS_FONT    (1<<2)
#define HAS_CONFIG  (1<<3)

#define TFT_BMP_DIR "0:"RESOURCE_DIR"/bmp"
#define TFT_FONT_DIR "0:"RESOURCE_DIR"/font"
#define TFT_RESET_FILE "0:reset.txt"
#define TFT_CONFIG_FILE "0:config.ini"

enum
{
  ICON_HEAT = 0,
  ICON_MOVE,
  ICON_HOME,
  ICON_PRINT,
  ICON_EXTRUDE,
  ICON_FAN,
  ICON_SETTINGS,
  ICON_LEVELING,  
  ICON_INC,
  ICON_DEC,
  ICON_NOZZLE,
  ICON_BED,
  ICON_1_DEGREE,
  ICON_5_DEGREE,
  ICON_10_DEGREE,
  ICON_STOP,
  ICON_BACK,
  ICON_X_INC,
  ICON_Y_INC,
  ICON_Z_INC,
  ICON_01_MM,
  ICON_1_MM,
  ICON_10_MM,
  ICON_X_DEC,
  ICON_Y_DEC,
  ICON_Z_DEC,  
  ICON_X_HOME,
  ICON_Y_HOME,
  ICON_Z_HOME,
  ICON_FOLDER,
  ICON_FILE,
  ICON_PAGE_UP,
  ICON_PAGE_DOWN,
  ICON_PAUSE,
  ICON_RESUME,  
  ICON_LOAD,
  ICON_UNLOAD,
  ICON_SLOW_SPEED,
  ICON_NORMAL_SPEED,
  ICON_FAST_SPEED,
  ICON_E_1_MM,
  ICON_E_5_MM,
  ICON_E_10_MM,  
  ICON_FAN_FULL_SPEED,
  ICON_FAN_HALF_SPEED,
  ICON_ROTATE_UI,
  ICON_LANGUAGE,
  ICON_TOUCHSCREEN_ADJUST,
  ICON_MORE,
  ICON_SCREEN_INFO,
  ICON_BKCOLOR,
  ICON_FONTCOLOR,
  ICON_DISCONNECT,
  ICON_BAUDRATE,  
  ICON_PERCENTAGE,
  ICON_BABYSTEP,
  ICON_001_MM,
  ICON_BSD_SOURCE,
  ICON_SD_SOURCE,
  ICON_U_DISK,
  ICON_RUNOUT,  
//  ICON_POINT_1,
//  ICON_POINT_2,
  ICON_POINT_3,
  ICON_POINT_4,
  ICON_MARLIN,
  ICON_BIGTREE,
  ICON_GCODE,
  ICON_BLTOUCH,
  ICON_BLTOUCH_DEPLOY,
  ICON_BLTOUCH_STOW,
  ICON_BLTOUCH_TEST,
  ICON_BLTOUCH_REPEAT,
  ICON_SCREEN_SETTINGS,
  ICON_MACHINE_SETTINGS,
  ICON_FEATURE_SETTINGS,
  ICON_PROBE_OFFSET,
  ICON_EEPROM_SAVE,
  ICON_SILENT_ON,
  ICON_SHUT_DOWN,
  ICON_RGB_SETTINGS,
  ICON_RGB_RED,
  ICON_RGB_GREEN,
  ICON_RGB_BLUE,
  ICON_RGB_WHITE,
  ICON_RGB_OFF,
  ICON_PREHEAT_BOTH,
  ICON_PREHEAT_1,
  ICON_PREHEAT_2,
  ICON_PREHEAT_3,
  ICON_POWER_SUPPLY,
  ICON_CUSTOM,
  ICON_HOME_MOVE,
  ICON_HEAT_FAN,
  ICON_MANUAL_LEVEL,
  ICON_COOLDOWN,
  ICON_SILENT_OFF,
  ICON_STATUS_NOZZLE,
  ICON_STATUS_BED,
  ICON_STATUS_FAN,
  ICON_MAINMENU,
  ICON_STATUS_SPEED,
  ICON_STATUS_FLOW,
  ICON_PARAMETER,
  ICON_GLOBAL_NOZZLE0,
  ICON_GLOBAL_BED,
  ICON_LEDCOLOR,
  //
  ICON_POINT_5,
  ICON_HOME_ALL,
  ICON_MOTION_STOP,
  ICON_PREHEAT_4,
  ICON_PREHEAT_5,
  ICON_HEAT_1,
  ICON_HEAT_2,

//add new icons above this line  only 
//  ICON_RESERVE

// Preview should be in the last place before ICON_BACKGROUND to save flash storage space
  ICON_PREVIEW,
// Back ground sign
  ICON_BACKGROUND
};

typedef union
{
	u16 color;
	struct{
	u16  b:5;
	u16  g:6;
	u16  r:5;
 }RGB;
} GUI_PIXEL;

void scanUpdates(void);
bool bmpDecode(char *bmp, u32 addr);
