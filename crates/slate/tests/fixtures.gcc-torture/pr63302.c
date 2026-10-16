/* PR tree-optimization/63302 */

#ifdef __SIZEOF_INT128__
#if __SIZEOF_INT128__ * __CHAR_BIT__ == 128
#define USE_INT128
#endif
#endif
#if __SIZEOF_LONG_LONG__ * __CHAR_BIT__ == 64
#define USE_LLONG
#endif

#ifdef USE_INT128
__attribute__((noinline, noclone)) int foo(__int128 x) {
  __int128 v = x & (((__int128)-1 << 63) | 0x7ff);

  return v == 0 || v == ((__int128)-1 << 63);
}
#endif

#ifdef USE_LLONG
__attribute__((noinline, noclone)) int bar(long long x) {
  long long v = x & (((long long)-1 << 31) | 0x7ff);

  return v == 0 || v == ((long long)-1 << 31);
}
#endif

int main() {
#ifdef USE_INT128
  if (foo(0) != 1 || foo(1) != 0 || foo(0x800) != 1 || foo(0x801) != 0 ||
      foo((__int128)1 << 63) != 0 || foo((__int128)-1 << 63) != 1 ||
      foo(((__int128)-1 << 63) | 1) != 0 ||
      foo(((__int128)-1 << 63) | 0x800) != 1 ||
      foo(((__int128)-1 << 63) | 0x801) != 0)
    __builtin_abort();
#endif
#ifdef USE_LLONG
  if (bar(0) != 1 || bar(1) != 0 || bar(0x800) != 1 || bar(0x801) != 0 ||
      bar(1LL << 31) != 0 || bar(-1LL << 31) != 1 ||
      bar((-1LL << 31) | 1) != 0 || bar((-1LL << 31) | 0x800) != 1 ||
      bar((-1LL << 31) | 0x801) != 0)
    __builtin_abort();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/63302
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i128) -> i32 {
// CHECK-NEXT:     let mut v: i128 = x & ((-(1 as i32) as i128) << (63 as i32) | ((2047 as i32) as i128));
// CHECK-NEXT:     (v == ((0 as i32) as i128) || v == (-(1 as i32) as i128) << (63 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i64) -> i32 {
// CHECK-NEXT:     let mut v: i64 = x & ((-(1 as i32) as i64) << (31 as i32) | ((2047 as i32) as i64));
// CHECK-NEXT:     (v == ((0 as i32) as i64) || v == (-(1 as i32) as i64) << (31 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo((0 as i32) as i128) != (1 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo((1 as i32) as i128) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo((2048 as i32) as i128) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = foo((2049 as i32) as i128) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = foo(((1 as i32) as i128) << (63 as i32)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = foo((-(1 as i32) as i128) << (63 as i32)) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = foo((-(1 as i32) as i128) << (63 as i32) | ((1 as i32) as i128)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = foo((-(1 as i32) as i128) << (63 as i32) | ((2048 as i32) as i128)) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = foo((-(1 as i32) as i128) << (63 as i32) | ((2049 as i32) as i128)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if bar((0 as i32) as i64) != (1 as i32) {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = bar((1 as i32) as i64) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_8]] {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = bar((2048 as i32) as i64) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = bar((2049 as i32) as i64) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         [[V_11]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_11]] = bar((1 as i64) << (31 as i32)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_11]] {
// CHECK-NEXT:         [[V_12]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_12]] = bar(-(1 as i64) << (31 as i32)) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_12]] {
// CHECK-NEXT:         [[V_13]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_13]] = bar(-(1 as i64) << (31 as i32) | ((1 as i32) as i64)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_13]] {
// CHECK-NEXT:         [[V_14]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_14]] = bar(-(1 as i64) << (31 as i32) | ((2048 as i32) as i64)) != (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_14]] {
// CHECK-NEXT:         [[V_15]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_15]] = bar(-(1 as i64) << (31 as i32) | ((2049 as i32) as i64)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_15]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
