typedef int local_int;
#include <outer.h>

static inline int local_wrapper(int value) {
    return system_call(value);
}
