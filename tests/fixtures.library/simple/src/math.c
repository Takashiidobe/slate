#include "shared.h"

int square(int x) {
    struct shared_value value = { x, SHARED_READY };
    return value.value * value.value;
}
