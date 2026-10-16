/* PR rtl-optimization/61673 */

char e;

__attribute__((noinline, noclone)) void bar(char x) {
  if (x != 0x54 && x != (char)0x87)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void foo(const char *x) {
  char d = x[0];
  int  c = d;
  if ((c >= 0 && c <= 0x7f) == 0)
    e = d;
  bar(d);
}

__attribute__((noinline, noclone)) void baz(const char *x) {
  char d = x[0];
  int  c = d;
  if ((c >= 0 && c <= 0x7f) == 0)
    e = d;
}

int main() {
  const char c[] = {0x54, 0x87};
  e              = 0x21;
  foo(c);
  if (e != 0x21)
    __builtin_abort();
  foo(c + 1);
  if (e != (char)0x87)
    __builtin_abort();
  e = 0x21;
  baz(c);
  if (e != 0x21)
    __builtin_abort();
  baz(c + 1);
  if (e != (char)0x87)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/61673
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i8) {
// CHECK-NEXT:     if (x as i32) != (84 as i32) && (x as i32) != (((135 as i32) as i8) as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *const i8) {
// CHECK-NEXT:     let mut d: i8 = unsafe { *unsafe { x.offset((0 as i32) as isize) } };
// CHECK-NEXT:     let mut c: i32 = d as i32;
// CHECK-NEXT:     if ((c >= (0 as i32) && c <= (127 as i32)) as i32) == (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = d;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(d);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: *const i8) {
// CHECK-NEXT:     let mut d: i8 = unsafe { *unsafe { x.offset((0 as i32) as isize) } };
// CHECK-NEXT:     let mut c: i32 = d as i32;
// CHECK-NEXT:     if ((c >= (0 as i32) && c <= (127 as i32)) as i32) == (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = d;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: [i8; 2] = [(84 as i32) as i8, (135 as i32) as i8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = (33 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(c.as_ptr() as *const i8);
// CHECK-NEXT:     if ((unsafe { e }) as i32) != (33 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(unsafe { (c.as_ptr() as *const i8).offset((1 as i32) as isize) });
// CHECK-NEXT:     if ((unsafe { e }) as i32) != (((135 as i32) as i8) as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = (33 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(c.as_ptr() as *const i8);
// CHECK-NEXT:     if ((unsafe { e }) as i32) != (33 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(unsafe { (c.as_ptr() as *const i8).offset((1 as i32) as isize) });
// CHECK-NEXT:     if ((unsafe { e }) as i32) != (((135 as i32) as i8) as i32) {
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
