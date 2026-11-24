#include <stdint.h>

enum { ordinary_argument = 7 };

typedef __attribute__((swiftcall)) int callback_fn(
    int value, void *context __attribute__((swift_context)), uintptr_t ordinary);
