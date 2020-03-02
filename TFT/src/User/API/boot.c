//
//
//

#include "boot.h"
#include "includes.h"

#include "config.h"

const GUI_RECT iconUpdateRect = {(LCD_WIDTH - ICON_WIDTH)/2,              (LCD_HEIGHT - ICON_HEIGHT)/2, 
                                 (LCD_WIDTH - ICON_WIDTH)/2 + ICON_WIDTH, (LCD_HEIGHT - ICON_HEIGHT)/2 + ICON_HEIGHT};
const GUI_RECT labelUpdateRect = {0,        (LCD_HEIGHT - ICON_HEIGHT)/2 + ICON_HEIGHT, 
                                 LCD_WIDTH, (LCD_HEIGHT - ICON_HEIGHT)/2 + ICON_HEIGHT + BYTE_HEIGHT};

const char icon_file_list[][32]={
"Heat", "Move", "Home", "Print", "Extrude", "Fan", "Settings", "Leveling", "Inc", "Dec",
"Nozzle", "Hotbed", "Temp_1", "Temp_5", "Temp_10", "Stop", "Back", "Inc_X", "Inc_Y", "Inc_Z",
"Mmm_01", "Mmm_1", "Mmm_10", "Dec_X", "Dec_Y","Dec_Z", "Home_X", "Home_Y", "Home_Z", "Folder",
"File", "Page_up", "Page_down", "Pause", "Resume", "Load", "Unload", "Slow", "Normal", "Fast",
"Emm_1", "Emm_5", "Emm_10", "Full", "Half", "Rotate", "Language", "TP_Adjust", "More", "About",
"BackGroundColor", "FontColor", "Disconnect", "BaudRate", "Percentage", "BabyStep", "Mmm_001", "OnBoardSD", "OnTFTSD", "U_Disk",
"Runout",
//"Point_1",
//"Point_2",
"Point_3",
"Point_4",
"Marlin", "BigTreeTech", "Gcode",
"BLTouch",
"BLTouchDrop",
"BLTouchStow",
"BLTouchTest",
"BLTouchRepeat",
"TSCSettings", "MachineSettings", "FeatureSettings", "ProbeOffset", "EEPROMSave", "SilentOn", "ShutDown",
"RGB_Settings", "RGB_Red", "RGB_Green", "RGB_Blue", "RGB_White", "RGB_Off",
"Preheat_Both",
"Preheat_1",
"Preheat_2",
"Preheat_3",
"PowerSupply",
"Custom",
"Home_Move", "Heat_Fan",
"ManualLevel", "CoolDown", "SilentOff","StatusNozzle","StatusBed","StatusFan","MainMenu","StatusSpeed","StatusFlow",
"parametersetting", "global_nozzle", "global_bed", "ledcolor",
//
"Point_5",     // ICON_POINT_5
"Home",        // ICON_HOME_ALL
"Stop",        // ICON_MOTION_STOP
"Preheat_4",   // ICON_PREHEAT_4
"Preheat_5",   // ICON_PREHEAT_5
"Heat_1",      // ICON_HEAT_1
"Heat_2",      // ICON_HEAT_2
};

u8 scanUpdateFile(void)
{
  DIR dir;
  u8 rst = 0;
  
  if (f_opendir(&dir, BMP_ROOT_DIR) == FR_OK)
  {
    rst |= HAS_BMP;  
    f_closedir(&dir);
  }
  if (f_opendir(&dir, FONT_ROOT_DIR) == FR_OK)
  {
    rst |= HAS_FONT;  
    f_closedir(&dir);
  }
  return rst;
}

//
// https://en.wikipedia.org/wiki/BMP_file_format
//
bool bmpDecode(char *file_path, const u32 base_addr)
{  

  FIL   image_file;
  UINT  read_size;
  char  image_magic[2];  
  int   image_width,image_height,bytePerLine;  
  short image_bpp; 
  int   image_offset;
  u32   flash_addr;
  u32   flash_offset;
  u8    flash_buff[W25QXX_PAGE_SIZE];
  u8    image_pixel[4];
  GUI_PIXEL screen_pixel;

  if(f_open(&image_file,file_path,FA_OPEN_EXISTING | FA_READ)!=FR_OK)
    return false;

  f_read(&image_file, image_magic, 2 ,&read_size);  
  if (memcmp(image_magic, "BM", 2))  
    return false;
  
  f_lseek(&image_file, 10);  
  f_read(&image_file, &image_offset, sizeof(int),&read_size);  

  f_lseek(&image_file, 18);  
  f_read(&image_file, &image_width, sizeof(int),&read_size);  
  f_read(&image_file, &image_height, sizeof(int),&read_size);  

  f_lseek(&image_file, 28);  
  f_read(&image_file, &image_bpp, sizeof(short),&read_size);  

  if(image_bpp<24) return false;
  image_bpp >>=3; 
  bytePerLine=image_width*image_bpp;     
  if(bytePerLine%4 !=0) bytePerLine=(bytePerLine/4+1)*4;
  
  const int flash_sector_max = (image_width*image_height*2+W25QXX_SECTOR_SIZE-1)/W25QXX_SECTOR_SIZE;

  for(int flash_sector=0; flash_sector<flash_sector_max; flash_sector++)
  {
    flash_addr=base_addr+flash_sector*W25QXX_SECTOR_SIZE;
    W25Qxx_EraseSector(flash_addr);
  }

  flash_addr=base_addr;
  flash_offset=0;
    
  for(int image_Y=0; image_Y<image_height; image_Y++)
  {  
    f_lseek(&image_file, image_offset+(image_height-image_Y-1)*bytePerLine);
    for(int image_X=0; image_X<image_width; image_X++)
    {
      f_read(&image_file,(char *)&image_pixel,image_bpp,&read_size);

      screen_pixel.RGB.r=image_pixel[2]>>3;
      screen_pixel.RGB.g=image_pixel[1]>>2;        
      screen_pixel.RGB.b=image_pixel[0]>>3;

      flash_buff[flash_offset++]=(u8)(screen_pixel.color>>8);
      flash_buff[flash_offset++]=(u8)(screen_pixel.color&0xFF);
      
      if(flash_offset == W25QXX_PAGE_SIZE)
      {
        W25Qxx_WritePage(flash_buff,flash_addr,W25QXX_PAGE_SIZE);
        flash_addr+=W25QXX_PAGE_SIZE;
        flash_offset=0;
      }
    }
  }    

  W25Qxx_WritePage(flash_buff,flash_addr,flash_offset);

  f_close(&image_file);

  return true;  
}  

