#include <stdio.h>

#define SZ 512
#define BITS ((SZ - 3 * 4) / 8 * 8 / 1 * 8)

static const signed char tab[4] = {0, 1, 2, -3};
static const short tab16[2] = {1279, 195};

// @check-fn-begin
static unsigned int flags(unsigned int f) { return f & (16777216 | 33554432); }
// @check-fn-end

// @check-fn-begin
static int bit(unsigned int i) { return 1 << (i & (8 - 1)); }
// @check-fn-end

// @check-fn-begin
static unsigned long big(void) { return BITS; }
// @check-fn-end

// @check-fn-begin
static unsigned char wrapped(void) { return (unsigned char)(250 + 10); }
// @check-fn-end

// @check-fn-begin
static int overflowing(int n) { return n + (2147483647 - 2147483646 + 1); }
// @check-fn-end

int main(void) {
  printf("%lu %d %u %d %d %d\n", big(), bit(5), flags(0x3000000), tab[3], tab16[0], tab16[1]);
  printf("%d %d\n", wrapped(), overflowing(7));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn flags(f: u32) -> u32 {
// CHECK-NEXT:     f & 50331648u32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bit(i: u32) -> i32 {
// CHECK-NEXT:     1i32 << (i & 7u32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn big() -> u64 {
// CHECK-NEXT:     3968u64
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn wrapped() -> u8 {
// CHECK-NEXT:     4u8
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn overflowing(n: i32) -> i32 {
// CHECK-NEXT:     n + (2147483647 - 2147483646 + 1)
// CHECK: {{^}}}
// SLATE-FILECHECK-END
