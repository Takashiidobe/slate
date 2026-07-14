#include "../src/shared.h"

extern int square(int x);
extern int bump(void);

int main(void) {
    struct shared_value value = { 3, SHARED_READY };
    return square(value.value) + bump() + (value.mode == SHARED_READY ? 0 : 1);
}
