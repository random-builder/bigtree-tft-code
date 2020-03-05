#include <HotbedProbeOffset.h>
#include "includes.h"

//1 titl, ITEM_PER_PAGE item
MENUITEMS probeOffsetItems = {
// title
LABEL_PROBE_OFFSET,
// icon                        label
 {
  {ICON_Dec,                  LABEL_DEC},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_Inc,                  LABEL_INC},
  {_ICON_EMPTY_,           _LABEL_EMPTY_},
  {ICON_EEPROMSave,           LABEL_EEPROM_SAVE},
  {ICON_Mmm_01,               LABEL_01_MM},
  {ICON_Normal,               LABEL_VALUE_ZERO},
  {ICON_Back,                 LABEL_BACK},
 }
};

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
}ELEMENTS;

#define ITEM_PROBE_OFFSET_UNIT_NUM 3
const ITEM itemProbeOffsetUnit[ITEM_PROBE_OFFSET_UNIT_NUM] = {
// icon                       label
  {ICON_Mmm_001,               LABEL_001_MM},
  {ICON_Mmm_01,                LABEL_01_MM},
  {ICON_Mmm_1,                 LABEL_1_MM},
};
const float item_probeOffset_unit[ITEM_PROBE_OFFSET_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;

static void initElements(u8 position)
{
  elementsUnit.totaled = ITEM_PROBE_OFFSET_UNIT_NUM;
  elementsUnit.list = itemProbeOffsetUnit;
  elementsUnit.ele  = item_probeOffset_unit;

  for(u8 i=0; i<elementsUnit.totaled; i++)
  {
    if(memcmp(&elementsUnit.list[i], &probeOffsetItems.items[position], sizeof(ITEM)) == 0)
    {
      elementsUnit.cur = i;
      break;
    }
  }
}

static float probe_offset_value=0.0;

#define PROBE_OFFSET_MAX_VALUE 20.0f
#define PROBE_OFFSET_MIN_VALUE -20.0f

void showProbeOffset(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, probe_offset_value, 3, 2, RIGHT);
}

void probeOffsetReDraw(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, probe_offset_value, 3, 2, RIGHT);
}

/* Set current offset */
void setCurrentOffset(float offset)
{
  //probe_offset_value = limitValue(PROBE_OFFSET_MIN_VALUE, offset, PROBE_OFFSET_MAX_VALUE);
  probe_offset_value = offset;
}

void menuProbeOffset(void)
{
  KEY_VALUES key_num = KEY_IDLE;
  float now = probe_offset_value;
  initElements(KEY_ICON_5);
  menuDrawPage(&probeOffsetItems);
  showProbeOffset();
  while(infoMenu.menu[infoMenu.cur] == menuProbeOffset)
  {
    storeCmd("M851\n");
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: // Decrease
        if(probe_offset_value - elementsUnit.ele[elementsUnit.cur] > PROBE_OFFSET_MIN_VALUE)
        {
          if(storeCmd("M851 Z%.2f\n",probe_offset_value-elementsUnit.ele[elementsUnit.cur]))
            probe_offset_value -= elementsUnit.ele[elementsUnit.cur];
        }
        break;
      case KEY_ICON_2: // Increase
        if(probe_offset_value + elementsUnit.ele[elementsUnit.cur] < PROBE_OFFSET_MAX_VALUE)
        {
          if(storeCmd("M851 Z%.2f\n",probe_offset_value+elementsUnit.ele[elementsUnit.cur]))
            probe_offset_value += elementsUnit.ele[elementsUnit.cur];
        }
        break;
      case KEY_ICON_4: // Save to PROM
        storeCmd("M500\n");
        break;
      case KEY_ICON_5: // Delta size
        elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
        probeOffsetItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
        menuDrawItem(&probeOffsetItems.items[key_num], key_num);
        break;
      case KEY_ICON_6: // Reset to zero
        if(storeCmd("M851 Z%.2f\n",0))
          probe_offset_value = 0.0f;
        break;
      case KEY_ICON_7:
        infoMenu.cur--;
        break;
      default :
        break;
      }
    if(now != probe_offset_value)
    {
      now = probe_offset_value;
      probeOffsetReDraw();
    }
    loopProcess();
  }
}





