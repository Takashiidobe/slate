/* PR tree-optimization/78720 */

__attribute__((noinline, noclone)) long int foo(signed char x) {
  return x < 0 ? 0x80000L : 0L;
}

__attribute__((noinline, noclone)) long int bar(signed char x) {
  return x < 0 ? 0x80L : 0L;
}

__attribute__((noinline, noclone)) long int baz(signed char x) {
  return x < 0 ? 0x20L : 0L;
}

int main() {
  if (foo(-1) != 0x80000L || bar(-1) != 0x80L || baz(-1) != 0x20L ||
      foo(0) != 0L || bar(0) != 0L || baz(0) != 0L || foo(31) != 0L ||
      bar(31) != 0L || baz(31) != 0L)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/78720
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i8) -> i64 {
// CHECK-NEXT:     if (x as i32) < (0 as i32) {
// CHECK-NEXT:         524288 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i8) -> i64 {
// CHECK-NEXT:     if (x as i32) < (0 as i32) {
// CHECK-NEXT:         128 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: i8) -> i64 {
// CHECK-NEXT:     if (x as i32) < (0 as i32) {
// CHECK-NEXT:         32 as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i64
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(-(1 as i32) as i8) != (524288 as i64) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = bar(-(1 as i32) as i8) != (128 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = baz(-(1 as i32) as i8) != (32 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = foo((0 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = bar((0 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = baz((0 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = foo((31 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = bar((31 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = baz((31 as i32) as i8) != (0 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]] {
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
