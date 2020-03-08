//
// select file for print
//

#include <PrintControl.h>
#include "includes.h"

  LISTITEMS printItems = {
  // title
  _LABEL_EMPTY_,
  // icon                 ItemType      Item Title        item value text(only for custom value)
  {
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {_SYMBOL_EMPTY_, LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},
    {SYMBOL_BACK,       LIST_LABEL, _LABEL_EMPTY_, _LABEL_EMPTY_},}
  };

// File list number per page
#define NUM_PER_PAGE	5

SCROLL   titleScroll;
GUI_RECT titleRect={10, (TITLE_END_Y - BYTE_HEIGHT) / 2, LCD_WIDTH-10, (TITLE_END_Y - BYTE_HEIGHT) / 2 + BYTE_HEIGHT};

SCROLL   gcodeScroll;
  
void normalNameDisp(const GUI_RECT *rect, u8 *name)
{
  if(name == NULL) return;

  GUI_ClearPrect(rect);
  GUI_SetRange(rect->x0, rect->y0, rect->x1, rect->y1);
  GUI_DispString(rect->x0, rect->y0 + (rect->y1 - rect->y0 - BYTE_HEIGHT)/2, name);
  GUI_CancelRange();
}

u8 show_Num = 0;
bool icon_pre = true;
int16_t icon_Enum[25];
char * icon_File_Name[25];

int16_t get_Pre_Icon(char * filename)
{
  for(u8 i=0;i<show_Num;i++)
  {
    if(strcmp(icon_File_Name[i],filename)==0)
    return icon_Enum[i];
  }
  return _ICON_EMPTY_;
}

  void gocdeListDraw(void)
  {
    u8 i = 0,k = 0;
    int gn;
    char *gnew;

    //scrollFileNameCreate(0);
    Scroll_CreatePara(&titleScroll, (u8 *)infoFile.title, &titleRect);
    printItems.title.address = (u8 *)infoFile.title;
    GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
    GUI_ClearRect(0, 0, LCD_WIDTH, TITLE_END_Y);
    GUI_SetBkColor(BACKGROUND_COLOR);

    for (i = 0; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.F_num) && (i < NUM_PER_PAGE); i++) // folder
    {
      printItems.items[i].icon = SYMBOL_FOLDER;
      dynamic_label[i] = infoFile.folder[i + infoFile.cur_page * NUM_PER_PAGE];
      printItems.items[i].titlelabel.index = _LABEL_DYNAMIC_;
      menuDrawListItem(&printItems.items[i], i);
    }
    for (; (i + infoFile.cur_page * NUM_PER_PAGE < infoFile.f_num + infoFile.F_num) && (i < NUM_PER_PAGE); i++) // gcode file
    {
      printItems.items[i].icon = SYMBOL_FILE;
      dynamic_label[i] = (infoFile.source == BOARD_SD) ? infoFile.Longfile[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num] : infoFile.file[i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num];
      printItems.items[i].titlelabel.index = _LABEL_DYNAMIC_;
      menuDrawListItem(&printItems.items[i], i);

      k = i + infoFile.cur_page * NUM_PER_PAGE - infoFile.F_num;
      gn = strlen(infoFile.file[k]) - 6; // -6 means ".gcode"
      gnew = malloc(gn + 10);

      if(gnew && get_Pre_Icon(infoFile.file[k]) == _ICON_EMPTY_)
      {

        strcpy(gnew, getCurFileSource());
        strncat(gnew, infoFile.file[k], gn);
        if(bmpDecode(strcat(gnew, "_"STRINGIFY(ICON_WIDTH)".bmp"), ICON_ADDR(_ICON_VIEW_+show_Num)))
        {
          icon_File_Name[show_Num]=infoFile.file[k];
          icon_Enum[show_Num]=_ICON_VIEW_+show_Num;
          show_Num++;
        }
        free(gnew);
      }
    }

    for (; (i < NUM_PER_PAGE); i++) //background
    {
      printItems.items[i].icon = _SYMBOL_EMPTY_;
      printItems.items[i].titlelabel.index = _LABEL_EMPTY_;
      menuDrawListItem(&printItems.items[i], i);
    }
      // set page up down button according to page count and current page
      int t_pagenum = (infoFile.F_num+infoFile.f_num+(LISTITEM_PER_PAGE-1))/LISTITEM_PER_PAGE;
      if ((infoFile.F_num+infoFile.f_num) <= LISTITEM_PER_PAGE)
      {
        printItems.items[5].icon = _SYMBOL_EMPTY_;
        printItems.items[6].icon = _SYMBOL_EMPTY_;
      }
      else
      {
        if(infoFile.cur_page == 0){
          printItems.items[5].icon = _SYMBOL_EMPTY_;
          printItems.items[6].icon = SYMBOL_PAGEDOWN;
        }
        else if(infoFile.cur_page == (t_pagenum-1)){
          printItems.items[5].icon = SYMBOL_PAGEUP;
          printItems.items[6].icon = _SYMBOL_EMPTY_;
        }
        else
        {
          printItems.items[5].icon = SYMBOL_PAGEUP;
          printItems.items[6].icon = SYMBOL_PAGEDOWN;
        }
      }
      menuDrawListItem(&printItems.items[5],5);
      menuDrawListItem(&printItems.items[6],6);
  }

