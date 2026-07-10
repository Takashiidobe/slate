#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct FixedPair {
    int16_t left;
    uint32_t right;
    size_t count;
};

static uint32_t global_u32 = 4000000000u;
static size_t global_size = 7;

static int32_t add_i32(int32_t a, int16_t b) {
    int32_t sum = a + b;
    return sum;
}

static uint64_t widen_u32(uint32_t value) {
    uint64_t wide = value + global_size;
    return wide;
}

static int use_fixed_types(void) {
    int8_t small = -5;
    uint8_t byte = 250;
    int16_t short_value = 1200;
    uint16_t ushort_value = 65000;
    int32_t signed_value = add_i32(100000, short_value);
    uint64_t wide = widen_u32(global_u32);
    struct FixedPair pair;
    pair.left = short_value;
    pair.right = global_u32;
    pair.count = global_size + 3;
    return small + byte + pair.left + signed_value + ushort_value + pair.count + wide;
}

int main(void) {
    printf("%d\n", add_i32(10, 20));
    printf("%lu\n", widen_u32(5));
    printf("%d\n", use_fixed_types());
    return 0;
}
