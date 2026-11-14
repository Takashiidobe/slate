#include <stdio.h>
#include "modes.h"

static int config_score(const struct mode_config *config) {
    int score = config->count * 10 + config->funcs->id;
    if (config->funcs->mode_valid != 0)
        score += 100;
    if (config->pending != 0)
        score += 1000;
    return score;
}

int main(void) {
    struct mode_config *config = mode_config_new();
    printf("%d\n", config_score(config));
    printf("%d %d %d\n", mode_config_check(config, 5, 5), mode_config_check(config, 20, 20),
           mode_config_check(config, 0, 3));
    printf("%d %d\n", mode_config_access(config, 1), mode_config_access(config, 2));
    mode_config_free(config);
    return 0;
}
