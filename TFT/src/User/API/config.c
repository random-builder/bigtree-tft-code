//
// config file support
//

// configure parser
#define INI_USE_STACK 0
#define INI_MAX_LINE 256
#include "ini.h"

#include "config.h"
#include "includes.h"
#include "Configuration.h"

// declare global instance and populate with default values
SYSTEM_CONFIG system_config = {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE)   DEFAULT_VALUE,
#include "config.inc"
#undef  X_ENTRY
        };

// generate config parser event reactor; see ini_handler
int config_handler(void *user, const char *section, const char *name, const char *value) {
    SYSTEM_CONFIG *config = (SYSTEM_CONFIG*) user;
    if (0) {
    }
    #define X_ENTRY(SECTION, NAME, DEFAULT_VALUE) else if \
    (strcmp(section, #SECTION)==0 && strcmp(name, #NAME)==0) \
    { config->CONFIG_ENTRY(SECTION,NAME) = strdup(value); }
#include "config.inc"
#undef  X_ENTRY
    else {
        return 0;  // failure: wrong section/name
    }
    return 1;  // success: match was found
}

// fgets-style flash stream buffer reader; see ini_reader
// https://en.wikibooks.org/wiki/C_Programming/stdio.h/fgets
char* config_reader(char *string, int length, void *stream) {
    FLASH_STREAM *flask = (FLASH_STREAM*) stream;
    char *buffer = string;  // mutable pointer
    char letter;  // current character

    if (length < 2) {  // expect at least "\n\0"
        return NULL;  // failure
    }
    if (length > INI_MAX_LINE) {  // expect limited lines
        return NULL;  // failure
    }
    if (length > flask->buff_size) {  // expect limited lines
        return NULL;  // failure
    }
    if (flask->flash_addr >= flask->flash_tail) {  // can not move past file end
        return NULL;  // failure
    }

    while (true) {
        // fill text buffer when empty
        if (flask->text_point == NULL) {  // buffer is empty
            W25Qxx_ReadBuffer(flask->text_buffer, flask->flash_addr, flask->buff_size);
            flask->flash_addr += flask->buff_size;
            flask->text_point = flask->text_buffer;
        }
        // copy letters while present
        while (length > 1) {
            length--;
            letter = *flask->text_point++;
            *buffer++ = letter;
            if (letter == '\n') {  // found end of line
                break;
            }
            if (letter == 0xFF) {  // found end of flash
                buffer--;  // move back to discard marker
                break;
            }
            if (flask->text_point >= flask->text_buffer + flask->buff_size) {
                flask->text_point = NULL;  // mark buffer drained
                break;
            }
        }
        if (length <= 1 || letter == '\n' || letter == 0xFF) {
            break;  // found complete line
        }
    }

    if (letter == 0xFF && buffer == string) {
        return NULL;  // found end of file
    }

    *buffer = '\0';  // mark end of line

    return string;  // success

}

// parse flash-stored config.ini into memory struct
int config_parse_stream(FLASH_STREAM *flash_stream) {
    return ini_parse_stream(config_reader, flash_stream, config_handler, &system_config);
}

// expose global config
SYSTEM_CONFIG config_instance() {
    return system_config;
}

// send g-code commands to the printer
void config_issue_gcode(const char *command_list) {
//    char *command = strtok(command_list, "\n");
//    while (command != NULL) {
//        storeCmd("%s\n", command);
//        command = strtok(NULL, "\n");
//    }
}

const char boolean_true_list[][8] =
        {
          "1", "t", "y", "true", "yes", "on",
        };

bool config_parse_bool(const char *value) {
    for (int index = 0; index < COUNT(boolean_true_list); index++) {
        if (strcmp(value, boolean_true_list[index]) == 0) {
            return true;
        }
    }
    return false;
}

int config_parse_int(const char *value) {
    return atoi(value);
}

int config_find_icon(const char *icon_name) {
    return icon_find_index(icon_name);
}
