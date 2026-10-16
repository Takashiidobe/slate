/* PR c++/55137 */

extern void abort(void);

int foo(unsigned int x) { return ((int)(x + 1U) + 1) < (int)x; }

int bar(unsigned int x) { return (int)(x + 1U) + 1; }

int baz(unsigned int x) { return x + 1U; }

int main() {
  if (foo(__INT_MAX__) != (bar(__INT_MAX__) < __INT_MAX__) ||
      foo(__INT_MAX__) != ((int)baz(__INT_MAX__) + 1 < __INT_MAX__))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c++/55137
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) -> i32 {
// CHECK-NEXT:     ((x.wrapping_add(1 as u32) as i32) + (1 as i32) < (x as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32) -> i32 {
// CHECK-NEXT:     (x.wrapping_add(1 as u32) as i32) + (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: u32) -> i32 {
// CHECK-NEXT:     x.wrapping_add(1 as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo((2147483647 as i32) as u32)
// CHECK-NEXT:         != ((bar((2147483647 as i32) as u32) < (2147483647 as i32)) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo((2147483647 as i32) as u32)
// CHECK-NEXT:             != ((baz((2147483647 as i32) as u32) + (1 as i32) < (2147483647 as i32)) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
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
