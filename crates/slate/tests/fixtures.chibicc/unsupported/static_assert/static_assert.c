#include "test.h"

// Test _Static_assert at file scope
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");
_Static_assert(sizeof(long) == 8, "long must be 8 bytes");
_Static_assert(sizeof(char) == 1, "char must be 1 byte");
_Static_assert(1 + 1 == 2, "math is broken");
_Static_assert(sizeof(void *) == 8, "pointer must be 8 bytes");

// Test with expressions
_Static_assert(sizeof(int) >= sizeof(short), "int must be at least as large as short");
_Static_assert((1 << 4) == 16, "bit shift failed");

// Test static_assert macro (C11 convenience macro)
static_assert(sizeof(short) == 2, "short must be 2 bytes");

int main() {
    // Test _Static_assert at block scope
    _Static_assert(sizeof(int) == 4, "int must be 4 bytes in function");
    static_assert(1, "constant 1 should be true");

    // Test with local sizeof
    int arr[10];
    _Static_assert(sizeof(arr) == 40, "array size check");

    printf("OK\n");
    return 0;
}
