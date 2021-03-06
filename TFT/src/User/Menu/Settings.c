#include "Settings.h"
#include "includes.h"

SETTINGS infoSettings;

// Reset settings data
void infoSettingsReset(void)
{
  infoSettings.baudrate = BAUDRATE;
  infoSettings.language = DEFAULT_LANGUAGE;
  infoSettings.mode = DEFAULT_LCD_MODE;
  infoSettings.runout = 0;
  infoSettings.rotate_ui = 0;
  infoSettings.bg_color = ST7920_BKCOLOR;
  infoSettings.font_color = ST7920_FNCOLOR;
  infoSettings.silent = 0;
  infoSettings.auto_off = 0;
  infoSettings.terminalACK = 0;
  infoSettings.invert_axis[X_AXIS] = 0;
  infoSettings.invert_axis[Y_AXIS] = 0;
  infoSettings.invert_axis[Z_AXIS] = 0;
  infoSettings.move_speed = 0;
  infoSettings.knob_led_color = (STARTUP_KNOB_LED_COLOR - 1); 
  infoSettings.send_start_gcode = 1;
  infoSettings.send_end_gcode = 1;
  infoSettings.persistent_info = 1;
  infoSettings.file_listmode = 1;
}

// Version infomation
void menuInfo(void)
{
  const char* hardware = "Board   : BIGTREETECH_" HARDWARE_VERSION;
  const char* firmware = "Firmware: "HARDWARE_VERSION"." STRINGIFY(SOFTWARE_VERSION) " " __DATE__;

  u16 HW_X = (LCD_WIDTH - GUI_StrPixelWidth((u8 *)hardware))/2;
  u16 FW_X = (LCD_WIDTH - GUI_StrPixelWidth((u8 *)firmware))/2;
  u16 centerY = LCD_HEIGHT/2;
  u16 startX = MIN(HW_X, FW_X);

  GUI_Clear(BACKGROUND_COLOR);

  GUI_DispString(startX, centerY - BYTE_HEIGHT, (u8 *)hardware);
  GUI_DispString(startX, centerY, (u8 *)firmware);
  GUI_DispStringInRect(20, LCD_HEIGHT - (BYTE_HEIGHT*2), LCD_WIDTH-20, LCD_HEIGHT, language_text(LABEL_TOUCH_TO_EXIT));

  while(!isPress()) loopProcess();
  while(isPress())  loopProcess();

  infoMenu.cur--;
}

// Set uart pins to input, free uart
void menuDisconnect(void)
{
  GUI_Clear(BACKGROUND_COLOR);
  GUI_DispStringInRect(20, 0, LCD_WIDTH-20, LCD_HEIGHT, language_text(LABEL_DISCONNECT_INFO));
  GUI_DispStringInRect(20, LCD_HEIGHT - (BYTE_HEIGHT*2), LCD_WIDTH-20, LCD_HEIGHT, language_text(LABEL_TOUCH_TO_EXIT));

  Serial_DeInit();
  while(!isPress());
  while(isPress());
  Serial_Init(infoSettings.baudrate);

  infoMenu.cur--;
}

MENUITEMS settingsItems = {
// title
LABEL_SETTINGS,
// icon                       label
{{ICON_TSCSettings,         LABEL_SCREEN_SETTINGS},
 {ICON_MachineSettings,     LABEL_MACHINE_SETTINGS},
 {ICON_FeatureSettings,     LABEL_FEATURE_SETTINGS},
 {_ICON_EMPTY_,             _LABEL_EMPTY_},
 {ICON_Disconnect,          LABEL_DISCONNECT},
 {ICON_BaudRate,            LABEL_BAUDRATE_115200},
 {ICON_More,                LABEL_DEBUG},
 {ICON_Back,                LABEL_BACK},}
};

#define ITEM_BAUDRATE_NUM 9
const ITEM itemBaudrate[ITEM_BAUDRATE_NUM] = {
// icon                       label
  {ICON_BaudRate,             {.address = "2400"}},
  {ICON_BaudRate,             {.address = "9600"}},
  {ICON_BaudRate,             {.address = "19200"}},
  {ICON_BaudRate,             {.address = "38400"}},
  {ICON_BaudRate,             {.address = "57600"}},
  {ICON_BaudRate,             {.address = "115200"}},
  {ICON_BaudRate,             {.address = "250000"}},
  {ICON_BaudRate,             {.address = "500000"}},
  {ICON_BaudRate,             {.address = "1000000"}},
};
const  u32 item_baudrate[ITEM_BAUDRATE_NUM] = {2400, 9600, 19200, 38400, 57600, 115200, 250000, 500000, 1000000};
static u8  item_baudrate_i = 0;

void menuSettings(void)
{
  KEY_VALUE key_num = KEY_IDLE;
  SETTINGS now = infoSettings;

  for(u8 i=0; i<ITEM_BAUDRATE_NUM; i++)
  {
    if(infoSettings.baudrate == item_baudrate[i])
    {
      item_baudrate_i = i;
      settingsItems.items[KEY_ICON_5] = itemBaudrate[item_baudrate_i];
    }
  }

  menuDrawPage(&settingsItems);

  while(infoMenu.menu[infoMenu.cur] == menuSettings)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.cur] = menuScreenSettings;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuMachineSettings;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.cur] = menuFeatureSettings;
        break;

      case KEY_ICON_3:
        infoMenu.menu[++infoMenu.cur] = menuInfo;
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuDisconnect;
        break;

      case KEY_ICON_5:
        item_baudrate_i = (item_baudrate_i + 1) % ITEM_BAUDRATE_NUM;
        settingsItems.items[key_num] = itemBaudrate[item_baudrate_i];
        menuDrawItem(&settingsItems.items[key_num], key_num);
        infoSettings.baudrate = item_baudrate[item_baudrate_i];
        Serial_DeInit(); // Serial_Init() will malloc a dynamic memory, so Serial_DeInit() first to free, then malloc again.
        Serial_Init(infoSettings.baudrate);
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default:
        break;
    }
    loopProcess();
  }

  if(memcmp(&now, &infoSettings, sizeof(SETTINGS)))
  {
    storePara();
  }
}
