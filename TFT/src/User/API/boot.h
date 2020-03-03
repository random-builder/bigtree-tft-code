//
// boot update actions
//

#pragma once

#include "variants.h"
#include "stdbool.h"

#define FLASH_PAGE_SIZE      256  // BY25Q64AS spec

#define FLASH_SECTOR_SIZE 0x1000  // BY25Q64AS spec, 4K

#ifndef FLASH_TOTAL_SIZE
#define FLASH_TOTAL_SIZE 0x800000 // BY25Q64AS spec, 8M
#endif

// native image uses 2 bytes per pixel
#define FLASH_IMAGE_SIZE(image_width, image_height) \
    ( (image_width) * (image_height) * 2 )

// native image size, rounded to sector boundary
#define FLASH_IMAGE_UNIT(image_width, image_height) \
    ( ( ( (FLASH_IMAGE_SIZE(image_width, image_height)) / FLASH_SECTOR_SIZE) + 1) * FLASH_SECTOR_SIZE )

//
// address in spiflash W25Qxx, rounded to sector boundary
//

#define BASE_ADDR               0x0 // memory start

#define BYTE_ASCII_SIZE         0x1000 // byte_ascii.fon exact size 3420<=0x1000
#define BYTE_ASCII_ADDR         BASE_ADDR
#define BYTE_ASCII_TAIL         BYTE_ASCII_ADDR + BYTE_ASCII_SIZE

#define CONFIG_FILE_SIZE        0x3000 // allow up to 12k
#define CONFIG_FILE_ADDR        BYTE_ASCII_TAIL
#define CONFIG_FILE_TAIL        CONFIG_FILE_ADDR + CONFIG_FILE_SIZE

#define WORD_UNICODE_SIZE       0x480000  // word_unicode.fon exact size 4718592==0x480000
#define WORD_UNICODE_ADDR       CONFIG_FILE_TAIL
#define WORD_UNICODE_TAIL       WORD_UNICODE_ADDR + WORD_UNICODE_SIZE

#ifdef  SHOW_LOGO
#define LOGO_SIZE               FLASH_IMAGE_UNIT(LCD_WIDTH,LCD_HEIGHT)
#else
#define LOGO_SIZE               0
#endif
#define LOGO_ADDR               WORD_UNICODE_TAIL
#define LOGO_TAIL               LOGO_ADDR + LOGO_SIZE

#define INFOBOX_SIZE            FLASH_IMAGE_UNIT(INFOBOX_WIDTH,INFOBOX_HEIGHT)
#define INFOBOX_ADDR            LOGO_TAIL
#define INFOBOX_TAIL            INFOBOX_ADDR + INFOBOX_SIZE

#define ICON_SIZE               FLASH_IMAGE_UNIT(ICON_WIDTH,ICON_HEIGHT)
#define ICON_ADDR(index)        INFOBOX_TAIL + (ICON_SIZE * index)

//

#define HAS_BMP     (1<<1)
#define HAS_FONT    (1<<2)
#define HAS_RESET   (1<<3)
#define HAS_CONFIG  (1<<4)

#define TFT_BMP_DIR "0:"RESOURCE_DIR"/bmp"
#define TFT_FONT_DIR "0:"RESOURCE_DIR"/font"
#define TFT_RESET_FILE "0:reset.txt"
#define TFT_CONFIG_FILE "0:config.ini"

typedef union {
    u16 color;
    struct {
        u16 b :5;
        u16 g :6;
        u16 r :5;
    } RGB;
} GUI_PIXEL;

void scanUpdates(void);
bool bmpDecode(char *bmp, u32 addr);
