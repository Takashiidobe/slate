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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: BitIntOrArray = BitIntOrArray {
// COMMON-LOWERING-DAG:     bytes: [
// COMMON-LOWERING-DAG:         97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// COMMON-LOWERING-DAG:     ],
// COMMON-LOWERING-DAG: };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.prefix };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[({{__v[0-9]+}} as usize)].inner.value };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.to_i128() as i64;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i16 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tail };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:     printf(
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         {{__v[0-9]+}},
// COMMON-LOWERING-DAG:     )
// COMMON-LOWERING-DAG: };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: item = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u64 = 24;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: u64 = 40;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: *item = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%zu %zu %d %d %lld %d %c%c%c\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u64 = 32;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u64 = 64;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { (*values)[({{__v[0-9]+}} as usize)].tag };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { item.bytes[({{__v[0-9]+}} as usize)] };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG:     bytes: [
// COMMON-REWRITES-DAG:         97, 98, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// COMMON-REWRITES-DAG:     ],
// COMMON-REWRITES-DAG: };
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32 = unsafe { (*values)[1].inner.prefix };
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: bitint::BInt<65, 2, 16> = unsafe { (*values)[1].inner.value };
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%zu %zu %d %d %lld %d %c%c%c\n".as_ptr(),
// COMMON-REWRITES-DAG:         (unsafe { (*values)[1].tag }) as i32,
// COMMON-REWRITES-DAG:         {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}.to_i128() as i64,
// COMMON-REWRITES-DAG:         (unsafe { (*values)[1].tail }) as i32,
// COMMON-REWRITES-DAG:         (unsafe { item.bytes[0] }) as i32,
// COMMON-REWRITES-DAG:         (unsafe { item.bytes[1] }) as i32,
// COMMON-REWRITES-DAG:         (unsafe { item.bytes[2] }) as i32,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: item = BitIntOrArray {
// REWRITES-X86_64-GNU-DAG:         24 as u64,
// REWRITES-X86_64-GNU-DAG:         40 as u64,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: *item = BitIntOrArray {
// REWRITES-AARCH64-GNU-DAG:         32 as u64,
// REWRITES-AARCH64-GNU-DAG:         64 as u64,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
