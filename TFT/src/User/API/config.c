//
// config file support
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "ini.h"
#include "interfaceCmd.h"

// declare global instance and populate with default values
SYSTEM_CONFIG system_config = {
#define X_ENTRY(SECTION, NAME, DEFAULT_VALUE) DEFAULT_VALUE ,
#include "config.x"
#undef  X_ENTRY
        };

// generate config parser events reactor
int config_handler(void *user, const char *section, const char *name, const char *value) {
    SYSTEM_CONFIG *config = (SYSTEM_CONFIG*) user;
    if (0) {
    }
#define X_ENTRY(SECTION, NAME, DEF_VAL) else if \
    (strcmp(section, #SECTION)==0 && strcmp(name, #NAME)==0) { config->SECTION##_##NAME = strdup(value); }
#include "config.x"
#undef  X_ENTRY
    else {
        return -1; // failure: wrong section/name
    }
    return 0; // success: match was found
}

// apply config entries form config file text
void config_update(const char *config_text) {
    if (ini_parse_string(config_text, config_handler, &system_config) < 0) {
        printf("can not parse config\n");
    }
}

// expose global config
SYSTEM_CONFIG config_instance() {
    return system_config;
}

// send g-code commands to the printer
void config_issue_gcode(const char *command_list) {
    char *command = strtok(command_list, "\n");
    while (command != NULL) {
        storeCmd("%s\n", command);
        command = strtok(NULL, "\n");
    }
}
