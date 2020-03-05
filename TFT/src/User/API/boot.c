//
// boot update actions
//

#include "boot.h"
#include "config.h"
#include "includes.h"

#define BOX_BASE_Y             BYTE_HEIGHT / 4
#define BOX_HEIGHT             BYTE_HEIGHT

#define BOX_ACTION_TITLE_X     0
#define BOX_ACTION_TITLE_Y     BOX_BASE_Y + BOX_HEIGHT * 0

#define BOX_FLASH_USED_X       0
#define BOX_FLASH_USED_Y       BOX_BASE_Y + BOX_HEIGHT * 1

#define BOX_FILE_PROGRESS_X    0
#define BOX_FILE_PROGRESS_Y    BOX_BASE_Y + BOX_HEIGHT * 2

#define BOX_FILE_PATH_X        0
#define BOX_FILE_PATH_Y        BOX_BASE_Y + BOX_HEIGHT * 3

#define BOX_FILE_SIZE_X        0
#define BOX_FILE_SIZE_Y        BOX_BASE_Y + BOX_HEIGHT * 4

#define BOX_ERROR_MESSAGE_X    0
#define BOX_ERROR_MESSAGE_Y    BOX_BASE_Y + BOX_HEIGHT * 5

const GUI_RECT box_action_title =
        {  //
        BOX_ACTION_TITLE_X, BOX_ACTION_TITLE_Y,
        BOX_ACTION_TITLE_X + LCD_WIDTH, BOX_ACTION_TITLE_Y + BOX_HEIGHT };

const GUI_RECT box_flash_used =
        {  //
        BOX_FLASH_USED_X, BOX_FLASH_USED_Y,
        BOX_FLASH_USED_X + LCD_WIDTH, BOX_FLASH_USED_Y + BOX_HEIGHT };

const GUI_RECT box_file_progress =
        {  //
        BOX_FILE_PROGRESS_X, BOX_FILE_PROGRESS_Y,
        BOX_FILE_PROGRESS_X + LCD_WIDTH, BOX_FILE_PROGRESS_Y + BOX_HEIGHT };

const GUI_RECT box_file_path =
        {  //
        BOX_FILE_PATH_X, BOX_FILE_PATH_Y,
        BOX_FILE_PATH_X + LCD_WIDTH, BOX_FILE_PATH_Y + BOX_HEIGHT };

const GUI_RECT box_file_size =
        {  //
        BOX_FILE_SIZE_X, BOX_FILE_SIZE_Y,
        BOX_FILE_SIZE_X + LCD_WIDTH, BOX_FILE_SIZE_Y + BOX_HEIGHT };

const GUI_RECT box_common_message =
        {  //
        BOX_ERROR_MESSAGE_X, BOX_ERROR_MESSAGE_Y,
        BOX_ERROR_MESSAGE_X + LCD_WIDTH, BOX_ERROR_MESSAGE_Y + BOX_HEIGHT };

const GUI_RECT box_icon_view =
        {  //
        (LCD_WIDTH - ICON_WIDTH) / 2, (LCD_HEIGHT * 3 / 4) - (ICON_HEIGHT / 2),
        (LCD_WIDTH + ICON_WIDTH) / 2, (LCD_HEIGHT * 3 / 4) + (ICON_HEIGHT / 2) };

//
// report current action
//
void render_action_title(const char *title) {
    GUI_ClearPrect(&box_action_title);
    GUI_DispString(box_action_title.x0, box_action_title.y0, (u8*) title);
}

//
// report flash usage percent
//
void render_flash_used(const u16 flash_info) {
    char text_buff[64];
    my_sprintf(text_buff, "Flash used: %d %%", flash_info);
    GUI_ClearPrect(&box_flash_used);
    GUI_DispString(box_flash_used.x0, box_flash_used.y0, (u8*) text_buff);
}

//
// report flash usage percent
//
void render_file_progress(const u16 progress_info) {
    char text_buff[64];
    my_sprintf(text_buff, "File progress: %d %%", progress_info);
    GUI_ClearPrect(&box_file_progress);
    GUI_DispString(box_file_progress.x0, box_file_progress.y0, (u8*) text_buff);
}

