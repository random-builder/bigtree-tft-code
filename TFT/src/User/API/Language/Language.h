//
// language pack generator
//

#pragma once

#include "stdbool.h"
#include "variants.h"

enum {
    LANG_ENGLISH = 0,
    LANG_CHINESE,
    LANG_RUSSIAN,
    LANG_JAPANESE,
    LANG_ARMENIAN,
    LANG_GERMAN,
    LANG_CZECH,
    LANG_SPAIN,
    LANG_FRENCH,
    LANG_PORTUGUESE,
    LANG_ITALIAN,
    LANG_POLISH,
    LANG_SLOVAK,
    LANG_DUTCH,
    // keep tail
    _LANG_COUNT_,
};

// generate index for dictionary word labels
enum {
#define X_WORD(NAME) LABEL_##NAME ,
#include "Language.inc"
#undef  X_WORD
    //keep tail
    _LABEL_COUNT_,
    _LABEL_EMPTY_,  // marker
    _LABEL_DYNAMIC_,  // marker
};

u8* language_text(const u8 label_index);

