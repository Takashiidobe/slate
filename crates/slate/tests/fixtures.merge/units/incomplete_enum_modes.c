#include <stdlib.h>
#include "incomplete_enum_modes.h"

enum mode_status { MODE_OK = 0, MODE_BAD = -2, MODE_ERROR = -1 };
enum access_mode { ACCESS_READ = 1, ACCESS_WRITE = 2, ACCESS_BOTH = 3 };

static enum mode_status mode_valid(int width, int height) {
    if (width <= 0 || height <= 0)
        return MODE_ERROR;
    return width * height > 100 ? MODE_BAD : MODE_OK;
}

static enum access_mode access_for(int flags) { return flags & 1 ? ACCESS_BOTH : ACCESS_READ; }

static const struct mode_funcs funcs = {mode_valid, access_for, 7};

static enum access_mode pending_access = ACCESS_WRITE;

struct mode_config *mode_config_new(void) {
    struct mode_config *config = malloc(sizeof *config);
    config->funcs = &funcs;
    config->pending = &pending_access;
    config->count = 1;
    return config;
}

int mode_config_check(const struct mode_config *config, int width, int height) {
    return config->funcs->mode_valid(width, height);
}

int mode_config_access(struct mode_config *config, int flags) {
    *config->pending = config->funcs->access(flags);
    return *config->pending;
}

void mode_config_free(struct mode_config *config) { free(config); }