void updateIcon(void)
{
  char file_path[64];  
  u32  flash_addr = 0;
  u8   progress_info = 0;
  u8   progress_unit = 0;
  char text_buff[128];

  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispString(100, 5, (u8*)"Logo Update...");

  if(bmpDecode(BMP_ROOT_DIR"/Logo.bmp", LOGO_ADDR))
  {
    LOGO_ReadDisplay();
    Delay_ms(2*1000); // preview time
  }

  if(bmpDecode(BMP_ROOT_DIR"/InfoBox.bmp", INFOBOX_ADDR))
  {
    ICON_CustomReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, INFOBOX_WIDTH, INFOBOX_HEIGHT,INFOBOX_ADDR);
  }

  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispString(100, 5, (u8*)"Icon Update...");

  for(int icon_index=0; icon_index<COUNT(icon_file_list); icon_index++)
  {
    my_sprintf(file_path, BMP_ROOT_DIR"/%s.bmp", icon_file_list[icon_index]);
    flash_addr = ICON_ADDR(icon_index);
    if(bmpDecode(file_path, flash_addr))
    {
      GUI_ClearRect(labelUpdateRect.x0, labelUpdateRect.y0, labelUpdateRect.x1, labelUpdateRect.y1);
      GUI_DispStringInPrect(&labelUpdateRect, (u8 *)file_path);
      ICON_ReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, icon_index);
    }
    progress_unit = flash_addr * 100 / FLASH_TOTAL_SIZE;
    if(progress_info != progress_unit) {
        progress_info = progress_unit;
        my_sprintf((void *)text_buff,"Flash used: %d %%   ", progress_unit);
        GUI_DispString(0, 50, (u8*)text_buff);
    }
  }

  Delay_ms(2*1000); // preview time

}

void updateFont(char *file_path, const u32 base_addr)
{

  FIL  file_data;
  UINT read_size = 0;
  u32  flash_addr = 0;
  u32  flash_offset = 0;
  u8*  flash_buff = NULL;
  u8   progress_info = 0;
  u8   progress_unit = 0;
  char text_buff[128];

  if (f_open(&file_data, file_path, FA_OPEN_EXISTING|FA_READ) != FR_OK)  return;

  FSIZE_t file_size = f_size(&file_data);

  flash_buff = malloc(W25QXX_SECTOR_SIZE);
  if (flash_buff == NULL)  return;

  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispString(100, 5, (u8*)"Font Update...");

  my_sprintf((void *)text_buff,"%s Size=%dK",file_path, (u32)file_size>>10);
  GUI_DispString(0, 100, (u8*)text_buff);
  GUI_DispString(0, 140, (u8*)"Progress:   %");

  while(!f_eof(&file_data))
  {
    if (f_read(&file_data, flash_buff, W25QXX_SECTOR_SIZE, &read_size) != FR_OK) break;
    flash_addr = base_addr + flash_offset;
    W25Qxx_EraseSector(flash_addr);
    W25Qxx_WriteBuffer(flash_buff, flash_addr, W25QXX_SECTOR_SIZE);
    flash_offset += read_size;
    progress_unit = flash_offset * 100 / file_size;
    if(progress_info != progress_unit)
    {
      progress_info = progress_unit;
      GUI_DispDec(0 + BYTE_WIDTH*9, 140, progress_info, 3, RIGHT);
    }
    if(read_size !=W25QXX_SECTOR_SIZE) break;
  }

  f_close(&file_data);
  free(flash_buff);
}

void scanResetDir(void)
{
  FIL resetfile;
  if (f_open(&resetfile, TFT_RESET_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    f_close(&resetfile);
    f_rename(TFT_RESET_FILE, TFT_RESET_FILE ".DONE");
    infoSettingsReset();
    TSC_Calibration();
    storePara();
  }
}

void scanUpdates(void)
{
  volatile u8 result = 0;   // must use volatile
  if(mountSDCard())
  {
    result = scanUpdateFile();
    if (result & HAS_FONT)
    {
      updateFont(FONT_ROOT_DIR"/byte_ascii.fon", BYTE_ASCII_ADDR);
      updateFont(FONT_ROOT_DIR"/word_unicode.fon", WORD_UNICODE_ADDR);
    }
    if (result & HAS_BMP) //bmp
    {
      updateIcon();
    }
    //if (result) f_rename(ROOT_DIR, ROOT_DIR".CUR");
    scanResetDir();
  }
}
