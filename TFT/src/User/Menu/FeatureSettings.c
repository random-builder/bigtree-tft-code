//
//
//

#include "FeatureSettings.h"
#include "includes.h"

LISTITEMS featureSettingsItems = {
// title
LABEL_FEATURE_SETTINGS,
// icon                 ItemType      Item Title        item value text(only for custom value)
{
  {_SYMBOL_EMPTY_, LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {_SYMBOL_EMPTY_, LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {_SYMBOL_EMPTY_, LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {_SYMBOL_EMPTY_, LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {_SYMBOL_EMPTY_, LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {SYMBOL_PAGEUP,     LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {SYMBOL_PAGEDOWN,   LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},
  {SYMBOL_BACK,       LIST_LABEL,  _LABEL_EMPTY_, _LABEL_EMPTY_},}
};

//
//setup item states
//
const uint16_t toggleitem[2] = {SYMBOL_TOGGLE_OFF,SYMBOL_TOGGLE_ON};
const  u8  item_toggleState[2]    = {0, 1};

#ifdef PS_ON_PIN
  #define ITEM_PS_ON_NUM 2
  const LISTITEM itemPowerOff[ITEM_PS_ON_NUM] = {
  // icon                 ItemType      Item Title        item value text(only for custom value)
    {SYMBOL_TOGGLE_OFF, LIST_TOGGLE,  LABEL_AUTO_SHUT_DOWN, _LABEL_EMPTY_},
    {SYMBOL_TOGGLE_ON,  LIST_TOGGLE,  LABEL_AUTO_SHUT_DOWN, _LABEL_EMPTY_},
  };
  const  u8 item_power_off[ITEM_PS_ON_NUM] = {0, 1};
  static u8  item_power_off_i = 0; //for auto power
#endif

#ifdef FIL_RUNOUT_PIN
  #define ITEM_RUNOUT_NUM 3
  const LISTITEM itemRunout[ITEM_RUNOUT_NUM] = {
// icon                 ItemType      Item Title        item value text(only for custom value)
    {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_FILAMENT_RUNOUT, LABEL_OFF},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_FILAMENT_RUNOUT, LABEL_ON},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_FILAMENT_RUNOUT, LABEL_SMART},
  };

  const  u8 item_runout[ITEM_RUNOUT_NUM] = {FILAMENT_RUNOUT_OFF, FILAMENT_RUNOUT_ON, FILAMENT_SMART_RUNOUT_ON};
  static u8  item_runout_i = 0; //for runout sensor
#endif

#define ITEM_SPEED_NUM 3
const LISTITEM itemMoveSpeed[ITEM_SPEED_NUM] = {
  // icon          ItemType           Item Title              item value text(only for custom value)
  {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_MOVE_SPEED, LABEL_NORMAL_SPEED},
  {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_MOVE_SPEED, LABEL_SLOW_SPEED},
  {SYMBOL_BLANK, LIST_CUSTOMVALUE,  LABEL_MOVE_SPEED, LABEL_FAST_SPEED},
};
const  u8 item_movespeed[ITEM_SPEED_NUM] = {LABEL_NORMAL_SPEED, LABEL_SLOW_SPEED, LABEL_FAST_SPEED};

#ifdef LED_color_PIN
  #define LED_color_NUM 9
  const LISTITEM itemLedcolor[LED_color_NUM] = {
  // icon                       label
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_OFF},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_WHITE},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_RED},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_ORANGE},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_YELLOW},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_GREEN},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_BLUE},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_INDIGO},
    {SYMBOL_BLANK, LIST_CUSTOMVALUE, LABEL_KNOB_LED, LABEL_VIOLET},
  };
  const  uint32_t led_color[LED_color_NUM] = {
                                    LED_OFF,
                                    LED_WHITE,
                                    LED_RED,
                                    LED_ORANGE,
                                    LED_YELLOW,
                                    LED_GREEN,
                                    LED_BLUE,
                                    LED_INDIGO,
                                    LED_VIOLET
                                    };

#endif
//
//add key number index of the items
//
typedef enum
{
  SKEY_HIDEACK = 0,
  SKEY_INVERT_Y,
  SKEY_INVERT_Z,
  #ifdef PS_ON_PIN
    SKEY_POWER,
  #endif
  #ifdef FIL_RUNOUT_PIN
    SKEY_RUNOUT,
  #endif
  SKEY_SPEED,
  #ifdef LED_color_PIN
  SKEY_KNOB,
  #endif
  SKEY_COUNT //keep this always at the end
}SKEY_LIST;

