//
// config file support
//

#pragma once

// define the config struct type
typedef struct {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE) char * SECTION##_##NAME ;
#include "config.x"
#undef  X_ENTRY
} SYSTEM_CONFIG;

// expose global config
SYSTEM_CONFIG config_instance();

// send g-code commands to the printer
void config_issue_gcode(const char *command_text);
