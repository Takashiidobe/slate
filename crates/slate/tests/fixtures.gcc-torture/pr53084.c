/* PR middle-end/53084 */

extern void abort(void);

__attribute__((noinline, noclone)) void bar(const char *p) {
  if (p[0] != 'o' || p[1] != 'o' || p[2])
    abort();
}

int main() {
  static const char *const foo[] = {"foo" + 1};
  bar(foo[0]);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/53084
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *const i8) {
// CHECK-NEXT:     if ((unsafe { *unsafe { p.offset((0 as i32) as isize) } }) as i32) != (111 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { p.offset((1 as i32) as isize) } }) as i32) != (111 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { p.offset((2 as i32) as isize) } }) != (0 as i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar(unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of!(foo) as *const *const i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut foo: [*const i8; 1] =
// CHECK-NEXT:     [(unsafe { (b"foo\0".as_ptr() as *mut i8).offset((1 as i32) as isize) }) as *const i8];
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
