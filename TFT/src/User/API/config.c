//
// config file support
//

// config parser
#define INI_USE_STACK 0
#define INI_MAX_LINE 256
#include "ini.h"

// expression calculator
#include "tinyexpr.h"

#include "config.h"
#include "includes.h"
#include "Configuration.h"

// declare global instance and populate with default values
SYSTEM_CONFIG system_config = {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE)   DEFAULT_VALUE,
#include "config.inc"
#undef  X_ENTRY
        };

// remember last collected config entry
static char **private_collect_entry = NULL;

// collect multi-line config entries into one
void private_config_collect(char **entry, const char *value) {
    if (entry != private_collect_entry) {
        // first visit, overwrite default
        private_collect_entry = entry;  // memento
        *entry = strdup(value);  // allocate first
    } else {
        // following visit, concat with existing
        char text_buff[INI_MAX_LINE];
        my_sprintf(text_buff, "%s\n%s", *entry, value);  // use \n
        free(*entry);  // recycle past
        *entry = strdup(text_buff);  // allocate next
    }
}

// generate config parser event reactor; see ini_handler
int private_config_handler(void *user, const char *section, const char *name, const char *value) {
    SYSTEM_CONFIG *config = (SYSTEM_CONFIG*) user;
    if (false) {
    } /* x-macro start */
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE)   \
    else if (strcmp(section, #SECTION)==0 && strcmp(name, #NAME)==0) {  \
        private_config_collect(&(config->CONFIG_ENTRY(SECTION,NAME)), value); \
        return 1; /*success: found secion/name */ \
    } \
      /* x-macro finish */
#include "config.inc"
#undef  X_ENTRY
    else {
        return 0; /* failure: missing section/name */
    }
}

// fgets-style flash stream buffer reader; see ini_reader
char* private_config_reader(char *string, int length, void *stream) {
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

// parse flash-stored config.ini file into memory struct
int config_parse_stream(FLASH_STREAM *flash_stream) {
    return ini_parse_stream(private_config_reader, flash_stream, private_config_handler, &system_config);
}

// expose global config
const SYSTEM_CONFIG* config_instance() {
    return &system_config;
}

// tokenize and send g-code commands to the printer
void config_issue_gcode(const char *command_list) {
    char *buffer = strdup(command_list);  // mutable
    char *command = strtok(buffer, "\n");
    while (command != NULL) {
        //render_plain_message(command);
        storeCmd("%s\n", command);
        command = strtok(NULL, "\n");
    }
    free(buffer);
}

static const char boolean_true_list[][8] =
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

float config_parse_expr(const char *value) {
    int error = 0;
    float result = te_interp(value, &error);
    // TODO error trace
    return result;
}

// find icon index by name (no suffix)
int config_find_icon(const char *icon_name) {
    return icon_find_index(icon_name);
}

