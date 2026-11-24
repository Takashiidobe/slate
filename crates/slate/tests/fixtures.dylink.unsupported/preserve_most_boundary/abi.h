typedef void (*clobber_fn)(void);
typedef __attribute__((preserve_most)) void callback_fn(clobber_fn);
