#include "shared.h"

int square(int x) {
    struct shared_value value = { x };
    return value.value * value.value;
}
