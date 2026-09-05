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

// COMMON-DAG: bitint::BInt::<65, 2, 16>::from_decimal_str("333")
// COMMON-DAG: bitint::BInt::<65, 2, 16>::from_decimal_str("777")

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: BitIntOrArray = BitIntOrArray {
// LOWERING-DAG:     bytes: [
// LOWERING-DAG:         97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// LOWERING-DAG:     ],
// LOWERING-DAG: };
// LOWERING-DAG: item = {{_v[0-9]+}};
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
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:     printf(
// LOWERING-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:         {{_v[0-9]+}},
// LOWERING-DAG:     )
// LOWERING-DAG: };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: item = BitIntOrArray {
// REWRITES-DAG:     bytes: [
// REWRITES-DAG:         97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// REWRITES-DAG:     ],
// REWRITES-DAG: };
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = unsafe { (*values)[1].inner.prefix };
// REWRITES-DAG: let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[1].inner.value };
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%zu %zu %d %d %lld %d %c%c%c\n".as_ptr(),
// REWRITES-DAG:         24 as u64,
// REWRITES-DAG:         40 as u64,
// REWRITES-DAG:         (unsafe { (*values)[1].tag }) as i32,
// REWRITES-DAG:         {{_v[0-9]+}},
// REWRITES-DAG:         {{_v[0-9]+}}.to_i128() as i64,
// REWRITES-DAG:         (unsafe { (*values)[1].tail }) as i32,
// REWRITES-DAG:         (unsafe { item.bytes[0] }) as i32,
// REWRITES-DAG:         (unsafe { item.bytes[1] }) as i32,
// REWRITES-DAG:         (unsafe { item.bytes[2] }) as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
