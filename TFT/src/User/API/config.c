//
//
//

#include "config.h"
#include "ini.h"

typedef struct {
    int version;
    const char *name;
    const char *email;
} CONFIG;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

static int configHandler(void *user, const char *section, const char *name, const char *value) {

    CONFIG *config = (CONFIG*) user;

    if (MATCH("protocol", "version")) {
        config->version = atoi(value);
    } else if (MATCH("user", "name")) {
        config->name = strdup(value);
    } else if (MATCH("user", "email")) {
        config->email = strdup(value);
    } else {
        return 0; /* error: wrong section/option */
    }
    return 1;
}

int configVerify(void) {

    CONFIG config;

    if (ini_parse("test.ini", configHandler, &config) < 0) {
        printf("Can't load 'test.ini'\n");
        return 1;
    }

    printf("Config loaded from 'test.ini': version=%d, name=%s, email=%s\n", config.version, config.name, config.email);

    free((void*) config.name);
    free((void*) config.email);

    return 0;
}