const int16_t labelVolumeError[] = {LABEL_READ_TFTSD_ERROR, LABEL_READ_U_DISK_ERROR, LABEL_READ_ONBOARDSD_ERROR};

void menuPrintFromSource(void)
{
  KEY_VALUE key_num = KEY_IDLE;

  u8 update=0;

  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, language_text(LABEL_LOADING));

  if (mountFS() == true && scanPrintFiles() == true)
  {
      menuDrawListPage(&printItems);
    gocdeListDraw();	
  }
  else
  {
    GUI_DispStringInRect(0, 0, LCD_WIDTH, LCD_HEIGHT, language_text(labelVolumeError[infoFile.source]));
    Delay_ms(1000);
    infoMenu.cur--;
  }

  while(infoMenu.menu[infoMenu.cur] == menuPrintFromSource)
  {
    GUI_SetBkColor(TITLE_BACKGROUND_COLOR);
    Scroll_DispString(&titleScroll, LEFT);    //
    GUI_SetBkColor(BACKGROUND_COLOR);
    Scroll_DispString(&gcodeScroll, CENTER); //

    key_num = menuKeyGetValue();

    switch(key_num)
    {
      case KEY_ICON_5:			
        if(infoFile.cur_page > 0)
        {
          infoFile.cur_page--;
          update=1;
        }
        break;

      case KEY_ICON_6:	
        if(infoFile.cur_page+1 < (infoFile.F_num+infoFile.f_num+(NUM_PER_PAGE-1))/NUM_PER_PAGE)
        {
          infoFile.cur_page++;
          update=1;
        }	
        break;

      case KEY_ICON_7:
        infoFile.cur_page = 0;
        if(IsRootDir() == true)
        {
          clearInfoFile();
          infoMenu.cur--;
          break;
        }
        else
        {
          ExitDir();				
          scanPrintFiles();			
          update = 1;
        }
        break;

      case KEY_IDLE:
        break;

      default:                   
        if(key_num <= KEY_ICON_4)
        {	
          u16 start = infoFile.cur_page * NUM_PER_PAGE;
          if(key_num + start < infoFile.F_num)						//folder
          {
            if(EnterDir(infoFile.folder[key_num + start]) == false)  break;						
            scanPrintFiles();
            update=1;
            infoFile.cur_page=0;
          }
          else if(key_num+start < infoFile.F_num+infoFile.f_num)	//gcode
          {	
            if(infoHost.connected !=true) break;
            if(EnterDir(infoFile.file[key_num + start - infoFile.F_num]) == false) break;	
            
            infoMenu.menu[++infoMenu.cur] = menuBeforePrinting;	
          }				
        }
        break;
    }
    
    if(update)
    {
      update=0;
      gocdeListDraw();
    }
    
    #ifdef SD_CD_PIN
    if(isVolumeExist(infoFile.source) != true)
    {
      resetInfoFile();
      infoMenu.cur--;
    }
    #endif
    loopProcess();
  }
}

MENUITEMS sourceSelItems = {
//  title
LABEL_PRINT,
// icon                       label
 {{ICON_OnTFTSD,              LABEL_TFTSD},
 #ifdef ONBOARD_SD_SUPPORT
  {ICON_OnBoardSD,            LABEL_ONBOARDSD},
 #endif
 #ifdef U_DISK_SUPPROT
  {ICON_U_Disk,               LABEL_U_DISK},
 #else
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
 #endif
 #ifndef ONBOARD_SD_SUPPORT
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
 #endif
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Back,                 LABEL_BACK},}
};

void menuPrint(void)
{
  KEY_VALUE  key_num = KEY_IDLE;
  
  menuDrawPage(&sourceSelItems);
  while(infoMenu.menu[infoMenu.cur] == menuPrint)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
        infoFile.source = TFT_SD;
        infoMenu.menu[++infoMenu.cur] = menuPrintFromSource;
        infoMenu.menu[++infoMenu.cur] = menuPowerOff;
        goto selectEnd;
      
      #ifdef ONBOARD_SD_SUPPORT
      case KEY_ICON_1:
        infoFile.source = BOARD_SD;
        infoMenu.menu[++infoMenu.cur] = menuPrintFromSource;   //TODO: fix here,  onboard sd card PLR feature
        goto selectEnd;
      #endif
      
      #ifdef U_DISK_SUPPROT
      #ifdef ONBOARD_SD_SUPPORT
      case KEY_ICON_2:
      #else
      case KEY_ICON_1:
      #endif
        infoFile.source = TFT_UDISK;
        infoMenu.menu[++infoMenu.cur] = menuPrintFromSource;
        infoMenu.menu[++infoMenu.cur] = menuPowerOff;
        goto selectEnd;
      #endif
      
      case KEY_ICON_7:
        infoMenu.cur--;
        return;
      
      default: break;
    }
    loopProcess();
  }
  
selectEnd:  
  resetInfoFile();
  powerFailedSetDriverSource(getCurFileSource());
}