#define FE_PAGE_COUNT  (SKEY_COUNT+LISTITEM_PER_PAGE-1)/LISTITEM_PER_PAGE
int fe_cur_page = 0;

//
//set item types
//
LISTITEM settingPage[SKEY_COUNT] = {
  {SYMBOL_TOGGLE_ON,  LIST_TOGGLE,        LABEL_TERMINAL_ACK,       _LABEL_EMPTY_},
  {SYMBOL_TOGGLE_ON,  LIST_TOGGLE,        LABEL_INVERT_YAXIS,       _LABEL_EMPTY_},
  {SYMBOL_TOGGLE_ON,  LIST_TOGGLE,        LABEL_INVERT_ZAXIS,       _LABEL_EMPTY_},
  #ifdef PS_ON_PIN
  {SYMBOL_TOGGLE_ON,  LIST_TOGGLE,        LABEL_AUTO_SHUT_DOWN,     _LABEL_EMPTY_},
  #endif
  #ifdef FIL_RUNOUT_PIN
  {SYMBOL_TOGGLE_ON,  LIST_CUSTOMVALUE,   LABEL_FILAMENT_RUNOUT,    LABEL_OFF       },
  #endif
  {SYMBOL_TOGGLE_ON,  LIST_CUSTOMVALUE,   LABEL_MOVE_SPEED,         LABEL_NORMAL_SPEED},
  #ifdef LED_color_PIN
  {SYMBOL_BLANK,      LIST_CUSTOMVALUE,   LABEL_KNOB_LED,           LABEL_OFF       },
  #endif

};

//
//perform action on button press
//
void updateFeatureSettings(uint8_t key_val)
{
  uint8_t item_index = fe_cur_page*LISTITEM_PER_PAGE+ key_val;
  switch (item_index)
  {
    case SKEY_HIDEACK:
    infoSettings.terminalACK = (infoSettings.terminalACK + 1) % 2;
    settingPage[item_index].icon = toggleitem[infoSettings.terminalACK];
    featureSettingsItems.items[key_val].icon = toggleitem[infoSettings.terminalACK];
    break;

    case SKEY_INVERT_Y:
    infoSettings.invert_yaxis = (infoSettings.invert_yaxis + 1) % 2;
    settingPage[item_index].icon = toggleitem[infoSettings.invert_yaxis];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    break;

    case SKEY_INVERT_Z:
    infoSettings.invert_zaxis = (infoSettings.invert_zaxis + 1) % 2;
    settingPage[item_index].icon = toggleitem[infoSettings.invert_zaxis];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    break;

    #ifdef PS_ON_PIN
    case SKEY_POWER:
    item_power_off_i = (item_power_off_i + 1) % ITEM_PS_ON_NUM;
    settingPage[item_index] = itemPowerOff[item_power_off_i];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    infoSettings.auto_off = item_power_off[item_power_off_i];
    break;
    #endif

    #ifdef FIL_RUNOUT_PIN
    case SKEY_RUNOUT:
    item_runout_i = (item_runout_i + 1) % ITEM_RUNOUT_NUM;
    settingPage[item_index] = itemRunout[item_runout_i];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    infoSettings.runout = item_runout[item_runout_i];
    break;
    #endif

    case SKEY_SPEED:
    infoSettings.move_speed = (infoSettings.move_speed + 1) % ITEM_SPEED_NUM;
    settingPage[item_index] = itemMoveSpeed[infoSettings.move_speed];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    break;

    #ifdef LED_color_PIN
    case SKEY_KNOB:
    infoSettings.led_color = (infoSettings.led_color + 1) % LED_color_NUM;
    settingPage[item_index] = itemLedcolor[infoSettings.led_color];
    featureSettingsItems.items[key_val] = settingPage[item_index];
    ws2812_send_DAT(led_color[infoSettings.led_color]);
    break;
    #endif

  default:
    break;
  }
}

