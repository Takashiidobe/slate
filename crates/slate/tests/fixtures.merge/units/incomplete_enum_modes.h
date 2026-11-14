enum mode_status;
enum access_mode;

struct mode_funcs {
    enum mode_status (*mode_valid)(int width, int height);
    enum access_mode (*access)(int flags);
    int id;
};

struct mode_config {
    const struct mode_funcs *funcs;
    enum access_mode *pending;
    int count;
};

struct mode_config *mode_config_new(void);
int mode_config_check(const struct mode_config *config, int width, int height);
int mode_config_access(struct mode_config *config, int flags);
void mode_config_free(struct mode_config *config);
