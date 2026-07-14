typedef enum shared_mode_e {
    SHARED_IDLE,
    SHARED_READY
} shared_mode_t;

struct shared_value {
    int value;
    shared_mode_t mode;
};
