//
// symbols (character based icons) support
//

#pragma once

#include <stdint.h>

#include "lcd.h"
#include "my_misc.h"
#include "list_view.h"

// generate symbol index
enum {
#define X_STAMP(NAME,IMAGE,COLOR)   SYMBOL_##NAME ,
#include "symbol.inc"
#undef  X_STAMP
    // keep tail
    _SYMBOL_COUNT_,
    _SYMBOL_EMPTY_  // marker
};

extern const uint16_t symbol_mapper_color[_SYMBOL_COUNT_];
extern const char *const symbol_mapper_image[_SYMBOL_COUNT_];
extern const char *const symbol_mapper_name[_SYMBOL_COUNT_];

// resolve symbol image text from symbol index
uint8_t* symbol_image_by_index(const uint8_t symbol_index);

// resolve symbol enum index given symbol enum name
uint16_t symbol_index_by_name(const char *symbol_name);
