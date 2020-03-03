//
//
//

#include "boot.h"
#include "includes.h"

const GUI_RECT iconUpdateRect = { //
        (LCD_WIDTH - ICON_WIDTH) / 2, (LCD_HEIGHT - ICON_HEIGHT) / 2, //
        (LCD_WIDTH - ICON_WIDTH) / 2 + ICON_WIDTH, (LCD_HEIGHT - ICON_HEIGHT) / 2 + ICON_HEIGHT //
        };

const GUI_RECT labelUpdateRect = { //
        0, (LCD_HEIGHT - ICON_HEIGHT) / 2 + ICON_HEIGHT,
        LCD_WIDTH, (LCD_HEIGHT - ICON_HEIGHT) / 2 + ICON_HEIGHT + BYTE_HEIGHT };

u8 scanUpdateFile(void) {
    DIR dir;
    FIL file;
    u8 result = 0;

    if (f_opendir(&dir, TFT_BMP_DIR) == FR_OK) {
        result |= HAS_BMP;
        f_closedir(&dir);
    }

    if (f_opendir(&dir, TFT_FONT_DIR) == FR_OK) {
        result |= HAS_FONT;
        f_closedir(&dir);
    }

    if (f_open(&file, TFT_CONFIG_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        result |= HAS_CONFIG;
        f_close(&file);
    }

    return result;
}

//
// https://en.wikipedia.org/wiki/BMP_file_format
//
bool bmpDecode(char *file_path, const u32 base_addr) {

    FIL image_file;
    UINT read_size;
    char image_magic[2];
    int image_width, image_height, bytePerLine;
    short image_bpp;
    int image_offset;
    u32 flash_addr;
    u32 flash_offset;
    u8 flash_buff[W25QXX_PAGE_SIZE];
    u8 image_pixel[4];
    GUI_PIXEL screen_pixel;

    if (f_open(&image_file, file_path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        return false;

    f_read(&image_file, image_magic, 2, &read_size);
    if (memcmp(image_magic, "BM", 2)) {
        return false;
    }

    f_lseek(&image_file, 10);
    f_read(&image_file, &image_offset, sizeof(int), &read_size);

    f_lseek(&image_file, 18);
    f_read(&image_file, &image_width, sizeof(int), &read_size);
    f_read(&image_file, &image_height, sizeof(int), &read_size);

    f_lseek(&image_file, 28);
    f_read(&image_file, &image_bpp, sizeof(short), &read_size);

    if (image_bpp < 24) {
        return false;
    }
    image_bpp >>= 3;
    bytePerLine = image_width * image_bpp;
    if (bytePerLine % 4 != 0)
        bytePerLine = (bytePerLine / 4 + 1) * 4;

    const int flash_sector_max = (image_width * image_height * 2 + W25QXX_SECTOR_SIZE - 1) / W25QXX_SECTOR_SIZE;

    for (int flash_sector = 0; flash_sector < flash_sector_max; flash_sector++) {
        flash_addr = base_addr + flash_sector * W25QXX_SECTOR_SIZE;
        W25Qxx_EraseSector(flash_addr);
    }

    flash_addr = base_addr;
    flash_offset = 0;

    for (int image_Y = 0; image_Y < image_height; image_Y++) {
        f_lseek(&image_file, image_offset + (image_height - image_Y - 1) * bytePerLine);
        for (int image_X = 0; image_X < image_width; image_X++) {
            f_read(&image_file, (char*) &image_pixel, image_bpp, &read_size);

            screen_pixel.RGB.r = image_pixel[2] >> 3;
            screen_pixel.RGB.g = image_pixel[1] >> 2;
            screen_pixel.RGB.b = image_pixel[0] >> 3;

            flash_buff[flash_offset++] = (u8) (screen_pixel.color >> 8);
            flash_buff[flash_offset++] = (u8) (screen_pixel.color & 0xFF);

            if (flash_offset == W25QXX_PAGE_SIZE) {
                W25Qxx_WritePage(flash_buff, flash_addr, W25QXX_PAGE_SIZE);
                flash_addr += W25QXX_PAGE_SIZE;
                flash_offset = 0;
            }
        }
    }

    W25Qxx_WritePage(flash_buff, flash_addr, flash_offset);

    f_close(&image_file);

    return true;
}

//
// report flash usage percent
//
void report_flash(u8 flash_info) {
    char text_buff[64];
    my_sprintf((void*) text_buff, "Flash used: %d %%   ", flash_info);
    GUI_DispString(0, 50, (u8*) text_buff);
}

//
// report resource operation failue
//
void report_error(const char *message) {
    GUI_DispString(0, 60, (u8*) message);
    Delay_ms(3000); // preview time
}

void updateLogoImage(void) {

    GUI_Clear(BACKGROUND_COLOR);
    GUI_DispString(100, 5, (u8*) "Logo Update...");

    if (bmpDecode(TFT_BMP_DIR"/Logo.bmp", LOGO_ADDR)) {
        LOGO_ReadDisplay();
        Delay_ms(3000); // preview time
    }

}

void updateIconImageSet(void) {
    char file_path[64];
    u32 flash_addr = 0;
    u8 flash_info = 0;
    u8 flash_unit = 0;

    GUI_Clear(BACKGROUND_COLOR);
    GUI_DispString(100, 5, (u8*) "Icon Update...");

    const int icon_count = icon_list_size();
    const char (*icon_name_list)[FILE_NAME_SIZE] = icon_file_list();

    for (int icon_index = 0; icon_index < icon_count; icon_index++) {
        my_sprintf(file_path, TFT_BMP_DIR"/%s.bmp", icon_name_list[icon_index]);
        flash_addr = ICON_ADDR(icon_index);
        if (bmpDecode(file_path, flash_addr)) {
            GUI_ClearRect(labelUpdateRect.x0, labelUpdateRect.y0, labelUpdateRect.x1, labelUpdateRect.y1);
            GUI_DispStringInPrect(&labelUpdateRect, (u8*) file_path);
            ICON_ReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, icon_index);
        }
        flash_unit = flash_addr * 100 / FLASH_TOTAL_SIZE;
        if (flash_info != flash_unit) {
            flash_info = flash_unit;
            report_flash(flash_info);
        }
    }

    if (bmpDecode(TFT_BMP_DIR"/InfoBox.bmp", INFOBOX_ADDR)) {
        ICON_CustomReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, INFOBOX_WIDTH, INFOBOX_HEIGHT, INFOBOX_ADDR);
    }

    Delay_ms(3000); // preview time

}

//
// persist external file from disk into spi flash memory
//
void updateResource(char *file_path, const u32 base_addr, const u32 base_size, const char *window_title) {

    FIL file_data;
    UINT read_size = 0;
    u32 flash_addr = 0;
    u32 flash_offset = 0;
    u8 *flash_buff = NULL;
    u8 flash_info = 0;
    u8 flash_unit = 0;
    u8 progress_info = 0;
    u8 progress_unit = 0;
    char text_buff[128];

    if (f_open(&file_data, file_path, FA_OPEN_EXISTING | FA_READ) != FR_OK) {
        report_error("Failure: can not open file");
        return;
    }

    FSIZE_t file_size = f_size(&file_data);
    if (file_size > base_size) {
        report_error("Failure: file is too big");
        return;
    }

    flash_buff = malloc(W25QXX_SECTOR_SIZE);
    if (flash_buff == NULL) {
        report_error("Failure: no memory for buffer");
        return;
    }

    GUI_Clear(BACKGROUND_COLOR);
    GUI_DispString(100, 5, (u8*) window_title);

    my_sprintf((void*) text_buff, "%s Size=%dK", file_path, (u32) file_size >> 10);
    GUI_DispString(0, 100, (u8*) text_buff);
    GUI_DispString(0, 140, (u8*) "Progress:   %");

    while (!f_eof(&file_data)) {
        if (f_read(&file_data, flash_buff, W25QXX_SECTOR_SIZE, &read_size) != FR_OK) {
            report_error("Failure: can not read file");
            break;
        }
        flash_addr = base_addr + flash_offset;
        W25Qxx_EraseSector(flash_addr);
        W25Qxx_WriteBuffer(flash_buff, flash_addr, W25QXX_SECTOR_SIZE);
        flash_offset += read_size;
        // flash usage
        flash_unit = flash_addr * 100 / FLASH_TOTAL_SIZE;
        if (flash_info != flash_unit) {
            flash_info = flash_unit;
            report_flash(flash_info);
        }
        // file progress
        progress_unit = flash_offset * 100 / file_size;
        if (progress_info != progress_unit) {
            progress_info = progress_unit;
            GUI_DispDec(0 + BYTE_WIDTH * 9, 140, progress_info, 3, RIGHT);
        }
        if (read_size != W25QXX_SECTOR_SIZE) {
            break; // final sector
        }
    }

    f_close(&file_data);
    free(flash_buff);

    Delay_ms(3000); // preview time
}

void scanResetFile(void) {
    FIL resetfile;
    if (f_open(&resetfile, TFT_RESET_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        f_close(&resetfile);
        f_rename(TFT_RESET_FILE, TFT_RESET_FILE ".DONE");
        infoSettingsReset();
        TSC_Calibration();
        storePara();
    }
}

void updateConfigFile() {
    updateResource(TFT_CONFIG_FILE, CONFIG_FILE_ADDR, CONFIG_FILE_SIZE, "Config Update...");
}

void updateFontAscii() {
    updateResource(TFT_FONT_DIR"/byte_ascii.fon", BYTE_ASCII_ADDR, BYTE_ASCII_SIZE, "Ascii Font Update...");
}

void updateFontUnicode() {
    updateResource(TFT_FONT_DIR"/word_unicode.fon", WORD_UNICODE_ADDR, WORD_UNICODE_SIZE, "Unicode Font Update...");
}

void scanUpdates(void) {
    volatile u8 result = 0;   // must use volatile
    if (mountSDCard()) {
        result = scanUpdateFile();
        if (result & HAS_FONT) {
            updateFontAscii();
        }
        if (result & HAS_CONFIG) {
            updateConfigFile();
        }
        if (result & HAS_FONT) {
            updateFontUnicode();
        }
        if (result & HAS_BMP) {
#ifdef SHOW_LOGO
            updateLogoImage();
#endif
            updateIconImageSet();
        }

        if ((result & HAS_FONT) | (result & HAS_BMP)) {
            //f_rename(RESOURCE_DIR, RESOURCE_DIR".CUR");
        }

        if (result & HAS_CONFIG) {
            const char *config_backup = TFT_CONFIG_FILE".CUR";
            //f_unlink(config_backup);
            //f_rename(TFT_CONFIG_FILE, config_backup);
        }

        scanResetFile();

    }
}
