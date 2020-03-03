//
// define icon file list
//

#pragma once

// limit file name size
#define FILE_NAME_SIZE 32

// define index for icon name
enum {
#define X_IMAGE(NAME) ICON_##NAME,
#include "icon_list.inc"
#undef  X_IMAGE
    // keep last
    ICON_PREVIEW,
    ICON_EMPTY,
};

// report number of icon files
int icon_list_size();

// expose icon file names (no suffix)
const char (* icon_file_list())[FILE_NAME_SIZE];
