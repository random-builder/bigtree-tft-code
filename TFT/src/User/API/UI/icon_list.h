//
// define icon file list
//

#pragma once

// limit file name size
#define FILE_NAME_SIZE 32

// generate index for icon name
enum {
#define X_IMAGE(NAME) ICON_##NAME ,
#include "icon_list.inc"
#undef  X_IMAGE
    // keep tail
    _ICON_VIEW_,  // marker
    _ICON_EMPTY_,  // marker
};

// report number of icon files
int icon_list_size();

// expose icon file names (no suffix)
const char (* icon_file_list())[FILE_NAME_SIZE];

// find icon index by name (no suffix)
int icon_find_index(const char *icon_name);

