//
// config file support
//

#pragma once

#include <stdbool.h>

// produce synthetic config entry, uses "__" separator
#define CONFIG_ENTRY(SECTION,NAME) SECTION##__##NAME

// define the config struct type
typedef struct {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE) char * CONFIG_ENTRY(SECTION,NAME) ;
#include "config.inc"
#undef  X_ENTRY
} SYSTEM_CONFIG;

// expose global config
SYSTEM_CONFIG config_instance();

// send g-code commands to the printer
void config_issue_gcode(const char *command_text);

bool config_parse_bool(const char *value);
int config_parse_int(const char *value);
int config_find_icon(const char *icon_name);