//
//load values on page change and reload
//
void loadFeatureSettings(){
  for (uint8_t i = 0; i < LISTITEM_PER_PAGE; i++)
  {
    uint8_t item_index = fe_cur_page*LISTITEM_PER_PAGE + i;
    switch (item_index)
    {
      case SKEY_HIDEACK:
        //item_terminalACK_i = infoSettings.terminalACK;
        settingPage[item_index].icon = toggleitem[infoSettings.terminalACK];
        featureSettingsItems.items[i] = settingPage[item_index];
        break;

      case SKEY_INVERT_Y:
        //item_invert_yaxis_i = infoSettings.invert_yaxis;
        settingPage[item_index].icon = toggleitem[infoSettings.invert_yaxis];
        featureSettingsItems.items[i] = settingPage[item_index];
        break;

      case SKEY_INVERT_Z:
        //item_invert_zaxis_i = infoSettings.invert_zaxis;
        settingPage[item_index].icon = toggleitem[infoSettings.invert_zaxis];
        featureSettingsItems.items[i] = settingPage[item_index];
        break;

      #ifdef PS_ON_PIN
        case SKEY_POWER:
          for (u8 n = 0; n < ITEM_PS_ON_NUM; n++)
          {
            if (infoSettings.auto_off == item_power_off[n])
            { item_power_off_i = n; }
          }
          featureSettingsItems.items[i] = itemPowerOff[item_power_off_i];
        break;
      #endif

      #ifdef FIL_RUNOUT_PIN
        case SKEY_RUNOUT:
          for (u8 n = 0; n < ITEM_RUNOUT_NUM; n++)
          {
            if (infoSettings.runout == item_runout[n])
            { item_runout_i = n; }
          }
          featureSettingsItems.items[i] = itemRunout[item_runout_i];
        break;
      #endif

      case SKEY_SPEED:
          featureSettingsItems.items[i] = itemMoveSpeed[infoSettings.move_speed];
        break;

      #ifdef LED_color_PIN
        case SKEY_KNOB:
        settingPage[item_index] = itemLedcolor[infoSettings.led_color];
        featureSettingsItems.items[i] = settingPage[item_index];
        break;
      #endif

      default:
        settingPage[item_index].icon = _SYMBOL_EMPTY_;
        featureSettingsItems.items[i] = settingPage[item_index];
      break;
    }
 }
  // set page up down button according to page count and current page
  if (SKEY_COUNT <= LISTITEM_PER_PAGE)
  {
    featureSettingsItems.items[5].icon = _SYMBOL_EMPTY_;
    featureSettingsItems.items[6].icon = _SYMBOL_EMPTY_;
  }
  else
  {
    if(fe_cur_page == 0){
      featureSettingsItems.items[5].icon = _SYMBOL_EMPTY_;
      featureSettingsItems.items[6].icon = SYMBOL_PAGEDOWN;
    }
    else if(fe_cur_page == (FE_PAGE_COUNT-1)){
      featureSettingsItems.items[5].icon = SYMBOL_PAGEUP;
      featureSettingsItems.items[6].icon = _SYMBOL_EMPTY_;
    }
    else
    {
      featureSettingsItems.items[5].icon = SYMBOL_PAGEUP;
      featureSettingsItems.items[6].icon = SYMBOL_PAGEDOWN;
    }
  }
  //menuDrawListItem(&featureSettingsItems.items[5],5);
  //menuDrawListItem(&featureSettingsItems.items[6],6);

}


void menuFeatureSettings(void)
{
  KEY_VALUES key_num = KEY_IDLE;
  SETTINGS now = infoSettings;
  fe_cur_page = 0;
  loadFeatureSettings();
  menuDrawListPage(&featureSettingsItems);

  while (infoMenu.menu[infoMenu.cur] == menuFeatureSettings)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
    case KEY_ICON_5:
      if(FE_PAGE_COUNT > 1){
        if (fe_cur_page > 0){
          fe_cur_page--;
          loadFeatureSettings();
          menuRefreshListPage();
        }
      }
      break;

    case KEY_ICON_6:
      if(FE_PAGE_COUNT > 1){
        if (fe_cur_page < FE_PAGE_COUNT - 1){
          fe_cur_page++;
          loadFeatureSettings();
          menuRefreshListPage();
        }
      }
      break;

    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default:
      if(key_num < LISTITEM_PER_PAGE){
      updateFeatureSettings(key_num);
      menuDrawListItem(&featureSettingsItems.items[key_num],key_num);
      }
      break;
    }

  loopProcess();
}

  if(memcmp(&now, &infoSettings, sizeof(SETTINGS)))
  {
    storePara();
  }
}