//
// report file name
//
void render_file_path(const char *file_path) {
    char text_buff[64];
    my_sprintf(text_buff, "Path: %s", file_path);
    GUI_ClearPrect(&box_file_path);
    GUI_DispString(box_file_path.x0, box_file_path.y0, (u8*) text_buff);
}

//
// report file size
//
void render_file_size(const FSIZE_t file_size) {
    char text_buff[64];
    my_sprintf(text_buff, "Size: %d", file_size);
    GUI_ClearPrect(&box_file_size);
    GUI_DispString(box_file_size.x0, box_file_size.y0, (u8*) text_buff);
}

//
// report operation success
//
void render_plain_message(const char *message) {
    char text_buff[64];
    my_sprintf(text_buff, "%s", message);
    GUI_ClearPrect(&box_common_message);
    GUI_DispString(box_common_message.x0, box_common_message.y0, (u8*) text_buff);
    Delay_ms(3000);  // preview time
}

//
// report operation failure
//
void render_error_message(const char *message) {
    char text_buff[64];
    my_sprintf(text_buff, "Error: %s", message);
    GUI_ClearPrect(&box_common_message);
    GUI_DispString(box_common_message.x0, box_common_message.y0, (u8*) text_buff);
    Delay_ms(3000);  // preview time
}

