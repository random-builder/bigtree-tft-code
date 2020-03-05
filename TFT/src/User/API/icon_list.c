//
// define icon file list
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "icon_list.h"
#include "my_misc.h"

// define icon file list (no suffix)
const char icon_file_store[][FILE_NAME_SIZE] = {
#define X_IMAGE(NAME) STRINGIFY(NAME) ,
#include "icon_list.inc"
#undef  X_IMAGE
        };

// report number of icon files
int icon_list_size() {
    return COUNT(icon_file_store);
}

// find icon index by name (no suffix)
int icon_find_index(const char *icon_name) {

    const int icon_count = icon_list_size();
    const char (*icon_name_list)[FILE_NAME_SIZE] = icon_file_list();

    for (int icon_index = 0; icon_index < icon_count; icon_index++) {
        if (strcmp(icon_name, icon_name_list[icon_index]) == 0) {
            return icon_index;
        }
    }

    return 0;
}

// expose icon file names (no suffix)
const char (* icon_file_list())[FILE_NAME_SIZE] {
            return icon_file_store;
        }
