//
// define icon file list
//

#include "icon_list.h"
#include "my_misc.h"

// define icon file list (no suffix)
const char icon_file_store[][FILE_NAME_SIZE] = {
#define X_IMAGE(NAME) STRINGIFY(NAME),
#include "icon_list.x"
#undef  X_IMAGE
        };

// report number of icon files
int icon_list_size() {
    return COUNT(icon_file_store);
}

// expose icon file names (no suffix)
const char (* icon_file_list())[FILE_NAME_SIZE] {
            return icon_file_store;
        }
