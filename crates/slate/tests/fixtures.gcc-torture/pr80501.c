/* PR rtl-optimization/80501 */

signed char v = 0;

static signed char foo(int x, int y) { return x << y; }

__attribute__((noinline, noclone)) int bar(void) {
  return foo(v >= 0, __CHAR_BIT__ - 1) >= 1;
}

int main() {
  if (sizeof(int) > sizeof(char) && bar() != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/80501
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i8 = (0 as i32) as i8;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) -> i8 {
// CHECK-NEXT:     (x << y) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     ((foo(
// CHECK-NEXT:         (((unsafe { v }) as i32) >= (0 as i32)) as i32,
// CHECK-NEXT:         (8 as i32) - (1 as i32),
// CHECK-NEXT:     ) as i32)
// CHECK-NEXT:         >= (1 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (4 as u64) > (1 as u64) {
// CHECK-NEXT:         [[V_0]] = bar() != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
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