//
// render system config for debugging
//
void render_config_debug() {
    const SYSTEM_CONFIG *config = config_instance();
    char text_buff[256];
#define X_ENTRY(SECTION, NAME, DEF_VAL) \
    my_sprintf( text_buff, "%s/%s::%s", #SECTION, #NAME, config->CONFIG_ENTRY(SECTION,NAME)); \
    render_error_message(text_buff);
#include "config.inc"
#undef  X_ENTRY
}

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
    if (f_open(&file, TFT_RESET_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
        result |= HAS_RESET;
        f_close(&file);
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
    u8 flash_buff[FLASH_PAGE_SIZE];
    u8 image_pixel[4];
    GUI_PIXEL screen_pixel;

    render_file_path(file_path);

    if (f_open(&image_file, file_path, FA_OPEN_EXISTING | FA_READ) != FR_OK) {
        return false;
    }

    FSIZE_t file_size = f_size(&image_file);
    render_file_size(file_size);

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

    const int flash_sector_max = (image_width * image_height * 2 + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE;

    for (int flash_sector = 0; flash_sector < flash_sector_max; flash_sector++) {
        flash_addr = base_addr + flash_sector * FLASH_SECTOR_SIZE;
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

            if (flash_offset == FLASH_PAGE_SIZE) {
                W25Qxx_WritePage(flash_buff, flash_addr, FLASH_PAGE_SIZE);
                flash_addr += FLASH_PAGE_SIZE;
                flash_offset = 0;
            }
        }
    }

    W25Qxx_WritePage(flash_buff, flash_addr, flash_offset);

    f_close(&image_file);

    return true;
}

void updateLogoImage(void) {

    GUI_Clear(BACKGROUND_COLOR);
    render_action_title("Logo Image Update...");

    if (bmpDecode(TFT_BMP_DIR"/Logo.bmp", LOGO_ADDR)) {
        LOGO_ReadDisplay();
        Delay_ms(3000);  // preview time
    }

}

void updateIconImageSet(void) {
    char file_path[64];
    u32 flash_addr = 0;
    u8 flash_info = 0;
    u8 flash_unit = 0;
    u8 progress_info = 0;
    u8 progress_unit = 0;

    GUI_Clear(BACKGROUND_COLOR);
    render_action_title("Icon Image Update...");

    const int icon_count = icon_list_size();
    const char (*icon_name_list)[FILE_NAME_SIZE] = icon_file_list();

    for (int icon_index = 0; icon_index < icon_count; icon_index++) {
        my_sprintf(file_path, TFT_BMP_DIR"/%s.bmp", icon_name_list[icon_index]);
        flash_addr = ICON_ADDR(icon_index);
        if (bmpDecode(file_path, flash_addr)) {
            ICON_ReadDisplay(box_icon_view.x0, box_icon_view.y0, icon_index);
        }
        // flash usage
        flash_unit = flash_addr * 100 / FLASH_TOTAL_SIZE;
        if (flash_info != flash_unit) {
            flash_info = flash_unit;
            render_flash_used(flash_info);
        }
        // file progress
        progress_unit = icon_index * 100 / icon_count;
        if (progress_info != progress_unit) {
            progress_info = progress_unit;
            render_file_progress(progress_info);
        }
    }

    if (bmpDecode(TFT_BMP_DIR"/InfoBox.bmp", INFOBOX_ADDR)) {
        ICON_CustomReadDisplay(box_icon_view.x0, box_icon_view.y0, INFOBOX_WIDTH, INFOBOX_HEIGHT, INFOBOX_ADDR);
    }

    Delay_ms(3000);  // preview time

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

    if (f_open(&file_data, file_path, FA_OPEN_EXISTING | FA_READ) != FR_OK) {
        render_error_message("can not open file");
        return;
    }

    FSIZE_t file_size = f_size(&file_data);
    if (file_size > base_size) {
        render_error_message("file is too big");
        return;
    }

    flash_buff = malloc(FLASH_SECTOR_SIZE);
    if (flash_buff == NULL) {
        render_error_message("no memory for buffer");
        return;
    }

    GUI_Clear(BACKGROUND_COLOR);
    render_action_title(window_title);
    render_file_path(file_path);
    render_file_size(file_size);

    while (!f_eof(&file_data)) {
        if (f_read(&file_data, flash_buff, FLASH_SECTOR_SIZE, &read_size) != FR_OK) {
            render_error_message("can not read file");
            break;
        }
        flash_addr = base_addr + flash_offset;
        W25Qxx_EraseSector(flash_addr);
        W25Qxx_WriteBuffer(flash_buff, flash_addr, FLASH_SECTOR_SIZE);
        flash_offset += read_size;
        // flash usage
        flash_unit = flash_addr * 100 / FLASH_TOTAL_SIZE;
        if (flash_info != flash_unit) {
            flash_info = flash_unit;
            render_flash_used(flash_info);
        }
        // file progress
        progress_unit = flash_offset * 100 / file_size;
        if (progress_info != progress_unit) {
            progress_info = progress_unit;
            render_file_progress(progress_info);
        }
        if (read_size != FLASH_SECTOR_SIZE) {
            break;  // final sector
        }
    }

    f_close(&file_data);
    free(flash_buff);

    Delay_ms(3000);  // preview time
}

//
// apply config entries form config.ini flash file into the memory struct
//
void parseSystemConfig() {
    FLASH_STREAM flash_stream;

    flash_stream.flash_addr = CONFIG_FILE_ADDR;
    flash_stream.flash_tail = CONFIG_FILE_TAIL;
    flash_stream.buff_size = FLASH_PAGE_SIZE;
    flash_stream.text_point = NULL;  // declare empty

    flash_stream.text_buffer = malloc(flash_stream.buff_size);
    if (flash_stream.text_buffer == NULL) {
        render_error_message("no memory for buffer");
        return;
    }

    if (config_parse_stream(&flash_stream) < 0) {
        render_error_message("can not parse config.ini");
    }

    free(flash_stream.text_buffer);

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
    updateResource(TFT_CONFIG_FILE, CONFIG_FILE_ADDR, CONFIG_FILE_SIZE, "Config File Update...");
}

void updateFontAscii() {
    updateResource(TFT_ASCII_FILE, BYTE_ASCII_ADDR, BYTE_ASCII_SIZE, "Ascii Font Update...");
}

void updateFontUnicode() {
    updateResource(TFT_UNICODE_FILE, WORD_UNICODE_ADDR, WORD_UNICODE_SIZE, "Unicode Font Update...");
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

        if ((result & HAS_FONT) || (result & HAS_BMP)) {
            //f_rename(RESOURCE_DIR, RESOURCE_DIR".CUR");
        }

        if (result & HAS_CONFIG) {
            //const char *config_backup = TFT_CONFIG_FILE".CUR";
            //f_unlink(config_backup);
            //f_rename(TFT_CONFIG_FILE, config_backup);
        }

        scanResetFile();

    }

    parseSystemConfig();

#ifdef SHOW_CONFIG_DEBUG
    render_config_debug();
#endif
#ifdef SHOW_ERROR_VERIFY
    render_plain_message("verify plain message");
    render_error_message("verify error message");
#endif

}
