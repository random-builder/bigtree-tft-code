//
// symbols (character based icons) support
//

#include <string.h>

#include "symbol.h"

// generate symbol color mapper (by symbol index)
const uint16_t symbol_mapper_color[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) COLOR ,
#include "symbol.inc"
#undef  X_STAMP
        };

// generate symbol image mapper (by symbol index)
const char *const symbol_mapper_image[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) IMAGE ,
#include "symbol.inc"
#undef  X_STAMP
        };

// generate symbol name mapper (by symbol index)
const char *const symbol_mapper_name[_SYMBOL_COUNT_] = {
#define X_STAMP(NAME,IMAGE,COLOR) STRINGIFY(SYMBOL_##NAME) ,
#include "symbol.inc"
#undef  X_STAMP
        };

// resolve symbol image text from symbol index
uint8_t* symbol_image_by_index(const uint8_t symbol_index) {
    return (uint8_t*) symbol_mapper_image[symbol_index];
}

// resolve symbol enum index given symbol enum name
uint16_t symbol_index_by_name(const char *symbol_name) {
    for (uint16_t symbol_index = 0; symbol_index < _SYMBOL_COUNT_; symbol_index++) {
        if (strcmp(symbol_name, symbol_mapper_name[symbol_index]) == 0) {
            return symbol_index;
        }
    }
    return SYMBOL_ERROR;  // display error for invalid symbol name
}
