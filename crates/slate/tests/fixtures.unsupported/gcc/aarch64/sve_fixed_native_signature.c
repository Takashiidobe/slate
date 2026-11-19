// { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
#include <arm_sve.h>
#include <stdio.h>

typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(512)));

fixed_i32 exported_add(fixed_i32 a, fixed_i32 b) { return a + b; }

int main(void) {
    fixed_i32 a = {1, 2, 3, 4};
    fixed_i32 sum = exported_add(a, a);
    printf("%d %d\n", sum[0], sum[3]);
    return 0;
}
