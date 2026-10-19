struct counter {
    int value;
};

struct api_routines {
    int (*add)(int, int);
    int (*sub)(int, int);
    void (*bump)(struct counter *, int);
    const char *(*name)(void);
};

int add(int a, int b);
int sub(int a, int b);
void bump(struct counter *c, int by);
const char *name(void);

extern const struct api_routines api;
