#include "shared.h"

#define PROJECT_WARNING_TOKEN expanded
#warning PROJECT_WARNING_TOKEN remains unexpanded

int square(int x) {
    struct shared_value value = { x, SHARED_READY };
    return value.value * value.value;
}
