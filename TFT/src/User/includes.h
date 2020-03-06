//
//
//

#pragma once

#include "config.h"
#include "icon_list.h"

#include <AnyMenu.h>
#include <AnyPopup.h>
#include <ArkonGUI.h>
#include <ArkonMenu.h>
#include <ArkonMode.h>
#include <CoolerControl.h>
#include <FilamentControl.h>
#include <HeaterControl.h>
#include <HeaterPreset.h>
#include <HomingControl.h>
#include <HotbedBabyStep.h>
#include <HotbedLeveling.h>
#include <HotbedProbeOffset.h>
#include <InvokeGcode.h>
#include <list_view.h>
#include <ledcolor.h>
#include <ParameterControl.h>
#include <PositionControl.h>
#include <PrintControl.h>
#include "variants.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <TinkerFeedSpeed.h>
#include <UnifiedHeating.h>
#include <UnifiedMotion.h>
#include "my_misc.h"

#include "variants.h"
#include "os_timer.h"
#include "delay.h"

#include "boot.h"

#include "lcd.h"
#include "LCD_Init.h"
#include "lcd_dma.h"
#include "GUI.h"
#include "Language.h"
#include "utf8_decode.h"

#include "usart.h"
#include "Serial.h"
#include "spi.h"
#include "sw_spi.h"
#include "spi_slave.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

#include "sd.h"
#include "w25qxx.h"
#include "xpt2046.h"

#include "LCD_Encoder.h"
#include "ST7920_Simulator.h"
#include "ui_draw.h"
#include "touch_process.h"
#include "interfaceCmd.h"
#include "coordinate.h"
#include "ff.h"
#include "Vfs/vfs.h"
#include "myfatfs.h"
#include "Gcode/gcode.h"
#include "Gcode/mygcodefs.h"
#include "flashStore.h"
#include "parseACK.h"
#include "Selectmode.h"
#include "extend.h"

//menu
#include "Printing.h"
#include "More.h"
#include "Settings.h"
#include "ScreenSettings.h"
#include "MachineSettings.h"
#include "FeatureSettings.h"
#include "PowerFailed.h"


#define MAX_MENU_DEPTH 10       // max sub menu depth
typedef void (*FP_MENU)(void); 

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];  // Menu function buffer
  u8      cur;                   // Current menu index in buffer
}MENU;

extern MENU infoMenu;

typedef struct
{	
  bool wait;       //Whether wait for Marlin's response
  bool rx_ok[_USART_CNT]; //Whether receive Marlin's response or get Gcode by other UART(ESP3D/OctoPrint)
  bool connected;  //Whether have connected to Marlin
  bool printing;   //Whether the host is busy in printing execution. ( USB serial printing and GCODE print from onboard)
}HOST;

extern HOST infoHost;

