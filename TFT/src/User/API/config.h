//
// config file support
//

#pragma once

#include <stdbool.h>

#include "variants.h"
#include "Configuration.h"

// produce synthetic config entry, uses "__" separator
#define CONFIG_ENTRY(SECTION,NAME) SECTION##__##NAME

// define the config struct type, linear list of fields
typedef struct {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE) char * CONFIG_ENTRY(SECTION,NAME) ;
#include "config.inc"
#undef  X_ENTRY
} SYSTEM_CONFIG;

// context for reading from flash-stored config.ini
typedef struct {
    u32 flash_addr;
    u32 flash_tail;
    u16 buff_size;
    u8 *text_buffer;
    u8 *text_point;
} FLASH_STREAM;

// parse flash-stored config.ini into memory struct
int config_parse_stream(FLASH_STREAM *flash_stream);

// expose global config
SYSTEM_CONFIG config_instance();

// tokenize and send g-code commands to the printer
void config_issue_gcode(const char *command_text);

// scalar value parser
bool config_parse_bool(const char *value);
int config_parse_int(const char *value);

// math expression parser
float config_parse_expr(const char *value);

// find icon index by file name (no suffix)
int config_find_icon(const char *icon_name);
