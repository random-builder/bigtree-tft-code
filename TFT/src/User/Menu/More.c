#include "includes.h"
#include "More.h"

void menuIsPause(void)
{
  u16 key_num = IDLE_TOUCH;	

  popupDrawPage(bottomDoubleBtn, language_text(LABEL_WARNING), language_text(LABEL_IS_PAUSE), language_text(LABEL_CONFIRM), language_text(LABEL_CANNEL));
 
  while(infoMenu.menu[infoMenu.cur] == menuIsPause)
  {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch(key_num)
    {
      case KEY_POPUP_CONFIRM:
				if(setPrintPause(true,false))
			    infoMenu.menu[infoMenu.cur]=menuFilament;
			  break;

      case KEY_POPUP_CANCEL:	
        infoMenu.cur--;
        break;		
    }
    loopProcess();
  }
}

MENUITEMS moreItems = {
//  title
LABEL_MORE,
// icon                       label
 {{ICON_Fan,                  LABEL_FAN},
  {ICON_RGB_Settings,         LABEL_RGB_SETTINGS},
  {ICON_Extrude,              LABEL_FILAMENT},
  {ICON_FeatureSettings,      LABEL_FEATURE_SETTINGS},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Back,                 LABEL_BACK},}
};

void menuMore(void)
{
  KEY_VALUES  key_num = KEY_IDLE;

  menuDrawPage(&moreItems);
  while(infoMenu.menu[infoMenu.cur] == menuMore)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.cur] = menuCoolerControl;
        break;
      
      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
        break;
      
      case KEY_ICON_2:
				if (isPrinting() && !isPause()) // need paused before extrude
				  infoMenu.menu[++infoMenu.cur] = menuIsPause;
				else
				  infoMenu.menu[++infoMenu.cur] = menuFilament; 
        break;
      
      case KEY_ICON_3:
        infoMenu.menu[++infoMenu.cur] = menuFeatureSettings;
        break;
      
      case KEY_ICON_7:
        infoMenu.cur--;
        break;
      
      default:
        break;            
    }
    loopProcess();
  }
}
