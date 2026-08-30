#include <stdio.h>

union BitIntOrArray {
  _BitInt(65) bits;
  char bytes[20];
};

struct NestedBitInt {
  char tag;
  struct {
    int prefix;
    _BitInt(65) value;
  } inner;
  short tail;
};

static struct NestedBitInt values[2] = {
    {.tag = 1, .inner = {.prefix = 2, .value = 333}, .tail = 4},
    {.tag = 5, .inner = {.prefix = 6, .value = 777}, .tail = 8},
};

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  union BitIntOrArray item = {.bytes = {'a', 'b', 'c'}};
  // @rewrite-end
  // @lowering-end
  // @lowering-begin
  // @rewrite-begin
  printf("%zu %zu %d %d %lld %d %c%c%c\n", sizeof(item), sizeof(values[0]),
         values[1].tag, values[1].inner.prefix,
         (long long)values[1].inner.value, values[1].tail, item.bytes[0],
         item.bytes[1], item.bytes[2]);
  // @rewrite-end
  // @lowering-end
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: item = BitIntOrArray { bytes: [97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 24;
// LOWERING-DAG: let {{_v[0-9]+}}: u64 = 40;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { (*values)[({{_v[0-9]+}} as usize)].tag };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*values)[({{_v[0-9]+}} as usize)].inner.prefix };
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[({{_v[0-9]+}} as usize)].inner.value };
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.to_i128() as i64;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i16 = unsafe { (*values)[({{_v[0-9]+}} as usize)].tail };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { item.bytes[({{_v[0-9]+}} as usize)] };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { item.bytes[({{_v[0-9]+}} as usize)] };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 2;
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { item.bytes[({{_v[0-9]+}} as usize)] };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: item = BitIntOrArray { bytes: [97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 24;
// REWRITES-DAG: let {{_v[0-9]+}}: u64 = 40;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = (unsafe { (*values)[({{_v[0-9]+}} as usize)].tag }) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*values)[({{_v[0-9]+}} as usize)].inner.prefix };
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[({{_v[0-9]+}} as usize)].inner.value };
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.to_i128() as i64;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, (unsafe { (*values)[({{_v[0-9]+}} as usize)].tail }) as i32, (unsafe { item.bytes[({{_v[0-9]+}} as usize)] }) as i32, (unsafe { item.bytes[({{_v[0-9]+}} as usize)] }) as i32, (unsafe { item.bytes[({{_v[0-9]+}} as usize)] }) as i32) };
// SLATE-FILECHECK-END rewrites

// COMMON-DAG: bitint::BInt::<65, 2, 16>::from_i128(333 as i128)
// COMMON-DAG: bitint::BInt::<65, 2, 16>::from_i128(777 as i128)
